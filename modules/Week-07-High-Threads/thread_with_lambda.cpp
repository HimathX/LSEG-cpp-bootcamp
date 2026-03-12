
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main()
{
	auto f1 = [](const std::string endpoint) {
		for (int i = 0; i < 10; i++)
		{
			std::cout << "Calling rest_api_1 [" << endpoint.c_str() << "] ..." << std::endl;
			std::this_thread::sleep_for(1s);
		}
	};

	auto f2 = [](const std::string endpoint){
		for (int i = 0; i < 5; i++)
		{
			std::cout << "Calling rest_api_2 [" << endpoint.c_str() << "] ..." << std::endl;
			std::this_thread::sleep_for(3s);
		}
	};

	std::thread restCall1(f1, "endpoint1");
	std::thread restCall2(f2, "endpoint2");

	restCall1.join();
	restCall2.join();

	std::cout << "All threads completed" << std::endl;

	std::cin.get();
}


