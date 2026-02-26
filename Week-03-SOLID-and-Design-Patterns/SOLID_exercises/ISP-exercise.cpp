// ISP Exercise - Fill in the blanks
// Interface Segregation Principle

#include <iostream>
using namespace std;

class ___1___
{
public:
    virtual void photocopy() = 0;
};

class ___2___
{
public:
    virtual void print() = 0;
};

class ___3___
{
public:
    virtual void scan() = 0;
};

class MultiFunctionPhotocopyMachine : public ___1___, ___2___, ___3___
{
public:
    
    virtual void photocopy() ___4___
    {
        cout << "MultiFunction Photocopy Success" << endl;
    }
    virtual void scan() ___4___
    {
        cout << "MultiFunction Scan Success" << endl;
    }
    virtual void print() ___4___
    {
        cout << "MultiFunction Print Success" << endl;
    }
};

class Printer : public ___2___
{
public:
    virtual void print() ___4___
    {
        cout << "Printer Print Success" << endl;
    }
};

class PrintAndCopyMachine : public ___1___, ___2___
{
public:
    virtual void photocopy() ___4___
    {
        cout << "PrintAndCopyMachine Photocopy Success" << endl;
    }

    
    virtual void print() ___4___
    {
        cout << "PrintAndCopyMachine Print Success" << endl;
    }
};

int main()
{
    MultiFunctionPhotocopyMachine m1;
    m1.print();
    m1.scan();
    PrintAndCopyMachine m2;
    m2.print();
    //m2.scan();
    Printer m3;
    m3.print();
    //m3.scan();
    
}

