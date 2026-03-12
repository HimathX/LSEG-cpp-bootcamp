#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;

int main()
{
    int i = 0;
    mutex mtx;
    condition_variable cv;

    thread t1([&]() {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "t1 acquiring lock" << endl;
        unique_lock<mutex> lock(mtx);
        cout << "t1 acquired lock" << endl;
        i++;
        this_thread::sleep_for(chrono::milliseconds(100));
        lock.unlock();
        cout << "t1 released lock. notifying" << endl;
        cv.notify_one();
        });

    

    unique_lock<mutex> lock1(mtx);
    cout << "main  acquired lock. waiting." << endl;
    while (i != 1)
    {
        cv.wait(lock1);
        // this_thread::sleep_for(chrono::milliseconds(10));
    }

    //cv.wait(lock, [&](){ return i ==1;});

    cout << "main received notify" << endl;
    cout << "Value : " << i << endl;
    t1.join();
    cin.get();
}