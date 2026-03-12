
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

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

class RestApiCall2 {
public:
	void operator()(const std::string endpoint)
	{
		//Using for loop and sleep to indicate long running functions
		for (int i = 0; i < 5; i++)
		{
			std::cout << "Calling rest_api_2 [" << endpoint.c_str() << "] ..." << std::endl;
			std::this_thread::sleep_for(3s);
		}
	}
};

int main()
{
	std::thread restCall1(RestApiCall1(), "endpoint1");
	std::thread restCall2(RestApiCall2(), "endpoint2");

	restCall1.join();
	restCall2.join();

	std::cout << "All threads completed" << std::endl;

	std::cin.get();
}