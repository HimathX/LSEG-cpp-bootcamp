# 🌸 Flower Exchange — Project Plan
> C++ Workshop Series | LSEG | Completion: 27 March 2026 | Demo: 8 April 2026

---

## 1. Project Overview

Build a C++ **Exchange Application** that reads orders from `orders.csv` and produces an `execution_rep.csv`. The system must validate orders, maintain per-instrument order books, and execute trades based on price and time priority.

A **Web-based GUI** (HTML/JS) will also be built to allow customers to submit orders and view execution reports visually.

| Item | Detail |
|---|---|
| **Group Size** | 2 Developers |
| **Duration** | 15 Days |
| **Submission** | 27 March 2026 |
| **Demo** | 8 April 2026 @ LSEG Malabe |
| **Language** | C++ (Backend), HTML/JS (Frontend GUI) |
| **Instruments** | Rose, Lavender, Lotus, Tulip, Orchid |

---

## 2. Project Scope
- **Core Functionality:**
  - Read and parse `orders.csv`
  - Validate orders against specified rules
  - Maintain separate order books for each instrument
  - Match orders based on price-time priority
  - Write execution reports to `execution_rep.csv`

---

## 3. Architecture

```
orders.csv
    │
    ▼
[ CSV Reader ]
    │
    ▼
[ Validator ] ──── FAIL ──▶ [ Rejected Execution Report ]
    │
   PASS
    │
    ▼
[ Order Router ] ── routes to instrument ──▶ [ Order Book x5 ]
                                                    │
                                            [ Matching Engine ]
                                                    │
                                                    ▼
                                          [ Execution Report Writer ]
                                                    │
                                                    ▼
                                            execution_rep.csv

[ Web GUI ] ◀──────────────────────────── [ Backend API / File Bridge ]
```

---

## 4. Phases & Tasks

---

### Phase 1 — Setup & Architecture (Days 1–2)
> Both developers work together. Nothing else starts until this is locked.

- [ ] Create GitHub repo under `cpp-bootcamp/weekly-lectures/` structure
- [ ] Set up `CMakeLists.txt` with `-O2` optimization flag from day one
- [ ] Define and agree on core structs (this is the shared contract):
  - `Order` — ClientOrderID, Instrument, Side, Price, Quantity, timestamp, seqNo
  - `ExecutionReport` — OrderID, ClientOrderID, Instrument, Side, Price, Quantity, Status, Reason, TransactionTime
- [ ] Implement Order ID generator (`ord1`, `ord2`, ... auto-incrementing)
- [ ] Set up folder structure:
```
cpp-bootcamp/
└── weekly-lectures/
    └── flower-exchange/
        ├── src/
        │   ├── main.cpp
        │   ├── Order.h
        │   ├── ExecutionReport.h
        │   ├── OrderBook.h / .cpp
        │   ├── MatchingEngine.h / .cpp
        │   ├── CSVReader.h / .cpp
        │   ├── CSVWriter.h / .cpp
        │   └── Validator.h / .cpp
        ├── gui/
        │   ├── index.html
        │   ├── style.css
        │   └── app.js
        ├── tests/
        │   ├── sample_orders_1-6.csv
        │   └── expected_exec_rep_1-6.csv
        ├── CMakeLists.txt
        └── PROJECT_PLAN.md
```

---

### Phase 2 — CSV I/O & Validation (Days 3–5)
> Dev 1 leads CSV Reader. Dev 2 leads Validator + CSV Writer.

#### CSV Reader (Dev 1 leads)
- [ ] Read `orders.csv` line by line using buffered I/O (`std::ifstream` with buffer)
- [ ] Parse each row into an `Order` struct
- [ ] Handle malformed rows gracefully (missing columns, empty fields)
- [ ] Assign arrival sequence number for time priority

#### Validator (Dev 2 leads)
- [ ] Missing required field check
- [ ] Invalid instrument check (must be one of 5 types)
- [ ] Invalid side check (must be 1 or 2)
- [ ] Price > 0 check
- [ ] Quantity must be multiple of 10
- [ ] Quantity must be between 10 and 1000 (inclusive)
- [ ] On failure: immediately write `Rejected` execution report with reason string

#### CSV Writer (Dev 2 leads)
- [ ] Write execution reports to `execution_rep.csv`
- [ ] Implement timestamp in `YYYYMMDD-HHMMSS.sss` format using `<chrono>`
- [ ] Use buffered writes — do not flush after every line

#### ✅ Phase 2 Done When:
Running Example 7 (input validations) produces the exact expected `exec_rep.csv`

---

### Phase 3 — Order Book & Matching Engine (Days 6–10)
> This is the hardest phase. Both developers work on it together.

#### Order Book (Dev 1 leads)
- [ ] Create 5 separate `OrderBook` instances (one per instrument)
- [ ] Buy side: sorted by price descending (highest price = highest priority)
- [ ] Sell side: sorted by price ascending (lowest price = highest priority)
- [ ] Time priority: orders at same price sorted by arrival sequence number
- [ ] Recommended structure: `std::map<double, std::queue<Order>>` per side

#### Matching Engine (Dev 2 leads — especially PFill + Example 5)
- [ ] On new order arrival, check if it can match against opposite side
- [ ] **Passive order (no match):** Add to book, output status `0 - New`
- [ ] **Aggressive order — Full match (Fill):** Remove resting order from book, output `2 - Fill` for both sides
- [ ] **Aggressive order — Partial match (PFill):** Output `3 - PFill` for aggressive order, reduce resting order quantity, keep remainder in book with original time priority
- [ ] **Example 5 rule (CRITICAL):** Execution price of aggressive order = price of the resting passive order in the book, NOT the incoming order's price
- [ ] Handle chained partial fills: one aggressive order can match against multiple resting orders sequentially

