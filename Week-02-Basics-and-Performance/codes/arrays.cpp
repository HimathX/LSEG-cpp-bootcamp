#include <iostream>

int main() {

    // 1) Integer array
    int scores[4] = {85, 90, 78, 81};

    std::cout << "scores[0] = " << scores[0] << "\n"; //access the first element
    std::cout << "scores[3] = " << scores[3] << "\n"; //access the 4th element 

    // 2) Double array
    double temperatures[4] = {36.5, 37.0, 36.8, 36.6};

    std::cout << "temperatures[0] = " << temperatures[0] << "\n"; //access the first element
    std::cout << "temperatures[2] = " << temperatures[2] << "\n"; //access the 3rd element 

    // 3) Character array
    char grades[4] = {'A', 'B', 'C', 'D'};

    std::cout << "grades[0] = " << grades[0] << "\n"; //access the first element
    std::cout << "grades[1] = " << grades[1] << "\n"; //access the 2nd element 

    // 4) Boolean array
    bool attendance[4] = {true, false, true, true};

    std::cout << "attendance[0] = " << attendance[0] << "\n"; //access the first element
    std::cout << "attendance[3] = " << attendance[3] << "\n"; //access the 4th element 

    // std::cout << "attendance[4] = " << attendance[4] << "\n"; Out of bounds (there is no 5th element)
    // This will lead to undefined behaviour

    return 0;
}

// g++ -o arrays arrays.cpp && ./arrays