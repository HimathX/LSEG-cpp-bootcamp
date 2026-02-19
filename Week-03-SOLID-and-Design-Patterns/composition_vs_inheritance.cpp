#include <iostream>
#include <string>

/* COMPOSITION vs INHERITANCE
 * INHERITANCE: "IS-A" relationship (Dog IS-A Animal)
 * COMPOSITION: "HAS-A" relationship (Car HAS-A Engine)
 * Composition is more flexible - components can be changed at runtime
 */

// ===== INHERITANCE Approach =====
class Bird {
public:
    virtual void move() {
        std::cout << "Bird is flying" << std::endl;
    }
};

class Penguin : public Bird {
public:
    void move() override {
        std::cout << "Penguin is waddling (can't fly!)" << std::endl;
    }
};
// Problem: Not all birds can fly, but inheritance forces this relationship

// ===== COMPOSITION Approach =====
class FlyBehavior {
public:
    void fly() {
        std::cout << "Flying in the sky" << std::endl;
    }
};

class WalkBehavior {
public:
    void walk() {
        std::cout << "Walking on ground" << std::endl;
    }
};

// Animal using composition - more flexible
class Animal {
private:
    std::string name;
    FlyBehavior* flyBehavior;   // HAS-A flying ability (optional)
    WalkBehavior* walkBehavior; // HAS-A walking ability (optional)

public:
    Animal(std::string n, FlyBehavior* fb, WalkBehavior* wb) 
        : name(n), flyBehavior(fb), walkBehavior(wb) {}
    
    void move() {
        std::cout << name << ": ";
        if (flyBehavior) {
            flyBehavior->fly();
        } else if (walkBehavior) {
            walkBehavior->walk();
        }
    }
};

int main() {
    std::cout << "=== INHERITANCE (Rigid) ===" << std::endl;
    Bird bird;
    bird.move();
    
    Penguin penguin;
    penguin.move();
    
    std::cout << "\n=== COMPOSITION (Flexible) ===" << std::endl;
    FlyBehavior canFly;
    WalkBehavior canWalk;
    
    // Can create different combinations easily
    Animal eagle("Eagle", &canFly, nullptr);
    eagle.move();
    
    Animal elephant("Elephant", nullptr, &canWalk);
    elephant.move();
    
    return 0;
}

/* KEY ADVANTAGES OF COMPOSITION:
 * 1. Flexibility: Mix and match behaviors
 * 2. No forced relationships: Only use what you need
 * 3. Runtime changes: Can swap components dynamically
 * 4. Principle: "Favor composition over inheritance"
 */
