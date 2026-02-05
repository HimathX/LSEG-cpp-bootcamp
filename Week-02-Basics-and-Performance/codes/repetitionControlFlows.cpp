#include <iostream>

int main() {

    // Divide the numerator by each divisor in the array - skip 0 and stop when -1 is encountered
    int divisors[10] = {2, 0, 5, 0, 4, -1, 3, 1, 2, 0};
    const int size = 10;

    int numerator = 20;

    std::cout << "=== Using for loop ===\n";
    for (int i = 0; i < size; ++i) {

        if (divisors[i] == 0) {
            // Skip dividing by zero
            std::cout << "Skip: divisor is 0\n";
            continue;
        }

        if (divisors[i] == -1) {
            // Stop further processing
            std::cout << "Stop: sentinel -1 encountered\n";
            break;
        }

        std::cout << numerator << " / " << divisors[i]
                  << " = " << (numerator / divisors[i]) << '\n';
    }

    std::cout << "\n=== Using while loop ===\n";
    int j = 0;
    while (j < size) {

        if (divisors[j] == 0) {
            std::cout << "Skip: divisor is 0\n";
            ++j;               // advance before continue in while-loop
            continue;
        }
        if (divisors[j] == -1) {
            std::cout << "Stop: sentinel -1 encountered\n";
            break;
        }
        std::cout << numerator << " / " << divisors[j]
                  << " = " << (numerator / divisors[j]) << '\n';

        ++j;
    }

    return 0;
}

// g++ -o repetitionControlFlows repetitionControlFlows.cpp && ./repetitionControlFlows