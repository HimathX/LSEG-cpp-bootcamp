#pragma once
#include <string>
#include <map>
#include <vector>
#include "Order.h"
#include "OrderBook.h"
#include "ExecutionReport.h"

class MatchingEngine {
public:
    explicit MatchingEngine(const std::string& engineName);

    std::vector<ExecutionReport> MatchOrder(const Order& order);

private:
    OrderBook& getOrderBook(const std::string& instrument);

    std::string engine_;
    std::map<std::string, OrderBook> orderBooks_;
};
