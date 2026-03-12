#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

int main()
{
    unsigned num_cpus = thread::hardware_concurrency();

    return 0;
}


    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(i, &cpuset);
    // int rc = pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuset);