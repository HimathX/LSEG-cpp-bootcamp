#pragma once
#include <string_view>
#include "Order.h"

class Validator
{
public:
    // Returns "" if valid, or the exact rejection reason string_view if not.
    // No allocation overhead since rejection texts are string literals.
    static std::string_view validate(const Order &order);
};
