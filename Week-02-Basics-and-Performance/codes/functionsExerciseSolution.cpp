#include <iostream>

int factorial(int n, bool& success) {

    if (n < 0 || n > 12) {
        success = false;
        return 0;
    }

    int result = 1;

    for (int i = 1; i <= n; i++) {
        result *= i;
    }

    success = true;
    return result;
}

int main() {
    int number = 5;
    bool success = false;

    int result = factorial(number, success);

    if (success) {
        std::cout << "Factorial of " << number
                  << " is " << result << std::endl;
        // Output: Factorial of 5 is 120
    } else {
        std::cout << "Invalid input for factorial" << std::endl;
    }

    return 0;
}

// g++ -o functionsExerciseSolution functionsExerciseSolution.cpp && ./functionsExerciseSolution