#### ✅ Phase 3 Done When:
All of Examples 1–6 produce correct output

---

### Phase 4 — Testing (Days 11–12)
> Both developers test together.

- [ ] Run all 6 sample `orders.csv` files through the system
- [ ] Compare output `execution_rep.csv` against expected outputs
- [ ] Note: order of rows doesn't have to match, but content must match exactly
- [ ] Fix any bugs found — common traps:
  - Example 5 aggressive price not using resting order price
  - PFill residual losing its time priority
  - Timestamp format off by milliseconds
  - Order ID counter not incrementing correctly

---

### Phase 5 — Performance Optimization (Days 13–14)
> Both developers work together.

- [ ] Confirm `-O2` or `-O3` is set in `CMakeLists.txt`
- [ ] Switch from `std::string` parsing to `std::string_view` where possible
- [ ] Use buffered file reads — read large chunks, parse manually if needed
- [ ] Use buffered file writes — accumulate output, write in batches
- [ ] Avoid `std::endl` — use `'\n'` instead (endl flushes, which is slow)
- [ ] Profile with a large mock `orders.csv` (10,000+ rows) and measure runtime
- [ ] Consider representing price as integer internally (`price * 100`) to avoid floating point comparison issues

---

### Phase 6 — Web GUI (Days 8–15, Dev 2 only)
> Start only after Phase 3 core is working. Drop if behind schedule.

- [ ] **Day 8–9:** Order submission form
  - Fields: Client Order ID, Instrument (dropdown), Side (Buy/Sell), Quantity, Price
  - Basic client-side validation with error messages
- [ ] **Day 9–10:** Execution report display table
  - Show: Order ID, Client Order ID, Instrument, Side, Status, Quantity, Price, Timestamp
  - Color code rows: New (blue), Fill (green), PFill (yellow), Rejected (red)
- [ ] **Day 10–11:** Connect GUI to backend
  - Option A: GUI writes `orders.csv`, polls and reads `execution_rep.csv`
  - Option B: Lightweight REST bridge (simple HTTP server in C++ or Python)
- [ ] **Day 11:** Input validation feedback in UI
- [ ] **Day 14–15:** Polish, demo prep

---

## 5. Key Technical Decisions

| Decision | Choice | Reason |
|---|---|---|
| Order book structure | `std::map` + `std::queue` | Price-sorted automatically, queue preserves time priority |
| Price type | `double` internally, careful comparison | Multiply by 100 and cast to int if precision issues appear |
| File I/O | Buffered reads + batched writes | Performance requirement |
| Compilation | `-O2` from Day 1 | Free speed, no code changes needed |
| Timestamp | `std::chrono` + `strftime` | Millisecond precision required |
| GUI connection | File polling (simple) or REST bridge | File polling is faster to build |

---

## 6. Execution Status Reference

| Code | Status | When |
|---|---|---|
| `0` | New | Order enters book without matching |
| `1` | Rejected | Validation failure |
| `2` | Fill | Order fully matched and removed from book |
| `3` | PFill | Order partially matched, remainder stays in book |

---

## 7. Risks & Mitigations

| Risk | Severity | Mitigation |
|---|---|---|
| Example 5 aggressive price rule misunderstood | 🔴 High | Dev 2 owns this, implement and test it on Day 6 not Day 10 |
| PFill residual loses time priority | 🔴 High | Store original sequence number in Order struct, never re-assign |
| GUI eats into engine time | 🟡 Medium | Hard rule: no GUI before Day 8. Drop GUI polish if behind on Day 11 |
| Floating point price comparison bugs | 🟡 Medium | Use epsilon comparison or convert to integer internally |
| Merge conflicts between developers | 🟢 Low | Clear file ownership per feature, agree on interfaces Day 1 |
| Timestamp format wrong | 🟢 Low | Test timestamp output against expected on Day 3 |

---

## 8. Day-by-Day Timeline

| Days | Dev 1 | Dev 2 (You) |
|---|---|---|
| 1–2 | 🤝 Together: repo, structs, Order ID generator, folder structure | 🤝 Together |
| 3–5 | CSV Reader (buffered), malformed row handling | Validator (all 6 rules), CSV Writer, timestamp |
| 6–7 | Order Book class, buy/sell side sorting | Matching engine — New + Fill states |
| 8–9 | Matching engine — review + PFill support | PFill + **Example 5 twist** + GUI form (Day 8) |
| 10 | Integration: plug CSV reader into engine | GUI execution table + backend connection |
| 11–12 | 🤝 Together: test all 6 sample files, fix bugs | 🤝 Together + GUI validation feedback |
| 13–14 | 🤝 Performance pass: flags, buffered I/O, profiling | 🤝 Performance + GUI polish |
| 15 | 🤝 Buffer day: cleanup, demo prep, rehearse | 🤝 Buffer day |

---

## 9. Definition of Done

- [ ] All 6 sample `orders.csv` files produce correct `execution_rep.csv` output
- [ ] System handles all rejection cases from Example 7
- [ ] Example 5 aggressive price rule works correctly
- [ ] Performance tested on large dataset (10,000+ orders)
- [ ] Code is clean, well-commented, and ready for code review at demo
- [ ] GUI allows order submission and displays execution reports (bonus)
- [ ] Repo is clean with a proper `README.md`

---

*Last updated: March 2026 | Flower Exchange Group Project | LSEG C++ Workshop*