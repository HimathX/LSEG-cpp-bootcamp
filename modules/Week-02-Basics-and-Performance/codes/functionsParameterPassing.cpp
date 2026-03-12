#include <iostream>

// -------------------------------
// Pass by VALUE
void incrementByValue(int x)
{
    x = x + 1; // modifies only the local copy
}

// -------------------------------
// Pass by REFERENCE
void incrementByReference(int &x)
{
    x = x + 1; // modifies the original variable
}

int main()
{
    int number = 10;

    std::cout << "Initial value: " << number << std::endl;
    // Output: Initial value: 10

    incrementByValue(number);
    std::cout << "After pass by value: " << number << std::endl;
    // Output: After pass by value: 10

    incrementByReference(number);
    std::cout << "After pass by reference: " << number << std::endl;
    // Output: After pass by reference: 11

    return 0;
}

// g++ -o functionsParameterPassing functionsParameterPassing.cpp && ./functionsParameterPassing