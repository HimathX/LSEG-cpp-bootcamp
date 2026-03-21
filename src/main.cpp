#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "CSVReader.h"
#include "CSVWriter.h"
#include "ExecutionReport.h"
#include "MatchingEngine.h"
#include "OrderIDGenerator.h"
#include "Validator.h"

// A simple wrapper to combine valid orders and rejects back into a chronological timeline
struct ProcessEvent {
    std::uint64_t seqNum;
    bool isParseReject;
    Order validOrder;
    ParseReject parseReject;
};

int main(int argc, char* argv[])
{
    const auto startTime = std::chrono::steady_clock::now();

    // Default to the official input file name unless overridden by command line
    std::string inputFile = "tests/sample_orders_6.csv";
    if (argc > 1) {
        inputFile = argv[1];
    }

    std::cout << "Starting Flower Exchange Engine...\n";
    std::cout << "Reading from: " << inputFile << "\n";

    CSVReader reader;
    CSVReadResult result;
    try {
        result = reader.readOrders(inputFile);
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    }

    // Combine valid orders and parse rejects into a single timeline
    std::vector<ProcessEvent> timeline;
    timeline.reserve(result.validOrders.size() + result.parseRejects.size());

    for (const auto& order : result.validOrders) {
        ProcessEvent ev;
        ev.seqNum = static_cast<std::uint64_t>(order.seqNo);
        ev.isParseReject = false;
        ev.validOrder = order;
        timeline.push_back(ev);
    }

    for (const auto& reject : result.parseRejects) {
        ProcessEvent ev;
        ev.seqNum = reject.seqNum;
        ev.isParseReject = true;
        ev.parseReject = reject;
        timeline.push_back(ev);
    }

    // Sort the timeline by sequence number to preserve the exact file line order.
    // This ensures OrderIDs (ord1, ord2, etc.) exactly match the LSEG sample files.
    std::sort(timeline.begin(), timeline.end(), [](const ProcessEvent& a, const ProcessEvent& b) {
        return a.seqNum < b.seqNum;
    });

    // Initialize the core components
    MatchingEngine engine("FlowerExchange");
    CSVWriter writer("execution_rep_6.csv");

    int processedCount = 0;
    int rejectCount = 0;
    int matchEventCount = 0;

    // Process the chronological timeline
    for (const auto& ev : timeline)
    {
        if (ev.isParseReject)
        {
            // --- 1. HANDLE MALFORMED ROWS (Phase 2) ---
            ExecutionReport rep;
            rep.orderID = OrderIDGenerator::getNext();
            rep.clientOrderID = ev.parseReject.clientOrderId;
            rep.instrument = ev.parseReject.instrument;
            rep.status = 1; // 1 - Rejected
            rep.reason = ev.parseReject.reason;
            rep.sideText = ev.parseReject.sideText;
            rep.quantityText = ev.parseReject.quantityText;
            rep.priceText = ev.parseReject.priceText;
            
            writer.write(rep);
            rejectCount++;
        }
        else
        {
            // --- 2. HANDLE WELL-FORMED ROWS ---
            const Order& order = ev.validOrder;
            std::string reason = Validator::validate(order);

            if (!reason.empty())
            {
                // Business Rule Validation Failure (e.g. Price < 0)
                ExecutionReport rep;
                rep.orderID = OrderIDGenerator::getNext();
                rep.clientOrderID = order.clientOrderID;
                rep.instrument = order.instrument;
                rep.side = order.side;
                rep.quantity = order.quantity;
                rep.price = order.price;
                rep.status = 1; // 1 - Rejected
                rep.reason = reason;
                
                writer.write(rep);
                rejectCount++;
            }
            else if (order.hasExtraFields)
            {
                // Validation Failure: More columns than expected
                ExecutionReport rep;
                rep.orderID = OrderIDGenerator::getNext();
                rep.clientOrderID = order.clientOrderID;
                rep.instrument = order.instrument;
                rep.side = order.side;
                rep.quantity = order.quantity;
                rep.price = order.price;
                rep.status = 1;
                rep.reason = "Extra field";
                
                writer.write(rep);
                rejectCount++;
            }
            else
            {
                // --- 3. THE MATCHING ENGINE (Phase 3) ---
                // The order is 100% valid. Send it to the Order Book!
                std::vector<ExecutionReport> matchReports = engine.MatchOrder(order);
                
                // Write all generated reports (could be multiple if PFill)
                for (const auto& rep : matchReports)
                {
                    writer.write(rep);
                }
                matchEventCount++;
            }
        }
        processedCount++;
    }

    std::cout << "Done.\n";
    std::cout << "Total rows processed : " << processedCount << "\n";
    std::cout << "Total rejected rows  : " << rejectCount << "\n";
    std::cout << "Total valid orders   : " << matchEventCount << "\n";
    std::cout << "Output saved to execution_rep.csv\n";
    const auto endTime = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime);
    std::cout << "Elapsed time         : " << elapsedMs.count() << " ms\n";

    return 0;
}
