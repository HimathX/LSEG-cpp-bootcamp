// LSP Exercise - Fill in the blanks
// Liskov Substitution Principle

#include <iostream>
using namespace std;

class ___1___
{

public: 
    
};

class ___2___ : public ___1___
{
public:
    void ___3___()
    {
        cout << "Bird flies" << endl;
    }
};

class Parrot: public ___2___
{
    //Parrot can fly, so it inherits from ___2___
};

class Ostrich : public ___1___
{
    //Ostrich cannot fly, so it inherits directly from ___1___
};

int main()
{
    ___2___* b = new Parrot();
    b->___3___();
    ___1___* nb = new Ostrich();
    //nb->___3___();
}


