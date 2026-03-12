// LSP Exercise - Liskov Substitution Principle
#include <iostream>
using namespace std;

class Bird
{
public:
    // It is a good practice to include a virtual destructor in base classes
    virtual ~Bird() = default; 
};

class FlyingBird : public Bird
{
public:
    // Added 'virtual' so derived classes can override it properly
    virtual void fly()
    {
        cout << "Bird flies" << endl;
    }
};

class Parrot : public FlyingBird
{
public: // Added 'public' so the method can be accessed from outside
    // Parrot can fly, so it inherits from FlyingBird
    void fly() override
    {
        cout << "Parrot flies" << endl;
    }
};

class Ostrich : public Bird
{
    // Ostrich cannot fly, so it inherits directly from Bird
    // It does not have a fly() method, preventing misuse.
};

int main()
{
    FlyingBird *b = new Parrot();
    b->fly(); // Outputs: Parrot flies
    
    Bird *nb = new Ostrich();
    // nb->fly(); // This would cause an error because Ostrich doesn't support flying
    
    // Clean up dynamically allocated memory
    delete b;
    delete nb;
    
    return 0;
}