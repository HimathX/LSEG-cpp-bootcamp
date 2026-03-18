#include "OrderBook.h"
#include <stdexcept>

// Constructor: Initializes the order book for a specific instrument
OrderBook::OrderBook(const std::string& instrumentName) 
    : instrument_(instrumentName) 
{
}

// calls this to rest a passive order in the book
void OrderBook::addOrder(const Order& order) 
{
    if (order.side == 1) 
    {
        // BUY side: Map automatically sorts descending (highest price first)
        // Queue automatically maintains time priority (First In, First Out)
        buySide_[order.price].push(order);
    } 
    else if (order.side == 2) 
    {
        // SELL side: Map automatically sorts ascending (lowest price first)
        sellSide_[order.price].push(order);
    }
}

// Check if there are any resting buy orders
bool OrderBook::hasBuyOrders() const 
{
    return !buySide_.empty();
}

// Check if there are any resting sell orders
bool OrderBook::hasSellOrders() const 
{
    return !sellSide_.empty();
}

// Gets the queue of orders at the highest available Buy price
std::queue<Order>& OrderBook::getBestBuyQueue() 
{
    if (buySide_.empty()) {
        throw std::runtime_error("Attempted to access empty Buy book");
    }
    return buySide_.begin()->second; // .begin() is guaranteed to be the highest price
}

// Gets the queue of orders at the lowest available Sell price
std::queue<Order>& OrderBook::getBestSellQueue() 
{
    if (sellSide_.empty()) {
        throw std::runtime_error("Attempted to access empty Sell book");
    }
    return sellSide_.begin()->second; // .begin() is guaranteed to be the lowest price
}

// MUST call this after fully filling an order to clean up the book
void OrderBook::removeEmptyPriceLevels() 
{
    // Clean up the top of the Buy book if the queue is empty
    if (!buySide_.empty() && buySide_.begin()->second.empty()) 
    {
        buySide_.erase(buySide_.begin());
    }

    // Clean up the top of the Sell book if the queue is empty
    if (!sellSide_.empty() && sellSide_.begin()->second.empty()) 
    {
        sellSide_.erase(sellSide_.begin());
    }
}