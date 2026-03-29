#include "MatchingEngine.h"
#include <stdexcept>
#include <algorithm> // For std::min
#include "OrderIDGenerator.h"

 MatchingEngine::MatchingEngine(const std::string &engineName)
    : engine_(engineName)
{
    orderBooks_.emplace("Rose", OrderBook("Rose"));
    orderBooks_.emplace("Lavender", OrderBook("Lavender"));
    orderBooks_.emplace("Lotus", OrderBook("Lotus"));
    orderBooks_.emplace("Tulip", OrderBook("Tulip"));
    orderBooks_.emplace("Orchid", OrderBook("Orchid"));
}

std::vector<ExecutionReport> MatchingEngine::MatchOrder(const Order &orderInput)
{
    Order order = orderInput;
    std::vector<ExecutionReport> reports;
    OrderBook &orderBook = getOrderBook(order.instrument);

    // 1. Generate the System ID immediately
    order.orderID = OrderIDGenerator::getNext();

    // 2. THE MATCHING LOOP
    bool oppositeSideHasOrders = (order.side == 1) ? orderBook.hasSellOrders() : orderBook.hasBuyOrders();
    bool isAggressive = false;
    while (order.quantity > 0 && oppositeSideHasOrders)
    {
        // Get the best available resting order
        std::deque<RestingOrder> &bestQueue = (order.side == 1) ? orderBook.getBestSellQueue() : orderBook.getBestBuyQueue();
        RestingOrder &restingOrder = bestQueue.front();

        // Convert PriceTick back to double for price matching
        double restingPrice = priceTickToPrice(restingOrder.priceTick);

        // Check if prices actually overlap
        bool priceMatches = false;
        if (order.side == 1)
        {
            priceMatches = (order.price >= restingPrice); // Buyer pays >= cheapest sell price
        }
        else
        {
            priceMatches = (order.price <= restingPrice); // Seller sells <= highest buy price
        }

        isAggressive = isAggressive || priceMatches; // Once we have a price match, we consider the order aggressive

        if (!priceMatches)
        {
            break; // Prices don't overlap, stop matching
        }

        // --- WE HAVE A MATCH ---

        int tradeQty = std::min(order.quantity, restingOrder.quantity);
        double executionPrice = restingPrice; // THE EXAMPLE 5 RULE

        // Update quantities
        restingOrder.quantity -= tradeQty;
        order.quantity -= tradeQty;

        int restingStatus = (restingOrder.quantity == 0) ? 2 : 3; // 2=Fill, 3=PFill
        int aggressiveStatus = (order.quantity == 0) ? 2 : 3;

        // Report for AGGRESSIVE Order
        ExecutionReport aggressiveReport;
        aggressiveReport.orderID = order.orderID;
        aggressiveReport.clientOrderID = order.clientOrderID;
        aggressiveReport.instrument = order.instrument;
        aggressiveReport.side = order.side;
        aggressiveReport.status = aggressiveStatus;
        aggressiveReport.quantity = tradeQty;
        aggressiveReport.price = executionPrice;
        reports.push_back(aggressiveReport);

        // Report for RESTING Order
        ExecutionReport restingReport;
        restingReport.orderID = restingOrder.orderID;
        restingReport.clientOrderID = restingOrder.clientOrderID;
        restingReport.instrument = order.instrument;
        restingReport.side = (order.side == 1) ? 2 : 1; // Opposite side
        restingReport.status = restingStatus;
        restingReport.quantity = tradeQty;
        restingReport.price = executionPrice;
        reports.push_back(restingReport);

        // Clean up empty resting order
        if (restingOrder.quantity == 0)
        {
            bestQueue.pop_front();
            orderBook.removeEmptyPriceLevels();
        }

        oppositeSideHasOrders = (order.side == 1) ? orderBook.hasSellOrders() : orderBook.hasBuyOrders();
    }

    // 3. RESTING THE REMAINDER (Passive)
    // If there's any quantity left, add it to the book as a new resting order
    // But Example 6 in slide says we should not add the remainder to the book, so we will skip this step for now. We can easily add it back later if needed.
    if (order.quantity > 0)
    {
        orderBook.addOrder(order);
        if (!isAggressive)
        {
            ExecutionReport newReport;
            newReport.orderID = order.orderID;
            newReport.clientOrderID = order.clientOrderID;
            newReport.instrument = order.instrument;
            newReport.side = order.side;
            newReport.status = 0; // 0 - New
            newReport.quantity = order.quantity;
            newReport.price = order.price;
            reports.push_back(newReport);
        }

    }

    return reports;
}

OrderBook &MatchingEngine::getOrderBook(const std::string &instrument)
{
    auto it = orderBooks_.find(instrument);
    if (it == orderBooks_.end())
    {
        throw std::invalid_argument("Unknown instrument: " + instrument);
    }
    return it->second;
}
