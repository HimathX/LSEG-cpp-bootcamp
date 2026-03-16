#include "CSVReader.h"
#include <fstream>
#include <stdexcept>

std::vector<std::string> CSVReader::splitLine(const std::string& line) 
{
    std::vector<std::string> fields;
    std::string current;

    for (char ch : line) {
        if (ch == ',') {
            fields.push_back(current); // Add the current field to the vector when encountering a comma
            current.clear(); // Clear the current field for the next value
        } else {
            current += ch; // Append the character to the current field
        }
    }
    fields.push_back(current); // Add the last field after the loop
    return fields;
}

bool CSVReader::isMalformedRow(const std::vector<std::string>& fields) const 
{
    if (fields.size() != 5) {
        return true; // A valid row must have exactly 5 fields
    }
    for (const auto& field : fields) {
        if (field.empty()) {
            return true; // A valid row cannot have empty fields
        }
    }
    return false; // The row is well-formed
}

Order CSVReader::pasrseOrder(const std::vector<std::string>& fields, std::uint64_t seqNum) 
{
    Order order;

    order.clientOrderID = fields[0];
    order.instrument    = fields[1];
    order.side          = std::stoi(fields[2]);
    order.quantity      = std::stoi(fields[3]);
    order.price         = std::stod(fields[4]);
    order.seqNo         = seqNum;

    return order;
}

//Main Reading Function
CSVReadResult CSVReader::readOrders(const std::string& fileName)
{
    CSVReadResult result;

    std::ifstream inFile(fileName);
     if (!inFile.is_open()) {
        throw std::runtime_error("Could not open file: " + fileName); // Throw an exception if the file cannot be opened
    }

    std::string line;
    std::uint64_t seqNum = 1; // Initialize sequence number for orders

    // Skip header
    std::getline(inFile, line);
   

    while (std::getline(inFile, line)) {
        auto fields = splitLine(line); // Split the line into fields

        if (isMalformedRow(fields)) {
            ParseReject reject;
            reject.clientOrderId = fields.size() > 0 ? fields[0] : "";
            reject.instrument = fields.size() > 1 ? fields[1] : "";
            reject.sideText = fields.size() > 2 ? fields[2] : "";
            reject.quantityText = fields.size() > 3 ? fields[3] : "";
            reject.priceText = fields.size() > 4 ? fields[4] : "";
            reject.seqNum = seqNum++;
            result.parseRejects.push_back(reject); // Add to parse rejects if the row is malformed
        } else {
            try {
                Order order = pasrseOrder(fields, seqNum++); // Parse the order and add to valid orders
                result.validOrders.push_back(order);
            } catch (const std::exception& e) {
                ParseReject reject;
                reject.clientOrderId = fields[0];
                reject.instrument = fields[1];
                reject.sideText = fields[2];
                reject.quantityText = fields[3];
                reject.priceText = fields[4];
                reject.reason = e.what(); // Capture the exception message as the reason for rejection
                reject.seqNum = seqNum++;
                result.parseRejects.push_back(reject); // Add to parse rejects if parsing fails
            }
        }
    }

    return result; // Return the result containing valid orders and parse rejects

}