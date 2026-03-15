#pragma once
#include <string>

struct ExecutionReport {
    std::string orderID;
    std::string clientOrderID;
    std::string instrument;
    int side; // 1 for Buy, 2 for Sell
    double price;
    int quantity;
    int status; // 0=New, 1=Rejected, 2=Fill, 3=PFill
    std::string reason;
    std::string transactionTime; // YYYYMMDD-HHMMSS.sss
};
