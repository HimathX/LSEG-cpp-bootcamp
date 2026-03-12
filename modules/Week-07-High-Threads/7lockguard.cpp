#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

void methodA(int& count, mutex& mtx)
{
    for (size_t i = 0; i < 100000; i++)
    {
        lock_guard<mutex> guard(mtx);


       // mtx.lock();
        count++;
        //mtx.unlock();
       


    }  
}

int main()
{
    
    int count = 0;
    mutex mtx;


    thread t1(methodA, ref(count), ref(mtx));
    thread t2(methodA, ref(count), ref(mtx));
    thread t3(methodA, ref(count), ref(mtx));
    thread t4(methodA, ref(count), ref(mtx));
    
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::cout<<"End"<<count<<endl;
    cin.get();
}