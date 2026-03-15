#include "OrderIDGenerator.h"
#include <atomic>

static std::atomic<int> counter{1};

std::string OrderIDGenerator::getNext() {
    return "ord" + std::to_string(counter.fetch_add(1));
}
