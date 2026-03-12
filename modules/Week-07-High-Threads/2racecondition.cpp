#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

int main()
{
    int count = 0;
    // atomic<int> count = 0; //fix

    thread t1([&count]()
              {
        for (size_t i = 0; i < 100000; i++)
        {
            count++;
        } });
    thread t2([&count]()
              {
        for (size_t i = 0; i < 100000; i++)
        {
            count++;
        } });
    thread t3([&count]()
              {
        for (size_t i = 0; i < 100000; i++)
        {
            count++;
        } });
    thread t4([&count]()
              {
        for (size_t i = 0; i < 100000; i++)
        {
            count++;
        } });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    cout << "End" << count << endl;
    cin.get();
}