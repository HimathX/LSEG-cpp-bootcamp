#pragma once
#include <array>
#include <string>
#include <vector>
#include "Order.h"
#include "OrderBook.h"
#include "ExecutionReport.h"

class MatchingEngine {
public:
    explicit MatchingEngine(const std::string& engineName);

    void matchOrder(Order order, std::vector<ExecutionReport>& outReports);

private:
    OrderBook& getOrderBook(InstrumentId instrumentId);
    void appendReport(std::vector<ExecutionReport>& outReports,
                      const std::string& orderID,
                      const std::string& clientOrderID,
                      const std::string& instrument,
                      int side,
                      int status,
                      int quantity,
                      double price) const;

    std::string engine_;
    std::array<OrderBook, 5> orderBooks_;
};
