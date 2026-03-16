#pragma once
#include <string>
#include "Order.h"

class Validator
{
public:
    // Returns "" if valid, or the exact rejection reason string if not.
    static std::string validate(const Order &order);
};
