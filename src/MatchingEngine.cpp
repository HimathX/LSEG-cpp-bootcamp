#include "MatchingEngine.h"
#include <stdexcept>
#include "OrderIDGenerator.h"

MatchingEngine::MatchingEngine(const std::string& engineName)
    : engine_(engineName)
{
    orderBooks_.emplace("Rose", OrderBook("Rose"));
    orderBooks_.emplace("Lavender", OrderBook("Lavender"));
    orderBooks_.emplace("Lotus", OrderBook("Lotus"));
    orderBooks_.emplace("Tulip", OrderBook("Tulip"));
    orderBooks_.emplace("Orchid", OrderBook("Orchid"));
}

std::vector<ExecutionReport> MatchingEngine::MatchOrder(const Order& order)
{
    OrderBook& orderBook = getOrderBook(order.instrument);
    const bool oppositeSideHasOrders = (order.side == 1) ? orderBook.hasSellOrders()
                                                         : orderBook.hasBuyOrders();

    // If Opposite site has orders check whether agressives passive then matching criteria
    if (oppositeSideHasOrders)
    {
        throw std::logic_error("Matching logic not implemented yet"); //Aggressive passive criteria
       // to be implemented here
    }

    orderBook.addOrder(order); // Has to be matched correctly because if orders get filled they do not go to orderbook

    ExecutionReport report;
    report.orderID = OrderIDGenerator::getNext();
    report.clientOrderID = order.clientOrderID;
    report.instrument = order.instrument;
    report.side = order.side;
    report.price = order.price;
    report.quantity = order.quantity;
    report.status = 0;

    return {report};
}

OrderBook& MatchingEngine::getOrderBook(const std::string& instrument)
{
    auto it = orderBooks_.find(instrument);
    if (it == orderBooks_.end())
    {
        throw std::invalid_argument("Unknown instrument: " + instrument);
    }

    return it->second;
}
