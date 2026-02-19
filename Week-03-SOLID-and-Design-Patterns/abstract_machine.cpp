#include <iostream>

// ===== 1. SEQUENCE OF EXECUTION =====
// The abstract machine executes statements in order (sequencing)
void sequenceExample() {
    std::cout << "\n=== Sequence of Execution ===" << std::endl;
    
    int x = 5;           // Step 1: x is initialized to 5
    x = x + 10;          // Step 2: x becomes 15
    std::cout << "x = " << x << std::endl;  // Step 3: output 15
    
    // Abstract machine guarantees this order
}

// ===== 2. MEMORY MODEL =====
// Variables occupy memory locations with specific values
void memoryModelExample() {
    std::cout << "\n=== Memory Model ===" << std::endl;
    
    int a = 10;  // 'a' occupies a memory location with value 10
    int b = 20;  // 'b' occupies a different memory location with value 20
    
    std::cout << "a is at address: " << &a << " with value: " << a << std::endl;
    std::cout << "b is at address: " << &b << " with value: " << b << std::endl;
    
    // Abstract machine tracks these memory locations
}

// ===== 3. OBSERVABLE BEHAVIOR =====
// Input/output operations are "observable" - compiler cannot change them
void observableBehaviorExample() {
    std::cout << "\n=== Observable Behavior ===" << std::endl;
    
    // These outputs MUST appear in this order - they are observable
    std::cout << "First" << std::endl;
    std::cout << "Second" << std::endl;
    std::cout << "Third" << std::endl;
}

// ===== 4. THE "AS-IF" RULE =====
// Compiler can optimize as long as observable behavior is unchanged
void asIfRuleExample() {
    std::cout << "\n=== As-If Rule ===" << std::endl;
    
    // Original code
    int x = 5;
    int y = 10;
    int z = x + y;
    std::cout << "Result: " << z << std::endl;
    
    // Compiler might optimize to:
    // std::cout << "Result: " << 15 << std::endl;
    // This is allowed because observable behavior (output) is the same!
}

// ===== 5. UNDEFINED BEHAVIOR =====
// When code violates rules, abstract machine has no defined behavior
void undefinedBehaviorExample() {
    std::cout << "\n=== Undefined Behavior ===" << std::endl;
    
    int arr[3] = {1, 2, 3};
    
    // Defined behavior - accessing valid index
    std::cout << "arr[1] = " << arr[1] << " (OK)" << std::endl;
    
    // UNDEFINED BEHAVIOR - accessing out of bounds
    // Abstract machine doesn't define what happens here!
    // std::cout << arr[10] << std::endl;  // Don't do this!
    
    std::cout << "Accessing out of bounds is undefined behavior" << std::endl;
}

// ===== 6. SEQUENCE POINTS (C++11: Sequenced Before) =====
// Points where all previous evaluations must be complete
void sequencePointsExample() {
    std::cout << "\n=== Sequence Points ===" << std::endl;
    
    int x = 5;
    
    // Semicolon is a sequence point - all effects of this statement
    // are complete before the next statement
    x = x + 1;
    std::cout << "x = " << x << std::endl;  // Guaranteed to print 6
    
    // WARNING: This has undefined behavior
    // x = x++;  // Don't do this! Multiple modifications without sequence point
}

int main() {
    std::cout << "===========================================";
    std::cout << "\nC++ ABSTRACT MACHINE - CONCEPTUAL MODEL";
    std::cout << "\n===========================================";
    
    sequenceExample();
    memoryModelExample();
    observableBehaviorExample();
    asIfRuleExample();
    undefinedBehaviorExample();
    sequencePointsExample();
    
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "The abstract machine is a specification that:" << std::endl;
    std::cout << "1. Defines how statements execute in sequence" << std::endl;
    std::cout << "2. Models memory as locations storing values" << std::endl;
    std::cout << "3. Protects observable behavior (I/O)" << std::endl;
    std::cout << "4. Allows optimizations via 'as-if' rule" << std::endl;
    std::cout << "5. Defines what is undefined behavior" << std::endl;
    
    return 0;
}

/* WHY IS THIS IMPORTANT?
 * 
 * 1. PORTABILITY: All compilers follow the same abstract machine model,
 *    so your code behaves consistently across platforms
 * 
 * 2. OPTIMIZATION: Compilers can optimize aggressively as long as they
 *    preserve observable behavior
 * 
 * 3. UNDERSTANDING: Knowing the abstract machine helps you understand
 *    why certain code is undefined or implementation-defined
 * 
 * 4. DEBUGGING: Many bugs come from assumptions about execution order
 *    or memory that violate the abstract machine model
 */
