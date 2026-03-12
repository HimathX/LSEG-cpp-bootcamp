#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

 
class RecursiveClass {
    recursive_mutex m;
    string shared;
  public:
    void func1() {
      m.lock();
      cout<< "function1" <<endl;
      m.unlock();
    }

    void func2() {
      m.lock();
      func1();
      cout << "function 2" <<endl;
      m.unlock();
    };
};
 
int main() 
{
    RecursiveClass rc;
    thread t1(&RecursiveClass::func1, &rc);
    thread t2(&RecursiveClass::func2, &rc);
    t1.join();
    t2.join();
    cin.get();
}