// SRP Exercise - Fill in the blanks
// Single Responsibility Principle

#include <iostream>
using namespace std;


class ___1___
{
    string dburl;
public:
    ___1___()
    {
        //create the db connection
    }

    void ___2___()
    {
        cout << "Save called" << endl;
        //create db connection
        //code to save to DB
    }

};

class ___3___
{
    string email;
    string emailpassword;
public:
    void ___4___(string toemail, string content)
    {
        cout << "Send email called" << endl;
        //create smtp connection
        //send email
    }
};


class Student
{
private:
    int id;
    string name;
    int age;
    
    int marks[9];

    ___1___ ___5___;
    ___3___ ___6___;
    
    
public:
    Student(int id, string name, int age, string email): id(id), name(name), age(age)
    {
        
    }
    string getName() { return name; }
    int getId() { return id;  }
    int getAge() { return age; }

    void save() {
        ___5___.___2___();
    }

    void sendEmail(string toemail, string content)
    {
        ___6___.___4___(toemail, content);
    }
    
};


int main()
{
    Student student(1,"John",23, "john@gmail.com");

    student.save();
    student.sendEmail("warne@gmail.com", "Hi, How are you");

    
}

