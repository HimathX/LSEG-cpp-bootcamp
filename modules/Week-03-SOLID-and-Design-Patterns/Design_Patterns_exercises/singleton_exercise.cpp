
#include <stdio.h>
#include <iostream>
#include <string>

class Student
{
public:
	Student(const std::string& name) 
		: m_name(name)
	{
	}
	
	~Student() {}
private:
	const std::string m_name;
};

class StudentRegistry
{
public:
	// BLANK 1: Implement the static getter method
	// Hint: Return a reference to the single instance (m_reg)
	static StudentRegistry& get() { return m_reg; }

	Student* find(const std::string& name) { return nullptr; /*return student found*/ }
	void add(Student* student) {}

private:
	// BLANK 2: Why is the constructor private?
	// Hint: To prevent direct (or external) instantiation
	StudentRegistry() {}
	~StudentRegistry() {}

	// BLANK 3: Prevent copying of the singleton
	// Hint: Use "= delete" to disable copy constructor and assignment operator
	StudentRegistry(const StudentRegistry&) = delete;
	StudentRegistry& operator=(const StudentRegistry&) = delete;

	// BLANK 4: What keyword makes this instance shared across all uses?
	// Hint: This keyword means "one instance for the entire class"
	static StudentRegistry m_reg;
};

// BLANK 5: Initialize the static member outside the class
// Hint: ClassName ClassName::memberName;
StudentRegistry StudentRegistry::m_reg;

int main()
{
	// BLANK 6: How do we access the singleton?
	// Hint: We call the static getter, not use 'new'
	auto& reg = StudentRegistry::get();
	auto student = reg.find("Student1");

	std::cin.get();
}

/*
=== SINGLETON PATTERN KEY POINTS ===
1. Single instance: Only ONE object of the class exists
2. Global access: Accessed via a static method
3. Private constructor: Prevents external instantiation
4. No copying: Delete copy constructor and assignment operator
5. Static member: Holds the single instance
*/
