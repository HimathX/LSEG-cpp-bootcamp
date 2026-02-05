#include <iostream>

int main()
{
    // STACK MEMORY (automatic)

    int stackInt = 10;             // single int on the stack
    int stackArray[3] = {1, 2, 3}; // array on the stack

    std::cout << stackInt << '\n';
    std::cout << stackArray[0] << '\n';

    // HEAP MEMORY (dynamic)

    // Single int on the heap
    int *heapInt = new int(20); // allocate int on heap
    std::cout << *heapInt << '\n';
    delete heapInt; // free heap memory

    // Array on the heap
    int *heapArray = new int[3]{4, 5, 6}; // allocate array on heap
    std::cout << heapArray[0] << '\n';
    delete[] heapArray; // free heap array

    return 0;
}

// g++ -o stackVsHeap stackVsHeap.cpp && ./stackVsHeap