#include "CSVReader.h"
#include <cctype>
#include <fstream>
#include <stdexcept>

// Helper functions for parsing and validation
namespace
{
    constexpr std::size_t EXPECTED_FIELD_COUNT = 5;

    std::string trim(const std::string &value)
    {
        std::size_t start = 0;
        while (start < value.size() &&
               std::isspace(static_cast<unsigned char>(value[start])))
        {
            ++start;
        }

        std::size_t end = value.size();
        while (end > start &&
               std::isspace(static_cast<unsigned char>(value[end - 1])))
        {
            --end;
        }

        return value.substr(start, end - start);
    }

    // Parsing functions to ensure that the entire string is a valid integer or double, without any extra characters
    bool parseIntStrict(const std::string &text, int &value)
    {
        std::size_t parsedLength = 0;

        try
        {
            value = std::stoi(text, &parsedLength);
        }
        catch (const std::exception &)
        {
            return false;
        }

        return parsedLength == text.size();
    }

    bool parseDoubleStrict(const std::string &text, double &value)
    {
        std::size_t parsedLength = 0;

        try
        {
            value = std::stod(text, &parsedLength);
        }
        catch (const std::exception &)
        {
            return false;
        }

        return parsedLength == text.size();
    }

    std::vector<std::string> normalizeFields(const std::vector<std::string> &fields)
    {
        std::vector<std::string> normalized(EXPECTED_FIELD_COUNT);

        for (std::size_t i = 0; i < fields.size() && i < EXPECTED_FIELD_COUNT; ++i)
        {
            normalized[i] = fields[i];
        }

        return normalized;
    }

    std::vector<std::string> trimFieldsForValidation(const std::vector<std::string> &fields)
    {
        std::vector<std::string> trimmed = fields;

        for (std::size_t i = 0; i < trimmed.size(); ++i)
        {
            trimmed[i] = trim(trimmed[i]);
        }

        return trimmed;
    }

    std::string findRejectReason(const std::vector<std::string> &rawFields,
                                 const std::vector<std::string> &trimmedFields)
                                 
    {
        //Cascading validation checks for each field, returning the first encountered error reason
        if (trimmedFields[0].empty())
        {
            return "Invalid client order id";
        }
        if (trimmedFields[1].empty())
        {
            return "Invalid instrument";
        }
        if (trimmedFields[2].empty())
        {
            return "Invalid side";
        }
        if (trimmedFields[3].empty())
        {
            return "Invalid size";
        }
        if (trimmedFields[4].empty())
        {
            return "Invalid price";
        }

        int side = 0;
        if (!parseIntStrict(trimmedFields[2], side))
        {
            return "Invalid side";
        }

        int quantity = 0;
        if (!parseIntStrict(trimmedFields[3], quantity))
        {
            return "Invalid size";
        }

        double price = 0.0;
        if (!parseDoubleStrict(trimmedFields[4], price))
        {
            return "Invalid price";
        }

        return "";
    }
} // namespace

std::vector<std::string> CSVReader::splitLine(const std::string &line)
{
    std::vector<std::string> fields;
    std::string current;

    for (char ch : line)
    {
        if (ch == ',')
        {
            fields.push_back(current); // Add the current field to the vector when encountering a comma
            current.clear();           // Clear the current field for the next value
        }
        else
        {
            current += ch; // Append the character to the current field
        }
    }
    fields.push_back(current); // Add the last field after the loop
    return fields;
}

Order CSVReader::parseOrder(const std::vector<std::string> &fields, std::uint64_t seqNum)
{
    Order order;
    int side = 0;
    int quantity = 0;
    double price = 0.0;

    parseIntStrict(fields[2], side);
    parseIntStrict(fields[3], quantity);
    parseDoubleStrict(fields[4], price);

    order.clientOrderID = fields[0];
    order.instrument = fields[1];
    order.instrumentId = stringToInstrumentId(fields[1]);  // Cache enum for fast comparisons
    order.side = side;
    order.quantity = quantity;
    order.price = price;
    order.priceTick = priceToPriceTick(price);  // Cache as integer cents
    order.seqNo = static_cast<int>(seqNum);

    return order;
}

// Main Reading Function
CSVReadResult CSVReader::readOrders(const std::string &fileName)
{
    CSVReadResult result;

    std::ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        throw std::runtime_error("Could not open file: " + fileName); // Throw an exception if the file cannot be opened
    }

    std::string line;
    std::uint64_t seqNum = 1; // Initialize sequence number for orders

    // Skip header
    std::getline(inFile, line);

    while (std::getline(inFile, line))
    {
        auto fields = splitLine(line); // Split the line into fields
        auto firstFiveRawFields = normalizeFields(fields);
        auto firstFiveTrimmedFields = trimFieldsForValidation(firstFiveRawFields);
        std::string rejectReason = findRejectReason(firstFiveRawFields, firstFiveTrimmedFields);

        if (!rejectReason.empty())
        {
            ParseReject reject;
            reject.clientOrderId = firstFiveRawFields[0];
            reject.instrument = firstFiveRawFields[1];
            reject.sideText = firstFiveRawFields[2];
            reject.quantityText = firstFiveRawFields[3];
            reject.priceText = firstFiveRawFields[4];
            reject.reason = rejectReason;
            reject.seqNum = seqNum++;
            result.parseRejects.push_back(reject); // Add to parse rejects if the row is malformed
        }
        else
        {
            Order order = parseOrder(firstFiveTrimmedFields, seqNum++); // Parse the order and add to valid orders
            order.hasExtraFields = fields.size() > EXPECTED_FIELD_COUNT;
            result.validOrders.push_back(order);
        }
    }

    return result; // Return the result containing valid orders and parse rejects
}
