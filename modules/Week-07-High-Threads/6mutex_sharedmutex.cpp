#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

using namespace std;

class Counter
{
private:
  shared_mutex mtx;
  int count = 0;

public:
  int get()
  {
    mtx.lock_shared();
    int i = count;
    mtx.unlock_shared();

    return i;
  }

  void increment()
  {
    mtx.lock_shared();
    count++;
    mtx.unlock_shared();
  }
};

int main()
{
  Counter counter;

  auto increment = [&counter]()
  {
    for (int i = 0; i < 3; i++)
    {
      counter.increment();
      cout << counter.get() << endl;
    }
  };

  thread thread1(increment);
  thread thread2(increment);

  thread1.join();
  thread2.join();
  cin.get();
}