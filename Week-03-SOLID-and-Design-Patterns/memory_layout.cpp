#include <iostream>
#include <cstddef>

// This struct demonstrates how the C++ abstract machine pads data members
// to align them on memory boundaries for efficient CPU access.
struct DataLayout {
    // char occupies 1 byte
    // Offset: 0
    char a;
    
    // Compiler inserts 3 bytes of padding here (bytes 1-3)
    // to align the next int on a 4-byte boundary
    // Offset: 4
    int b;
    
    // char occupies 1 byte
    // Offset: 8
    char c;
    
    // Compiler inserts 7 bytes of padding here (bytes 9-15)
    // Why 7 bytes? After char c at offset 8, the next member is double d.
    // double requires 8-byte alignment, so d must start at offset 16.
    // This requires 7 bytes of padding (9-15) to reach the next 8-byte boundary.
    // Alignment rule: each member must start at an offset divisible by its size.
    // to align the next double on an 8-byte boundary
    // Offset: 16
    double d;
    
    // char occupies 1 byte
    // Offset: 24
    char e;
    
    // Compiler inserts 7 bytes of padding at the end (bytes 25-31)
    // to make the struct size a multiple of its largest member alignment (8)
    // Offset: 32 (end of struct)
};

int main() {
    std::cout << "Structure Memory Layout Analysis:" << std::endl;
    std::cout << "=================================" << std::endl;
    
    // Display the total size of the struct
    std::cout << "Total size of DataLayout: " << sizeof(DataLayout) << " bytes" << std::endl;
    
    // Display offsets of each member
    DataLayout obj;
    std::cout << "\nMember offsets:" << std::endl;
    std::cout << "  a (char):   " << offsetof(DataLayout, a) << " bytes" << std::endl;
    std::cout << "  b (int):    " << offsetof(DataLayout, b) << " bytes" << std::endl;
    std::cout << "  c (char):   " << offsetof(DataLayout, c) << " bytes" << std::endl;
    std::cout << "  d (double): " << offsetof(DataLayout, d) << " bytes" << std::endl;
    std::cout << "  e (char):   " << offsetof(DataLayout, e) << " bytes" << std::endl;
    
    // Display individual member sizes
    std::cout << "\nMember sizes:" << std::endl;
    std::cout << "  sizeof(char):   " << sizeof(char) << " byte" << std::endl;
    std::cout << "  sizeof(int):    " << sizeof(int) << " bytes" << std::endl;
    std::cout << "  sizeof(double): " << sizeof(double) << " bytes" << std::endl;
    
    return 0;
}