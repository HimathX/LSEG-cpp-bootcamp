#include <iostream>
using namespace std;

int main()
{
    int i = 42;
    double d = 3.14;

    // Pointer declarations
    int *pInt = &i;
    double *pDouble = &d;

    cout << "\nDereferencing pointers (using *):" << endl;
    cout << "*pInt    = " << *pInt << endl;
    cout << "*pDouble = " << *pDouble << endl;

    // Printing values
    cout << "Values:" << endl;
    cout << "i = " << i << endl;
    cout << "d = " << d << endl;

    cout << "\nAddresses (using &):" << endl;
    cout << "&i = " << &i << endl;
    cout << "&d = " << &d << endl;

    cout << "\nPointer values (stored addresses):" << endl;
    cout << "pInt    = " << pInt << endl;
    cout << "pDouble = " << pDouble << endl;

    return 0;
}

// g++ -o pointersIntroduction pointersIntroduction.cpp && ./pointersIntroduction
