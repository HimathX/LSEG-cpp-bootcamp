#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

int main()
{
    int count = 0;

    mutex mtx;

    auto func = [&](){
        for (size_t i = 0; i < 100000; i++)
        {
            mtx.lock();
            count++;
            mtx.unlock();
        }   
    };
    

    thread t1(func);
    thread t2(func);
    thread t3(func);
    thread t4(func);
    
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::cout<<"End"<<count<<endl;
    cin.get();
}