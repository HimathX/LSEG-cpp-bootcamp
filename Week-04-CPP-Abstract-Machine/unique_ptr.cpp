#include <iostream>
#include <memory>

/* UNIQUE_PTR vs RAW POINTERS
 * 
 * Raw pointers require manual memory management (new/delete)
 * unique_ptr provides automatic memory management and exclusive ownership
 */

class Resource {
private:
    int id;

public:
    Resource(int i) : id(i) {
        std::cout << "[Resource " << id << "] Created" << std::endl;
    }
    
    ~Resource() {
        std::cout << "[Resource " << id << "] Destroyed" << std::endl;
    }
    
    void use() {
        std::cout << "[Resource " << id << "] Being used" << std::endl;
    }
};

// ===== RAW POINTER PROBLEMS =====
void rawPointerExample() {
    std::cout << "\n=== Raw Pointer (Manual Management) ===" << std::endl;
    
    Resource* raw = new Resource(1);
    raw->use();
    
    // Problem 1: Must remember to delete
    delete raw;  // Forget this = memory leak!
    
    // Problem 2: Can accidentally use after delete
    // raw->use();  // Dangerous! Undefined behavior
    
    // Problem 3: Can delete twice
    // delete raw;  // Crash!
}

void rawPointerLeak() {
    std::cout << "\n=== Raw Pointer Memory Leak ===" << std::endl;
    
    Resource* raw = new Resource(2);
    raw->use();
    
    // Function returns without delete - MEMORY LEAK!
    // Resource never destroyed
}

// ===== UNIQUE_PTR BENEFITS =====
void uniquePtrExample() {
    std::cout << "\n=== unique_ptr (Automatic Management) ===" << std::endl;
    
    std::unique_ptr<Resource> ptr = std::make_unique<Resource>(3);
    ptr->use();
    
    // No delete needed! Automatic cleanup when ptr goes out of scope
}  // Resource automatically destroyed here


// ===== OWNERSHIP TRANSFER =====
void ownershipExample() {
    std::cout << "\n=== Ownership Transfer ===" << std::endl;
    
    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>(5);
    ptr1->use();
    
    // Transfer ownership with std::move
    std::unique_ptr<Resource> ptr2 = std::move(ptr1);
    
    // ptr1 is now nullptr (no longer owns the resource)
    if (ptr1 == nullptr) {
        std::cout << "ptr1 is now empty" << std::endl;
    }
    
    // ptr2 now owns the resource
    ptr2->use();
    
    // Cannot copy unique_ptr (enforces single ownership)
    // std::unique_ptr<Resource> ptr3 = ptr2;  // Compile error!
}

// ===== FUNCTION PARAMETERS =====
void processResource(std::unique_ptr<Resource> ptr) {
    std::cout << "\n=== Inside processResource ===" << std::endl;
    ptr->use();
    // ptr destroyed here, taking ownership responsibility
}

void passingOwnershipExample() {
    std::cout << "\n=== Passing Ownership to Function ===" << std::endl;
    
    std::unique_ptr<Resource> ptr = std::make_unique<Resource>(6);
    
    // Transfer ownership to function
    processResource(std::move(ptr));
    
    // ptr is now empty
    if (ptr == nullptr) {
        std::cout << "ptr is empty after function call" << std::endl;
    }
}

int main() {
    rawPointerExample();
    
    std::cout << "\n--- Watch for memory leak! ---" << std::endl;
    rawPointerLeak();  // Resource 2 never destroyed!
    std::cout << "--- Leak happened (Resource 2 not destroyed) ---" << std::endl;
    
    uniquePtrExample();
    ownershipExample();
    passingOwnershipExample();
    
    std::cout << "\n=== End of main ===" << std::endl;
    
    return 0;
}

/* KEY DIFFERENCES:
 * 
 * RAW POINTERS:
 * - Manual memory management (new/delete)
 * - Easy to forget delete → memory leaks
 * - Can use after delete → undefined behavior
 * - Can delete twice → crash
 * - No ownership semantics
 * 
 * UNIQUE_PTR:
 * - Automatic memory management (RAII)
 * - No memory leaks (automatic cleanup)
 * - Cannot use after move (becomes nullptr)
 * - Cannot be copied (exclusive ownership)
 * - Clear ownership semantics
 * 
 * WHEN TO USE unique_ptr:
 * - Single ownership of dynamically allocated objects
 * - Returning dynamically allocated objects from functions
 * - Managing resources with clear ownership
 * - Default choice for dynamic memory in modern C++
 * 
 * Rule: Prefer unique_ptr over raw pointers for ownership
 */
