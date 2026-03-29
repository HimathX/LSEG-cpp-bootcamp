#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <optional>
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

namespace
{
    RejectedExecutionReport buildParseRejectReport(const ParseReject &reject)
    {
        RejectedExecutionReport report;
        report.orderID = OrderIDGenerator::getNext();
        report.clientOrderID = reject.clientOrderId;
        report.instrument = reject.instrument;
        report.status = 1; // 1 - Rejected
        report.rejectedReason = reject.reason;
        report.sideText = reject.sideText;
        report.quantityText = reject.quantityText;
        report.priceText = reject.priceText;
        return report;
    }

    RejectedExecutionReport buildValidationRejectReport(const Order &order, const std::string_view reason)
    {
        RejectedExecutionReport report;
        report.orderID = OrderIDGenerator::getNext();
        report.clientOrderID = order.clientOrderID;
        report.instrument = order.instrument;
        report.side = order.side;
        report.quantity = order.quantity;
        report.price = order.price;
        report.status = 1; // 1 - Rejected
        report.rejectedReason = std::string(reason);
        return report;
    }

    std::optional<RejectedExecutionReport> buildRejectedReport(const ProcessEvent &event)
    {
        if (event.isParseReject)
        {
            return buildParseRejectReport(event.parseReject);
        }

        const Order &order = event.validOrder;

        if (order.hasExtraFields)
        {
            return buildValidationRejectReport(order, "Extra field");
        }

        const std::string_view reason = Validator::validate(order);

        if (!reason.empty())
        {
            return buildValidationRejectReport(order, reason);
        }

        return std::nullopt;
    }
} // namespace

int main(int argc, char* argv[])
{
    const auto startTime = std::chrono::steady_clock::now();

    // Default to the official input file name unless overridden by command line
    std::string inputFile = (std::filesystem::current_path() / "tests/sample_orders_6.csv").string();
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
    ExecutionReportCSVWriter executionWriter("execution_rep.csv");
    RejectedExecutionReportCSVWriter rejectedWriter("rejected_execution_rep.csv");

    int processedCount = 0;
    int rejectCount = 0;
    int validOrderCount = 0;
    std::vector<ExecutionReport> matchReports;

    // Process the chronological timeline
    for (const auto& ev : timeline)
    {
        if (std::optional<RejectedExecutionReport> rejectedReport = buildRejectedReport(ev);
            rejectedReport.has_value())
        {
            rejectedWriter.write(*rejectedReport);
            rejectCount++;
        }
        else
        {
            const Order& order = ev.validOrder;

            // The order is 100% valid. Send it to the order book.
            matchReports.clear();
            engine.matchOrder(order, matchReports);

            for (const auto& rep : matchReports)
            {
                executionWriter.write(rep);
            }
            validOrderCount++;
        }
        processedCount++;
    }

    std::cout << "Done.\n";
    std::cout << "Total rows processed : " << processedCount << "\n";
    std::cout << "Total rejected rows  : " << rejectCount << "\n";
    std::cout << "Total valid orders   : " << validOrderCount << "\n";
    std::cout << "Successful output    : execution_rep.csv\n";
    std::cout << "Rejected output      : rejected_execution_rep.csv\n";
    const auto endTime = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime);
    std::cout << "Elapsed time         : " << elapsedMs.count() << " ms\n";

    return 0;
}
