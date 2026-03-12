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
                  { count++; });
        thread t2([&count]()
                  { count++; });

        t1.join();
        t2.join();

        cout << "End" << count << endl;
        cin.get();
}