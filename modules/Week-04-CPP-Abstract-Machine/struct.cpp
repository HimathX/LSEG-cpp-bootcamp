#include <iostream>

// Define a struct for a 2D point
struct Point {
    int x;
    int y;
};

int main() {
    // Create and initialize point objects
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    
    struct Point p2 = {30, 40};  // Initialize at declaration
    
    struct Point p3;
    p3.x = -5;
    p3.y = 15;
    
    // Print the points
    std::cout << "Point 1: (" << p1.x << ", " << p1.y << ")" << std::endl;
    std::cout << "Point 2: (" << p2.x << ", " << p2.y << ")" << std::endl;
    std::cout << "Point 3: (" << p3.x << ", " << p3.y << ")" << std::endl;
    
    return 0;
}