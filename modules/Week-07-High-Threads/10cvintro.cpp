#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main()
{
    int i =0;

    thread t1([&](){
        this_thread::sleep_for(chrono::seconds(50));
        i++;
    });

    t1.join();

    // while( i !=1)
    // {
    //     this_thread::sleep_for(chrono::milliseconds(10));
    // }
    cout << "Value : "<<i<<endl;    
    cin.get();
}