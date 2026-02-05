#include <iostream>

int main()
{
    int a;  // integer variable declared (no value yet)
    a = 42; // value 42 assigned later

    int b = 42; // integer variable declared and assigned in one step

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';

    return 0;
}

// g++ -o variables variables.cpp && ./variables