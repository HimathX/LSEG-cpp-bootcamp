#include "Validator.h"
#include <unordered_set>

static const std::unordered_set<std::string> VALID_INSTRUMENTS = {
    "Rose", "Lavender", "Lotus", "Tulip", "Orchid"};

std::string Validator::validate(const Order &order)
{
    if (VALID_INSTRUMENTS.find(order.instrument) == VALID_INSTRUMENTS.end())
    {
        return "Invalid instrument";
    }
    if (order.side != 1 && order.side != 2)
    {
        return "Invalid side";
    }
    if (order.price <= 0.0)
    {
        return "Invalid price";
    }
    if (order.quantity < 10 || order.quantity > 1000 || order.quantity % 10 != 0)
    {
        return "Invalid size";
    }
    return "";
}
