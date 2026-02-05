#include <iostream> // Preprocessor directive: Tells the compiler to include the standard input/output library
// This is required for using std::cout

int main()
{ // The main function is the entry point of every C++ program
    // Program execution starts here

    std::cout << "Hello, World!\n"; // std::cout is used to print output to the console
    // The << operator sends data to the output stream

    return 0; // Ends the program - Returning 0 indicates successful execution
}

// g++ -o main main.cpp && ./main
