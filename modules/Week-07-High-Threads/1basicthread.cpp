#include <iostream>
#include <chrono>

void sumTask(long long N)
{
    long long s = 0;
    for (long long i = 1; i <= N; ++i)
    {
        s += i;
    }
}

int main()
{
    const long long N = 200000000; // 200 million

    auto start = std::chrono::high_resolution_clock::now();

    sumTask(N); // Task A
    sumTask(N); // Task B

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Sequential time: " << diff.count() << " seconds\n";
}