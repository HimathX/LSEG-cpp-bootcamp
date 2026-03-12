
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

class RestApiCall1 {
public:
    void operator()(const std::string endpoint)
    {
        //Using for loop and sleep to indicate long running functions
        for (int i = 0; i < 10; i++)
        {
            std::cout << "Calling rest_api_1 [" << endpoint.c_str() << "] ..." << std::endl;
            std::this_thread::sleep_for(1s);
        }
    }
};

int main()
{
    RestApiCall1 rs1;

    std::thread t1(rs1, "endpoint1");

    t1.join();

	std::cin.get();
}