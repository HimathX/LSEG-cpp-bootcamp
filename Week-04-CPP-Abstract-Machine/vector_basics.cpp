#include <iostream>
#include <vector>

void print(const std::vector<int>& vec) {
    std::cout << "[ ";
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << "] size=" << vec.size() << ", capacity=" << vec.capacity() << std::endl;
}

int main() {
    // Create empty vector
    std::vector<int> vec;
    std::cout << "=== Initial ===" << std::endl;
    print(vec);
    
    // Add elements
    std::cout << "\n=== Adding elements ===" << std::endl;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    print(vec);
    
    // Access elements
    std::cout << "\n=== Accessing elements ===" << std::endl;
    std::cout << "vec[0] = " << vec[0] << std::endl;
    std::cout << "vec.at(1) = " << vec.at(1) << std::endl;
    std::cout << "vec.front() = " << vec.front() << std::endl;
    std::cout << "vec.back() = " << vec.back() << std::endl;
    
    // Modify element
    vec[1] = 25;
    std::cout << "\nAfter vec[1] = 25: ";
    print(vec);
    
    // Insert at position
    std::cout << "\n=== Insert at position 1 ===" << std::endl;
    vec.insert(vec.begin() + 1, 15);
    print(vec);
    
    // Remove elements
    std::cout << "\n=== Removing elements ===" << std::endl;
    vec.pop_back();  // Remove last element
    std::cout << "After pop_back(): ";
    print(vec);
    
    vec.erase(vec.begin() + 1);  // Remove element at index 1
    std::cout << "After erase(index 1): ";
    print(vec);
    
    // Initialize with values
    std::cout << "\n=== Initialize with values ===" << std::endl;
    std::vector<int> vec2 = {100, 200, 300, 400};
    print(vec2);
    
    // Reserve capacity
    std::cout << "\n=== Reserve capacity ===" << std::endl;
    std::vector<int> vec3;
    vec3.reserve(10);
    std::cout << "After reserve(10): ";
    print(vec3);
    
    // Clear
    std::cout << "\n=== Clear ===" << std::endl;
    vec2.clear();
    std::cout << "After clear(): ";
    print(vec2);
    std::cout << "Is empty? " << (vec2.empty() ? "yes" : "no") << std::endl;
    
    // Iteration methods
    std::cout << "\n=== Iteration Methods ===" << std::endl;
    std::vector<int> nums = {5, 10, 15, 20, 25};
    
    // 1. Range-based for loop (C++11)
    std::cout << "Range-based for: ";
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // 2. Index-based loop
    std::cout << "Index-based: ";
    for (size_t i = 0; i < nums.size(); ++i) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;
    
    // 3. Iterator
    std::cout << "Iterator: ";
    for (auto it = nums.begin(); it != nums.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 4. Reverse iterator
    std::cout << "Reverse iterator: ";
    for (auto it = nums.rbegin(); it != nums.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 5. Modifying during iteration
    std::cout << "Modify with reference: ";
    for (int& num : nums) {
        num *= 2;  // Double each element
    }
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
