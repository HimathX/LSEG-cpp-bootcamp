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
        rep.sideText = reject.sideText;
        rep.quantityText = reject.quantityText;
        rep.priceText = reject.priceText;
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

        //Handle the case where an order has more fields than expected, which is a validation failure in Phase 2
        else if (order.hasExtraFields)
        {
            ExecutionReport rep;
            rep.clientOrderID = order.clientOrderID;
            rep.instrument = order.instrument;
            rep.side = order.side;
            rep.quantity = order.quantity;
            rep.price = order.price;
            rep.status = 1;
            rep.reason = "Extra field";
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
