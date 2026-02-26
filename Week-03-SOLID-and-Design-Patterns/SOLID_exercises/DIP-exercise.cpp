// DIP Exercise - Fill in the blanks
// Dependency Inversion Principle

#include <iostream>
using namespace std;

class ___1___
{
public:
    ___2___ string getCustomerName(int id) = 0;
};

class CustomerDataAccess : ___3___ ___1___
{
public:
    ___2___ string getCustomerName(int id) {
        return "Customer Name Derived from DB";
    }
};


class CustomerBusinessLogic
{
    ___1___* ___4___ = new CustomerDataAccess();
public:
    string getCustomerName(int id)
    {
        return ___4___->getCustomerName(id);
    }
};


int main()
{
    CustomerBusinessLogic customer;
    string name = customer.getCustomerName(1);
    cout << name << endl;
    
}

