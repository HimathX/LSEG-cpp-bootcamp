#pragma once

#include <string>
#include <map>
#include <deque>
#include "Order.h"

class OrderBook
{
public:
    // Constructor requires the instrument name (e.g., "Rose")
    explicit OrderBook(const std::string& instrumentName);

    // Add a new resting passive order to the book
    void addOrder(const Order& order);

    // Check if there are orders available on a specific side
    bool hasBuyOrders() const;
    bool hasSellOrders() const;

    // Retrieve the deque of resting orders at the absolute best available price
    std::deque<RestingOrder>& getBestBuyQueue();
    std::deque<RestingOrder>& getBestSellQueue();

    // Clean up empty price levels after orders are fully filled
    void removeEmptyPriceLevels();

private:
    std::string instrument_;

    // BUY SIDE: std::greater<PriceTick> sorts from highest price to lowest price.
    // The highest buy price is the most aggressive, so it sits at the top (.begin())
    std::map<PriceTick, std::deque<RestingOrder>, std::greater<PriceTick>> buySide_;

    // SELL SIDE: std::less<PriceTick> sorts from lowest price to highest price.
    // The lowest sell price is the most aggressive, so it sits at the top (.begin())
    std::map<PriceTick, std::deque<RestingOrder>, std::less<PriceTick>> sellSide_;
};