#include <iostream>
#include <string>

/* MEMBER INITIALIZATION IN C++
 * This file demonstrates various ways to initialize class members.
 * Understanding these techniques is crucial for writing efficient C++ code.
 */

// Example 1: Traditional Constructor with Assignment
class Rectangle_V1 {
private:
    int width;
    int height;

public:
    // Constructor using assignment (less efficient)
    // Members are first default-initialized, then assigned
    Rectangle_V1(int w, int h) {
        width = w;   // Assignment happens AFTER default initialization
        height = h;
    }
    
    void display() {
        std::cout << "Rectangle: " << width << " x " << height << std::endl;
    }
};

// Example 2: Member Initializer List (Preferred Method)
class Rectangle_V2 {
private:
    int width;
    int height;

public:
    // Constructor using member initializer list (more efficient)
    // Members are directly initialized with the given values
    Rectangle_V2(int w, int h) : width(w), height(h) {
        // Constructor body can be empty or contain additional logic
        std::cout << "Rectangle created using initializer list" << std::endl;
    }
    
    void display() {
        std::cout << "Rectangle: " << width << " x " << height << std::endl;
    }
};

// Example 3: In-Class Member Initialization (C++11)
class Circle {
private:
    double radius = 5.0;        // Default value set in the class definition
    std::string color = "red";  // Can be overridden by constructor

public:
    // Default constructor uses in-class initialization
    Circle() {
        std::cout << "Circle created with default values" << std::endl;
    }
    
    // Parameterized constructor overrides default values
    Circle(double r, std::string c) : radius(r), color(c) {
        std::cout << "Circle created with custom values" << std::endl;
    }
    
    void display() {
        std::cout << "Circle: radius=" << radius << ", color=" << color << std::endl;
    }
};

// Example 4: Const and Reference Members (MUST use initializer list)
class Temperature {
private:
    const int id;              // Const member - cannot be assigned, must be initialized
    double& externalTemp;       // Reference member - must be initialized
    double value;

public:
    // Const and reference members REQUIRE initializer list
    Temperature(int identifier, double& temp, double val) 
        : id(identifier), externalTemp(temp), value(val) {
        // This is the ONLY way to initialize const and reference members
        // You cannot assign to them in the constructor body
    }
    
    void display() {
        std::cout << "Temperature[" << id << "]: " << value 
                  << "°C (external: " << externalTemp << "°C)" << std::endl;
    }
    
    void updateExternal(double newTemp) {
        externalTemp = newTemp;  // Can modify through reference
    }
};

// Example 5: Order of Initialization
class InitOrder {
private:
    int first;
    int second;
    int third;

public:
    // IMPORTANT: Members are initialized in the order they are DECLARED in the class,
    // NOT in the order they appear in the initializer list
    InitOrder(int val) : third(val), first(val * 2), second(first + third) {
        // Even though 'third' appears first in the initializer list,
        // 'first' is initialized first because it's declared first in the class
        std::cout << "Initialized: first=" << first << ", second=" << second 
                  << ", third=" << third << std::endl;
    }
};

// Example 6: Delegating Constructors (C++11)
class Point {
private:
    int x;
    int y;

public:
    // Primary constructor
    Point(int xVal, int yVal) : x(xVal), y(yVal) {
        std::cout << "Point(" << x << ", " << y << ") created" << std::endl;
    }
    
    // Delegating constructor - calls another constructor
    Point() : Point(0, 0) {  // Delegates to Point(int, int)
        std::cout << "Using delegating constructor" << std::endl;
    }
    
    // Another delegating constructor
    Point(int val) : Point(val, val) {  // Creates a point at (val, val)
    }
    
    void display() {
        std::cout << "Point: (" << x << ", " << y << ")" << std::endl;
    }
};

int main() {
    std::cout << "=== Example 1: Traditional Assignment ===" << std::endl;
    Rectangle_V1 r1(10, 20);
    r1.display();
    
    std::cout << "\n=== Example 2: Member Initializer List ===" << std::endl;
    Rectangle_V2 r2(15, 25);
    r2.display();
    
    std::cout << "\n=== Example 3: In-Class Initialization ===" << std::endl;
    Circle c1;  // Uses default values
    c1.display();
    Circle c2(10.5, "blue");  // Custom values
    c2.display();
    
    std::cout << "\n=== Example 4: Const and Reference Members ===" << std::endl;
    double roomTemp = 22.5;
    Temperature t1(1, roomTemp, 36.6);
    t1.display();
    roomTemp = 25.0;  // Modifying the original variable
    t1.display();     // Shows updated external temperature
    
    std::cout << "\n=== Example 5: Order of Initialization ===" << std::endl;
    InitOrder io(5);
    
    std::cout << "\n=== Example 6: Delegating Constructors ===" << std::endl;
    Point p1;        // Uses delegating constructor -> Point(0, 0)
    p1.display();
    Point p2(7);     // Uses delegating constructor -> Point(7, 7)
    p2.display();
    Point p3(3, 4);  // Uses primary constructor
    p3.display();
    
    /* BEST PRACTICES:
     * 1. Prefer member initializer lists over assignment in constructor body
     * 2. Use in-class initialization for default values (C++11+)
     * 3. ALWAYS use initializer list for const, reference, and class-type members
     * 4. Be aware that initialization order follows declaration order, not list order
     * 5. Use delegating constructors to avoid code duplication
     * 6. Initializer lists are more efficient (direct initialization vs default + assignment)
     */
    
    return 0;
}
