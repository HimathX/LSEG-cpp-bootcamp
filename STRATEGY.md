# Flower Exchange — Build Strategy

## The One Rule to Remember

Every decision you make should be: **correct first, fast second**. The evaluators will run your binary against test data. A wrong answer scores zero regardless of speed.

---

## Data Structures

### Order Book (one per instrument)

```cpp
struct OrderEntry {
    std::string orderId;
    std::string clientOrderId;
    int         quantity;
    double      price;
    uint64_t    seqNum;   // arrival order for time priority
};

// Buy side: highest price first → use std::greater
std::map<double, std::deque<OrderEntry>, std::greater<double>> buySide;

// Sell side: lowest price first → use default std::less
std::map<double, std::deque<OrderEntry>, std::less<double>> sellSide;
```

`std::map` gives you price priority for free. `std::deque` at each price level gives you time priority for free (push_back to add, pop_front to consume).

### Price Precision

Store prices as `int` internally by multiplying by 100.

```cpp
// "45.50" → 4550 as int
// Avoids all floating-point comparison bugs
int priceToTick(double p) { return (int)std::round(p * 100); }
```

Convert back to `"45.50"` only when writing the CSV output.

### Execution Report

```cpp
struct ExecReport {
    std::string orderId;
    std::string clientOrderId;
    std::string instrument;
    int         side;       // 1=Buy, 2=Sell
    int         status;     // 0=New, 1=Rejected, 2=Fill, 3=PFill
    int         quantity;
    double      price;
    std::string reason;     // only for rejections
    std::string transactTime; // "YYYYMMDD-HHMMSS.sss"
};
```

---

## The Matching Algorithm

This is the entire engine in pseudocode. Get this right and you pass.

```
function processOrder(order):

    1. VALIDATE the order (see validation section)
       → if invalid: emit Rejected report, stop

    2. SELECT the correct order book by instrument

    3. DETERMINE if order is aggressive:
       - Incoming BUY  is aggressive if buySide price >= best ask price
       - Incoming SELL is aggressive if sellSide price <= best bid price

    4. MATCH loop:
       while remainingQty > 0 AND opposite side is not empty:
           get best opposite order (front of best price level's deque)
           
           if prices do NOT cross: break

           tradeQty = min(remainingQty, passiveOrder.quantity)
           execPrice = passiveOrder.price   ← THE KEY RULE (Example 5)

           emit report for AGGRESSIVE order:
               status = Fill if remainingQty == tradeQty else PFill
               price  = execPrice

           emit report for PASSIVE order:
               status = Fill if passive.quantity == tradeQty else PFill
               price  = execPrice

           remainingQty       -= tradeQty
           passiveOrder.qty   -= tradeQty

           if passiveOrder.qty == 0: remove from book

    5. If remainingQty > 0 after matching:
       add remainder to own side of book
       emit New report for the resting quantity

```

### The Example 5 "Twist" — Do Not Miss This

When a sell order comes in at price 1.00 and the book has buy orders at 55.00 and 65.00, the execution price is **the buyer's price (55.00 or 65.00), not 1.00**.

The rule: **execution price = passive (resting) order's price, always.**

Your aggressive order's submitted price only determines *whether* a match occurs, not *at what price*.

---

## Execution Report Ordering

Reports must be emitted in this sequence during a match:

1. Aggressive order's report (PFill or Fill)
2. Passive order's report (PFill or Fill)

If the aggressive order matches multiple passive orders, repeat steps 1–2 for each fill before moving to step 5 (New for any remainder).

---

## Validation Rules

Reject the order (status = 1) with a reason string if:

| Condition | Reason string |
|-----------|---------------|
| Missing any field | `"Invalid fields"` |
| Instrument not in {Rose, Lavender, Lotus, Tulip, Orchid} | `"Invalid instrument"` |
| Side not 1 or 2 | `"Invalid side"` |
| Price <= 0 | `"Invalid price"` |
| Quantity not multiple of 10 | `"Invalid size"` |
| Quantity < 10 or > 1000 | `"Invalid size"` |

A rejected order does **not** enter the order book.

---

## I/O

### Reading orders.csv

```cpp
std::ifstream inFile("orders.csv");
std::string line;
std::getline(inFile, line); // skip header

while (std::getline(inFile, line)) {
    // split by comma → clientOrderId, instrument, side, quantity, price
    // parse each field, validate, process
}
```

### Writing execution_rep.csv

**Buffer all reports in a vector, write once at the end.**

```cpp
std::vector<ExecReport> allReports;
// ... process all orders, push_back to allReports ...

std::ofstream out("execution_rep.csv");
out << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time\n";
for (auto& r : allReports) {
    out << r.orderId << "," << r.clientOrderId << "," << ...  << "\n";
}
```

This is meaningfully faster than flushing per-line and takes 5 minutes to implement.

### Timestamp Format

```cpp
#include <chrono>
#include <iomanip>
#include <sstream>

std::string now() {
    auto tp  = std::chrono::system_clock::now();
    auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(
                   tp.time_since_epoch()) % 1000;
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm   = std::gmtime(&t);

    std::ostringstream ss;
    ss << std::put_time(tm, "%Y%m%d-%H%M%S")
       << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}
```

Output format: `20260315-143022.456`

---

## Order ID Generation

The system assigns its own Order IDs sequentially.

```cpp
int orderCounter = 1;
std::string nextOrderId() {
    return "ord" + std::to_string(orderCounter++);
}
```

---

## File Structure

Keep it simple. Two people, clean split:

```
flower_exchange/
├── main.cpp          → entry point, ties everything together
├── OrderBook.h/.cpp  → matching logic (Person B)
├── CsvParser.h/.cpp  → read orders.csv, validate (Person A)
├── CsvWriter.h/.cpp  → write execution_rep.csv (Person A)
└── Types.h           → shared structs (both agree on this first)
```

---

## Test Plan

Test against the 6 sample files in this order of difficulty:

| Sample | Tests |
|--------|-------|
| Example 1 | Single passive order → New |
| Example 2 | Two passives, one non-matching buy |
| Example 3 | Full fill (FILL) |
| Example 4 | Partial fill (PFILL) |
| **Example 5** | **The twist — exec price from order book** |
| Example 6 | Multi-fill across price levels |
| Example 7 | All rejection cases |

Pass Example 5 and 7 and you've almost certainly covered all the edge cases.

---

## What to Ignore

- Memory-mapped files
- Custom allocators / object pools  
- Bitsets, intrusive linked lists
- SIMD / AVX instructions
- CPU affinity / thread pinning

None of this is needed. The bottleneck at workshop scale is your matching logic correctness, not memory bandwidth.