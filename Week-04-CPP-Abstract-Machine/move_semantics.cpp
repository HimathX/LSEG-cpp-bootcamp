#include <iostream>
#include <string>

/* MOVE SEMANTICS IN C++
 * 
 * Move semantics allows transferring ownership of resources from one object
 * to another, avoiding expensive copying operations.
 * 
 * Key concept: Instead of COPYING data, we MOVE it (transfer ownership)
 */

class SimpleString {
private:
    char* data;
    size_t size;

public:
    // Constructor
    SimpleString(const char* str) {
        size = strlen(str);
        data = new char[size + 1];
        strcpy(data, str);
        std::cout << "[Constructor] Created: \"" << data << "\"" << std::endl;
    }
    
    // Copy Constructor - makes a COPY (expensive)
    SimpleString(const SimpleString& other) {
        size = other.size;
        data = new char[size + 1];  // Allocate NEW memory
        strcpy(data, other.data);    // Copy the data
        std::cout << "[Copy Constructor] Copied: \"" << data << "\"" << std::endl;
    }
    
    // Move Constructor - TRANSFERS ownership (cheap)
    SimpleString(SimpleString&& other) noexcept {
        // "Steal" the data from other
        data = other.data;
        size = other.size;
        
        // Leave other in a valid but empty state
        other.data = nullptr;
        other.size = 0;
        
        std::cout << "[Move Constructor] Moved: \"" << data << "\"" << std::endl;
    }
    
    // Destructor
    ~SimpleString() {
        if (data) {
            std::cout << "[Destructor] Deleting: \"" << data << "\"" << std::endl;
            delete[] data;
        } else {
            std::cout << "[Destructor] Nothing to delete (already moved)" << std::endl;
        }
    }
    
    void print() {
        if (data) {
            std::cout << "String: \"" << data << "\"" << std::endl;
        } else {
            std::cout << "String: (empty - moved from)" << std::endl;
        }
    }
};

SimpleString createString() {
    SimpleString temp("Hello from function");
    return temp;  // Move happens here (RVO may optimize this away)
}

int main() {
    std::cout << "=== Example 1: Copy Constructor ===" << std::endl;
    SimpleString s1("Original");
    SimpleString s2 = s1;  // Copy constructor called
    s1.print();
    s2.print();
    
    std::cout << "\n=== Example 2: Move Constructor ===" << std::endl;
    SimpleString s3("Temporary");
    SimpleString s4 = std::move(s3);  // Move constructor called
    std::cout << "After move:" << std::endl;
    s3.print();  // s3 is now empty
    s4.print();  // s4 owns the data
    
    std::cout << "\n=== Example 3: Returning from Function ===" << std::endl;
    SimpleString s5 = createString();  // Move happens automatically
    s5.print();
    
    std::cout << "\n=== End of main ===" << std::endl;
    
    return 0;
}

/* KEY POINTS:
 * 
 * 1. COPY: Creates a duplicate, both objects own separate resources
 *    - Copy constructor: SimpleString(const SimpleString& other)
 * 
 * 2. MOVE: Transfers ownership, source object is left empty
 *    - Move constructor: SimpleString(SimpleString&& other)
 *    - && means "rvalue reference" (temporary object)
 * 
 * 3. std::move(): Casts an lvalue to rvalue, enabling move
 *    - Without it, copy happens even when move would be better
 * 
 * 4. WHEN TO USE:
 *    - Large objects (vectors, strings, etc.)
 *    - Returning objects from functions
 *    - Transferring unique ownership
 * 
 * 5. BENEFITS:
 *    - Much faster than copying (no allocation/deallocation)
 *    - Enables unique ownership semantics
 */
