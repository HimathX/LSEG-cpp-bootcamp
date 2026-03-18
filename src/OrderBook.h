#pragma once

#include <string>
#include <map>
#include <queue>
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

    // Retrieve the queue of orders at the absolute best available price
    std::queue<Order>& getBestBuyQueue();
    std::queue<Order>& getBestSellQueue();

    // Clean up empty price levels after orders are fully filled
    void removeEmptyPriceLevels();

private:
    std::string instrument_;

    // BUY SIDE: std::greater<double> sorts from highest price to lowest price.
    // The highest buy price is the most aggressive, so it sits at the top (.begin())
    std::map<double, std::queue<Order>, std::greater<double>> buySide_;

    // SELL SIDE: std::less<double> sorts from lowest price to highest price.
    // The lowest sell price is the most aggressive, so it sits at the top (.begin())
    std::map<double, std::queue<Order>, std::less<double>> sellSide_;
};