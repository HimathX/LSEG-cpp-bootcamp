#include <iostream>
#include <cmath>

// Define a class for a 2D point
// Unlike struct (which has public members by default), 
// class has private members by default
class Point {
private:
    // Private data members - can only be accessed within the class
    int x;
    int y;
    
    // Private member function - helper function only used internally
    // Calculates the squared distance from origin (avoids sqrt for efficiency)
    double distanceSquaredFromOrigin() {
        return x * x + y * y;
    }

public:
    // Public constructor - allows initialization of private members
    Point(int xVal, int yVal) {
        x = xVal;
        y = yVal;
    }
    
    // Public member function - can be called from outside the class
    // Displays the point coordinates and distance from origin
    void display() {
        std::cout << "Point: (" << x << ", " << y << ")";
        
        // Private member function can be called from within public functions
        double distSq = distanceSquaredFromOrigin();
        double distance = sqrt(distSq);
        
        std::cout << " - Distance from origin: " << distance << std::endl;
    }
    
    // Public getter methods - provide controlled access to private data
    int getX() { return x; }
    int getY() { return y; }
    
    // Public setter methods - allow controlled modification of private data
    void setX(int xVal) { x = xVal; }
    void setY(int yVal) { y = yVal; }
};

int main() {
    // Create and initialize point objects using constructor
    Point p1(10, 20);
    Point p2(30, 40);
    Point p3(-5, 15);
    
    // Call public member function to display points
    // Note: We cannot directly access p1.x or p1.y because they are private
    p1.display();
    p2.display();
    p3.display();
    
    // Demonstrate using getter methods to access private data
    std::cout << "\nAccessing p1 coordinates using getters: (" 
              << p1.getX() << ", " << p1.getY() << ")" << std::endl;
    
    // Demonstrate using setter methods to modify private data
    p1.setX(100);
    p1.setY(200);
    std::cout << "After modification: ";
    p1.display();
    
    /* KEY DIFFERENCES FROM STRUCT:
     * 1. Class members are private by default (struct: public by default)
     * 2. data is protected and accessed through methods
     * 3. Can have private helper functions like distanceSquaredFromOrigin()
     * 4. Constructor provides clean initialization syntax
     * 5. Getters/setters provide controlled access to private data
     */
    
    return 0;
}
