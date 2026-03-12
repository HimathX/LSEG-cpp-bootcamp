#include <iostream>
#include <iomanip> // allows us to use std::setprecision

#define NEXT_YEAR    2027 //precprocessor directive to define a constant

int main() {

    // Declare and initialize variables
    int    year        = 2026;        // integer (whole number)
    float  temperature = 36.5;        // decimal, when lower precision is fine
    double pi          = 3.141592653; // decimal, when higher precision needed
    char   grade       = 'A';         // single character
    bool   isCppFun    = true;        // boolean

    // Print variables with labels
    std::cout << "year            = " << year << "\n";
    std::cout << "temperature     = " << temperature << "\n";
    std::cout << "pi              = " << std::setprecision(10) << pi << "\n"; 
    //by default std::cout will only print 6 significant digits. Using setprecison we can change that (need to include <iomanip>)
    std::cout << "grade           = " << grade << "\n";
    std::cout << "isCppFun        = " << isCppFun << "\n";
    std::cout << "Next year       = " << NEXT_YEAR << "\n"; //printing a constant defined using preprocessor directive

    return 0;
}

  //g++ -o datatypes datatypes.cpp && ./datatypes