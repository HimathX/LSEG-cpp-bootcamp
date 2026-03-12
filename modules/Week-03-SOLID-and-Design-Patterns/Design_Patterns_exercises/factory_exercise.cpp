
#include <stdio.h>
#include <iostream>
#include <list>
#include <memory>

// BLANK 1: What makes this class abstract?
// Hint: It has a pure ________ function
class Employee
{
public:
	Employee() {}
	virtual ~Employee() {}

	// BLANK 2: Complete the pure virtual function declaration
	// Hint: "= 0" makes it pure virtual
	virtual void calculateSalary() ________;
};

class FullTimeEmployee : public Employee
{
public:
	FullTimeEmployee() {}
	virtual ~FullTimeEmployee() {}

	// BLANK 3: What keyword ensures this overrides the base class method?
	void calculateSalary() ________
	{
		std::cout << "FullTimeEmployee - Salary calculation logic" << std::endl;
	}
};

class PartTimeEmployee : public Employee
{
public:
	PartTimeEmployee() {}
	virtual ~PartTimeEmployee() {}

	void calculateSalary() override
	{
		std::cout << "PartTimeEmployee - Salary calculation logic" << std::endl;
	}
};

class BaseEmployeeFactory 
{
public:
	// BLANK 4: Why do we need a virtual destructor here?
	// Hint: Because we delete derived objects through base class ________
	virtual ~BaseEmployeeFactory() = default;

	// BLANK 5: Complete the factory method signature
	// Hint: Returns a smart pointer to the BASE class type
	virtual std::unique_ptr<________> createEmployee(const std::string& type) = 0;
};

class EmployeeFactory : public BaseEmployeeFactory
{
public:
	// BLANK 6: The factory creates CONCRETE classes but returns BASE class pointer
	std::unique_ptr<Employee> createEmployee(const std::string& type) override
	{
		if (type == "FullTime")
		{
			// BLANK 7: Create and return a FullTimeEmployee
			// Hint: Use std::make_unique<ConcreteType>()
			return std::make_unique<________>();
		}
		else if (type == "PartTime")
		{
			return std::make_unique<PartTimeEmployee>();
		}
		else
		{
			std::cerr << "Unknown type" << std::endl;
			return nullptr;
		}
	}
};

int main()
{
	std::list<std::unique_ptr<Employee>> employees;
	
	// BLANK 8: Create the factory using the BASE factory type
	// Hint: Polymorphism - base pointer to derived object
	std::unique_ptr<________> factory = std::make_unique<EmployeeFactory>();
	
	employees.push_back(std::move(factory->createEmployee("FullTime")));
	employees.push_back(std::move(factory->createEmployee("PartTime")));

	// BLANK 9: Why can we call calculateSalary() on different employee types?
	// Hint: This is called ________ (runtime method resolution)
	for (auto& employee : employees)
	{
		employee->calculateSalary();
	}

	std::cin.get();
}

/*
=== FACTORY PATTERN KEY POINTS ===
1. Creates objects without exposing creation logic
2. Returns BASE class type (polymorphism)
3. Client code works with abstract interface, not concrete classes
4. Easy to add new product types without changing client code
5. Virtual destructor in base factory is CRITICAL for proper cleanup
*/
