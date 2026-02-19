#include <iostream>
#include <memory>

class Resource {
public:
    Resource(int val) : value(val) {
        std::cout << "Resource(" << value << ") constructed\n";
    }
    
    ~Resource() {
        std::cout << "Resource(" << value << ") destroyed\n";
    }
    
    void display() const {
        std::cout << "Resource value: " << value << std::endl;
    }
    
private:
    int value;
};

int main() {
    std::cout << "=== Shared Ownership ===" << std::endl;
    
    // Create shared_ptr - reference count = 1
    std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>(100);
    std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
    
    {
        // Multiple pointers can share ownership
        std::shared_ptr<Resource> ptr2 = ptr1;  // reference count = 2
        std::cout << "After ptr2 = ptr1:" << std::endl;
        std::cout << "  ptr1 use_count: " << ptr1.use_count() << std::endl;
        std::cout << "  ptr2 use_count: " << ptr2.use_count() << std::endl;
        
        std::shared_ptr<Resource> ptr3 = ptr1;  // reference count = 3
        std::cout << "After ptr3 = ptr1:" << std::endl;
        std::cout << "  ptr1 use_count: " << ptr1.use_count() << std::endl;
        
        ptr2->display();
        
    } // ptr2 and ptr3 go out of scope, but Resource NOT destroyed yet
    
    std::cout << "\nAfter inner block:" << std::endl;
    std::cout << "  ptr1 use_count: " << ptr1.use_count() << std::endl;
    std::cout << "  Resource still alive!\n";
    ptr1->display();
    
    // Reset manually
    std::cout << "\n=== Manual reset ===" << std::endl;
    ptr1.reset();  // Decrements count to 0, Resource destroyed
    std::cout << "ptr1 after reset: " << (ptr1 ? "valid" : "null") << std::endl;
    
    // Another example
    std::cout << "\n=== New shared_ptr ===" << std::endl;
    auto ptr4 = std::make_shared<Resource>(200);
    std::cout << "Created ptr4\n";
    
    return 0;
    // ptr4 goes out of scope, Resource(200) destroyed automatically
}
