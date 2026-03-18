#pragma once // Ensures the header file is included only once during compilation

#include <string>
#include <vector>
#include <cstdint> // Provides fixed-width integer types to ensure consistent size and ehaviour across dfferent platforms

#include "Order.h" // Include the Order struct definition, which is used to represent individual orders in the CSV file

struct ParseReject
{
    std::string clientOrderId;
    std::string instrument;
    std::string sideText;
    std::string quantityText;
    std::string priceText;
    std::string reason;
    std::uint64_t seqNum = 0;
};

struct CSVReadResult
{
    std::vector<Order> validOrders;
    std::vector<ParseReject> parseRejects;
};

class CSVReader
{
public:
    CSVReader() = default; // Default constructor for the CSVReader class

    CSVReadResult readOrders(const std::string &fileName); // Method to read the CSV file and return a vector of Order objects

private:
    std::vector<std::string> splitLine(const std::string &line); // Split one CSV rwo into fields

    Order parseOrder(const std::vector<std::string> &fields, std::uint64_t seqNum); // Convert a vector of strings into an Order object
};

