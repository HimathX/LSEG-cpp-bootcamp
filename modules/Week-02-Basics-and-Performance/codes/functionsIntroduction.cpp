#include <iostream>

// Function declaration (prototype)
int add(int a, int b);

int main() {
    int result = add(10, 20);
    std::cout << "Result = " << result << std::endl;
    return 0;
}

// Function definition
int add(int a, int b) {
    return a + b;
}

// g++ -o functionsIntroduction functionsIntroduction.cpp && ./functionsIntroduction