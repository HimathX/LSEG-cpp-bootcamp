
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

void rest_api_call1(const std::string& endpoint)
{
	//Using for loop and sleep to indicate long running functions
	for (int i = 0; i < 10; i++)
	{
		std::cout << "Calling rest_api_1 [" << endpoint.c_str() << "] ..." << std::endl;
		std::this_thread::sleep_for(1s);
	}
}

void rest_api_call2(const std::string& endpoint)
{
	//Using for loop and sleep to indicate long running functions
	for (int i = 0; i < 5; i++)
	{
		std::cout << "Calling rest_api_2 [" << endpoint.c_str() << "] ..." << std::endl;
		std::this_thread::sleep_for(3s);
	}
}

int main()
{
	std::thread restCall1(rest_api_call1, "endpoint1");
	std::thread restCall2(rest_api_call2, "endpoint2");

	restCall1.join();
	restCall2.join();

	std::cout << "All threads completed" << std::endl;

	std::cin.get();
}


