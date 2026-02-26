#include <iostream>
#include <utility>

// Function overloads to demonstrate lvalue vs rvalue references
void process(int& x) {
    std::cout << "Lvalue reference: " << x << std::endl;
}

void process(int&& x) {
    std::cout << "Rvalue reference: " << x << std::endl;
}

int getValue() { return 42; }

int main() {
    int a = 10;
    
    // Lvalue examples - objects with names/addresses
    std::cout << "=== Lvalue (can use &) ===" << std::endl;
    process(a);           // a is an lvalue - binds to int&
    
    int& ref = a;         // Lvalue reference to lvalue - OK
    // int& ref2 = 5;     // ERROR: can't bind lvalue ref to rvalue
    
    // Rvalue examples - temporary objects
    std::cout << "\n=== Rvalue (can use &&) ===" << std::endl;
    process(20);          // 20 is an rvalue - binds to int&&
    process(getValue());  // function return is rvalue - binds to int&&
    process(a + 5);       // expression result is rvalue - binds to int&&
    
    int&& rref = 30;      // Rvalue reference to rvalue - OK
    // int&& rref2 = a;   // ERROR: can't bind rvalue ref to lvalue
    
    // std::move converts lvalue to rvalue
    std::cout << "\n=== std::move (lvalue -> rvalue) ===" << std::endl;
    process(std::move(a)); // Treats 'a' as rvalue - binds to int&&
    
    // Note: rvalue reference variable itself is an lvalue!
    std::cout << "\n=== Rvalue ref variable is lvalue ===" << std::endl;
    process(rref);        // rref is lvalue (has name) - binds to int&
    
    return 0;
}
