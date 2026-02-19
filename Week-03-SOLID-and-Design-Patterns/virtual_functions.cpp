#include <iostream>
#include <string>

/* VIRTUAL FUNCTIONS IN C++
 * Allow derived classes to override base class methods
 * Enable runtime polymorphism (calling correct function based on actual object type)
 */

// Base class
class Animal {
public:
    // Virtual function - can be overridden by derived classes
    virtual void makeSound() {
        std::cout << "Animal makes a generic sound" << std::endl;
    }
    
    // Non-virtual function - cannot be overridden properly
    void sleep() {
        std::cout << "Animal is sleeping" << std::endl;
    }
};

// Derived class 1
class Dog : public Animal {
public:
    // Override the virtual function
    void makeSound() override {
        std::cout << "Dog says: Woof! Woof!" << std::endl;
    }
    
    void sleep() {
        std::cout << "Dog is sleeping on the couch" << std::endl;
    }
};

// Derived class 2
class Cat : public Animal {
public:
    // Override the virtual function
    void makeSound() override {
        std::cout << "Cat says: Meow!" << std::endl;
    }
    
    void sleep() {
        std::cout << "Cat is sleeping in a box" << std::endl;
    }
};

int main() {
    std::cout << "=== Creating Objects ===" << std::endl;
    Dog dog;
    Cat cat;
    
    std::cout << "\n=== Direct Calls (no polymorphism) ===" << std::endl;
    dog.makeSound();
    cat.makeSound();
    
    std::cout << "\n=== Using Base Class Pointers (polymorphism) ===" << std::endl;
    // Base class pointer pointing to derived class objects
    Animal* ptr1 = &dog;
    Animal* ptr2 = &cat;
    
    // Virtual function: calls the DERIVED class version (runtime polymorphism)
    std::cout << "ptr1->makeSound(): ";
    ptr1->makeSound();  // Calls Dog::makeSound()
    
    std::cout << "ptr2->makeSound(): ";
    ptr2->makeSound();  // Calls Cat::makeSound()
    
    std::cout << "\n=== Non-Virtual Function Behavior ===" << std::endl;
    // Non-virtual function: calls the BASE class version (no polymorphism)
    std::cout << "ptr1->sleep(): ";
    ptr1->sleep();  // Calls Animal::sleep() - NOT Dog::sleep()
    
    std::cout << "ptr2->sleep(): ";
    ptr2->sleep();  // Calls Animal::sleep() - NOT Cat::sleep()
    
    std::cout << "\n=== Why Virtual Functions Matter ===" << std::endl;
    // This is useful when you have arrays or collections of base class pointers
    Animal* animals[3] = {&dog, &cat, new Animal()};
    
    for (int i = 0; i < 3; i++) {
        std::cout << "Animal " << i+1 << ": ";
        animals[i]->makeSound();  // Each calls its own version!
    }
    
    delete animals[2];  // Clean up dynamically allocated object
    
    return 0;
}

/* KEY POINTS:
 * - virtual keyword enables runtime polymorphism
 * - Base class pointer can call derived class methods (if virtual)
 * - Without virtual, base class version is always called
 * - Use override keyword for clarity (C++11)
 */
