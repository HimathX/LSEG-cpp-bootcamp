#pragma once
#include <string>

struct Order {
    std::string orderID;
    std::string clientOrderID;
    std::string instrument;
    int side; // 1 for Buy, 2 for Sell
    double price;
    int quantity;
    std::string timestamp; // YYYYMMDD-HHMMSS.sss
    int seqNo; // Priority order based on time  

    bool hasExtraFields = false; //Added to track if the order has more fields than expected, which is a validation failure in Phase 2
};
