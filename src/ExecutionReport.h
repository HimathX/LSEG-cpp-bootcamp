#pragma once
#include <optional>
#include <string>

struct ExecutionReport
{
    std::string orderID;
    std::string clientOrderID;
    std::string instrument;
    int side = 0; // 1 for Buy, 2 for Sell
    double price = 0.0;
    int quantity = 0;
    int status = 0; // 0=New, 1=Rejected, 2=Fill, 3=PFill
    std::string reason;
    std::string transactionTime; // YYYYMMDD-HHMMSS.sss

    std::optional<std::string> sideText;
    std::optional<std::string> quantityText;
    std::optional<std::string> priceText;
};
