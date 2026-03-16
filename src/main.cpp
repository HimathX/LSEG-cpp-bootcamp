#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "CSVReader.h"
#include "CSVWriter.h"
#include "ExecutionReport.h"
#include "OrderIDGenerator.h"
#include "Validator.h"

int main()
{
    CSVReader reader;
    CSVReadResult result = reader.readOrders("tests/sample_orders_7.csv");

    // Collect all rejections paired with arrival sequence number for correct ordering.
    std::vector<std::pair<std::uint64_t, ExecutionReport>> rejections;

    // --- Parse rejects (CSVReader detected malformed / unparseable rows) ---
    for (const auto &reject : result.parseRejects)
    {
        ExecutionReport rep;
        rep.clientOrderID = reject.clientOrderId;
        rep.instrument = reject.instrument;
        rep.status = 1;
        rep.reason = reject.reason;
        try
        {
            rep.side = std::stoi(reject.sideText);
        }
        catch (...)
        {
            rep.side = 0;
        }
        try
        {
            rep.quantity = std::stoi(reject.quantityText);
        }
        catch (...)
        {
            rep.quantity = 0;
        }
        try
        {
            rep.price = std::stod(reject.priceText);
        }
        catch (...)
        {
            rep.price = 0.0;
        }
        rejections.emplace_back(reject.seqNum, std::move(rep));
    }

    // --- Validation failures (well-formed rows that break business rules) ---
    for (const auto &order : result.validOrders)
    {
        std::string reason = Validator::validate(order);
        if (!reason.empty())
        {
            ExecutionReport rep;
            rep.clientOrderID = order.clientOrderID;
            rep.instrument = order.instrument;
            rep.side = order.side;
            rep.quantity = order.quantity;
            rep.price = order.price;
            rep.status = 1;
            rep.reason = reason;
            rejections.emplace_back(static_cast<std::uint64_t>(order.seqNo), std::move(rep));
        }
        // Passing orders will feed into OrderBook / MatchingEngine in Phase 3.
    }

    // Sort by arrival sequence so Order IDs are assigned in time-priority order.
    std::sort(rejections.begin(), rejections.end(),
              [](const auto &a, const auto &b)
              { return a.first < b.first; });

    CSVWriter writer("execution_rep.csv");
    for (auto &[seq, rep] : rejections)
    {
        rep.orderID = OrderIDGenerator::getNext();
        writer.write(rep);
    }

    std::cout << "Done. " << rejections.size() << " rejection(s) written to execution_rep.csv\n";
    return 0;
}