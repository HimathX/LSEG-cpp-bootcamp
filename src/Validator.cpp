#include "Validator.h"

std::string_view Validator::validate(const Order &order)
{
    if (order.instrumentId == static_cast<InstrumentId>(-1))
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
