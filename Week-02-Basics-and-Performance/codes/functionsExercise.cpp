#include <iostream>

/*
    EXERCISE: Factorial Function

    Your task is to create a function called factorial with the following rules:

    1. The function must calculate the factorial of a number.
    2. Factorial is defined only for non-negative integers.
       - 0! = 1
    3. If the input number is negative, the function must fail.
    4. If the input number is greater than 12, the function must fail
       (to avoid integer overflow when using int).
    5. The function must:
       - Return the factorial result
       - Use a reference parameter to indicate success or failure
    6. Do NOT print anything inside the function.
    7. Use a loop (do NOT use recursion).


*/

// --------------------------------------------------
// Add the Function declaration (prototype) here

// --------------------------------------------------
int main()
{
    int number = 5;
    bool success = false;

    int result = factorial(number, success);

    if (success)
    {
        std::cout << "Factorial of " << number
                  << " is " << result << std::endl;
        // Expected output: Factorial of 5 is 120
    }
    else
    {
        std::cout << "Invalid input for factorial" << std::endl;
    }

    return 0;
}

// --------------------------------------------------
// Add the Function definition here

// how to run : g++ -o functionsExercise functionsExercise.cpp && ./functionsExercise