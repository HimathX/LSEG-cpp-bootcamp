#include <iostream>
#include <vector>
#include <string>

int main() {
    // Create a vector of strings
    std::vector<std::string> names;
    
    // Add 3 names
    names.push_back("Alice");
    names.push_back("Bob");
    names.push_back("Charlie");
    
    // Try to access 10th element (will throw exception)
    try {
        std::cout << "Trying to access 10th element..." << std::endl;
        std::string name = names.at(10);
        std::cout << "10th element: " << name << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    // Print all 3 names
    std::cout << "\nAll names in vector:" << std::endl;
    for (size_t i = 0; i < names.size(); ++i) {
        std::cout << i << ": " << names[i] << std::endl;
    }
    
    return 0;
}
