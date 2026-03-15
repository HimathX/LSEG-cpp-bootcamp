#pragma once
#include <string>

struct Order {
    std::string clientOrderID;
    std::string instrument;
    int side; // 1 for Buy, 2 for Sell
    double price;
    int quantity;
    std::string timestamp; // YYYYMMDD-HHMMSS.sss
    int seqNo; // Priority order based on time
};
