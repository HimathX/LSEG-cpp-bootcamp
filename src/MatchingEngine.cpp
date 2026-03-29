#include "MatchingEngine.h"
#include <algorithm>
#include <stdexcept>
#include "OrderIDGenerator.h"

MatchingEngine::MatchingEngine(const std::string &engineName)
    : engine_(engineName),
      orderBooks_{OrderBook("Rose"),
                  OrderBook("Lavender"),
                  OrderBook("Lotus"),
                  OrderBook("Tulip"),
                  OrderBook("Orchid")}
{
}

void MatchingEngine::matchOrder(Order order, std::vector<ExecutionReport>& outReports)
{
    OrderBook &orderBook = getOrderBook(order.instrumentId);

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

        // Check if prices actually overlap
        const bool priceMatches =
            (order.side == 1) ? (order.priceTick >= restingOrder.priceTick)
                              : (order.priceTick <= restingOrder.priceTick);

        isAggressive = isAggressive || priceMatches; // Once we have a price match, we consider the order aggressive

        if (!priceMatches)
        {
            break; // Prices don't overlap, stop matching
        }

        // --- WE HAVE A MATCH ---

        int tradeQty = std::min(order.quantity, restingOrder.quantity);
        double executionPrice = priceTickToPrice(restingOrder.priceTick); // THE EXAMPLE 5 RULE

        // Update quantities
        restingOrder.quantity -= tradeQty;
        order.quantity -= tradeQty;

        int restingStatus = (restingOrder.quantity == 0) ? 2 : 3; // 2=Fill, 3=PFill
        int aggressiveStatus = (order.quantity == 0) ? 2 : 3;

        appendReport(outReports,
                     order.orderID,
                     order.clientOrderID,
                     order.instrument,
                     order.side,
                     aggressiveStatus,
                     tradeQty,
                     executionPrice);

        appendReport(outReports,
                     restingOrder.orderID,
                     restingOrder.clientOrderID,
                     order.instrument,
                     (order.side == 1) ? 2 : 1,
                     restingStatus,
                     tradeQty,
                     executionPrice);

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
            appendReport(outReports,
                         order.orderID,
                         order.clientOrderID,
                         order.instrument,
                         order.side,
                         0,
                         order.quantity,
                         order.price);
        }
    }
}

OrderBook &MatchingEngine::getOrderBook(InstrumentId instrumentId)
{
    const int index = static_cast<int>(instrumentId);
    if (index < 0 || index >= static_cast<int>(orderBooks_.size()))
    {
        throw std::invalid_argument("Unknown instrument");
    }
    return orderBooks_[static_cast<std::size_t>(index)];
}

void MatchingEngine::appendReport(std::vector<ExecutionReport>& outReports,
                                  const std::string& orderID,
                                  const std::string& clientOrderID,
                                  const std::string& instrument,
                                  int side,
                                  int status,
                                  int quantity,
                                  double price) const
{
    ExecutionReport report;
    report.orderID = orderID;
    report.clientOrderID = clientOrderID;
    report.instrument = instrument;
    report.side = side;
    report.status = status;
    report.quantity = quantity;
    report.price = price;
    outReports.push_back(report);
}
