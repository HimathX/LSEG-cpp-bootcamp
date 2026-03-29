#pragma once
#include <string>

// Enum for the 5 valid instruments (cheaper than string comparisons)
enum class InstrumentId : int {
    Rose = 0,
    Lavender = 1,
    Lotus = 2,
    Tulip = 3,
    Orchid = 4
};

// Price as integer cents (e.g., 55.50 -> 5550)
using PriceTick = int;

// Helper to convert instrument string to enum (returns -1 if invalid)
inline InstrumentId stringToInstrumentId(const std::string& instrument) {
    if (instrument == "Rose") return InstrumentId::Rose;
    if (instrument == "Lavender") return InstrumentId::Lavender;
    if (instrument == "Lotus") return InstrumentId::Lotus;
    if (instrument == "Tulip") return InstrumentId::Tulip;
    if (instrument == "Orchid") return InstrumentId::Orchid;
    return static_cast<InstrumentId>(-1); // Invalid
}

// Helper to convert enum to string
inline std::string instrumentIdToString(InstrumentId id) {
    switch (id) {
        case InstrumentId::Rose: return "Rose";
        case InstrumentId::Lavender: return "Lavender";
        case InstrumentId::Lotus: return "Lotus";
        case InstrumentId::Tulip: return "Tulip";
        case InstrumentId::Orchid: return "Orchid";
        default: return "Unknown";
    }
}

// Helper to convert price double to PriceTick (cents)
inline PriceTick priceToPriceTick(double price) {
    return static_cast<PriceTick>(price * 100.0);
}

// Helper to convert PriceTick back to price double
inline double priceTickToPrice(PriceTick tick) {
    return static_cast<double>(tick) / 100.0;
}

// Minimal resting order in the order book (saves memory)
struct RestingOrder {
    std::string orderID;
    std::string clientOrderID;
    int quantity;
    PriceTick priceTick;  // Store price as integer cents for matching
};

struct Order {
    std::string orderID;
    std::string clientOrderID;
    std::string instrument;
    InstrumentId instrumentId;  // Cached for fast comparisons
    int side; // 1 for Buy, 2 for Sell
    double price;
    PriceTick priceTick;  // Cached for order book keys
    int quantity;
    int seqNo; // Priority order based on time

    bool hasExtraFields = false; //Added to track if the order has more fields than expected, which is a validation failure in Phase 2
};
