#include <iostream>
#include <stdexcept>

int divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero!");
    }
    return a / b;
}

int main() {
    // Example 1: Catching an exception
    std::cout << "=== Example 1: Division ===" << std::endl;
    try {
        int result = divide(10, 2);
        std::cout << "10 / 2 = " << result << std::endl;
        
        result = divide(10, 0);  // This will throw
        std::cout << "This won't print" << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    // Example 2: Multiple catch blocks
    std::cout << "\n=== Example 2: Multiple Types ===" << std::endl;
    try {
        throw 42;  // Throwing an integer
    }
    catch (const std::string& e) {
        std::cout << "Caught string: " << e << std::endl;
    }
    catch (int e) {
        std::cout << "Caught int: " << e << std::endl;
    }
    catch (...) {
        std::cout << "Caught unknown exception" << std::endl;
    }
    
    // Example 3: Program continues after catch
    std::cout << "\n=== Example 3: Continuing After Catch ===" << std::endl;
    try {
        throw std::logic_error("Something went wrong");
    }
    catch (const std::exception& e) {
        std::cout << "Error handled: " << e.what() << std::endl;
    }
    
    std::cout << "Program continues normally after exception!" << std::endl;
    
    return 0;
}
