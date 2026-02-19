#include <iostream>
#include <string>

/* CONSTRUCTORS AND DESTRUCTORS IN C++
 * CONSTRUCTOR: Called automatically when object is created
 * DESTRUCTOR:  Called automatically when object is destroyed
 */

class Book {
private:
    std::string title;
    int pages;

public:
    // Constructor - initializes the object
    Book(std::string t, int p) {
        title = t;
        pages = p;
        std::cout << "[Constructor] Created: '" << title << "'" << std::endl;
    }
    
    // Destructor - cleans up when object is destroyed
    ~Book() {
        std::cout << "[Destructor] Destroyed: '" << title << "'" << std::endl;
    }
    
    void display() {
        std::cout << "Book: '" << title << "' (" << pages << " pages)" << std::endl;
    }
};

int main() {
    std::cout << "Creating book1..." << std::endl;
    Book book1("C++ Basics", 200);
    book1.display();
    
    std::cout << "\nCreating book2..." << std::endl;
    Book book2("Advanced C++", 400);
    book2.display();
    
    std::cout << "\nEnd of main - objects will be destroyed now..." << std::endl;
    // Destructors called automatically in reverse order: book2, then book1
    
    return 0;
}
