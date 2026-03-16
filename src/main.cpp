#include <iostream>
#include "CSVReader.h"

int main() {
    CSVReader reader;
    
    // 1. Capture the result
    CSVReadResult result = reader.readOrders("tests/sample_orders_7.csv");

    // 2. Print Valid Orders
    std::cout << "========== VALID ORDERS (" << result.validOrders.size() << ") ==========\n\n";
    for (const auto& order : result.validOrders) {
        std::cout << "Seq No:     " << order.seqNo << "\n"
                  << "Client ID:  " << order.clientOrderID << "\n"
                  << "Instrument: " << order.instrument << "\n"
                  << "Side:       " << order.side << "\n"
                  << "Quantity:   " << order.quantity << "\n"
                  << "Price:      " << order.price << "\n"
                  << "Timestamp:  " << (order.timestamp.empty() ? "N/A" : order.timestamp) << "\n"
                  << "----------------------------------------\n\n";
    }

    // 3. Print Rejected Orders
    std::cout << "========== REJECTED ORDERS (" << result.parseRejects.size() << ") ==========\n\n";
    for (const auto& reject : result.parseRejects) {
        std::cout << "Seq Num:    " << reject.seqNum << "\n"
                  << "Client ID:  " << reject.clientOrderId << "\n"
                  << "Instrument: " << reject.instrument << "\n"
                  << "Side text:  " << reject.sideText << "\n"
                  << "Qty text:   " << reject.quantityText << "\n"
                  << "Price text: " << reject.priceText << "\n"
                  << "Reason:     " << reject.reason << "\n"
                  << "----------------------------------------\n\n";
    }

    return 0;
}