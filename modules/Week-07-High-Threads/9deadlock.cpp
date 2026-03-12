#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex m1;
mutex m2;


void func1(){
    m1.lock();
    cout<<"Func1 acquired m1"<<endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    m2.lock();
    cout<<"Func1 acquired m2"<<endl;
    cout<<"Function1"<<endl;
    m2.unlock();
    m1.unlock();
}

void func2(){
    m1.lock();
    cout<<"Func2 acquired m2"<<endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    m2.lock();
    cout<<"Func2 acquired m1"<<endl;
    cout<<"Function2"<<endl;
    m1.unlock();
    m2.unlock();
}

int main()
{
    thread t1(func1);
    thread t2(func2);

    t1.join();
    t2.join();
    cin.get();
}