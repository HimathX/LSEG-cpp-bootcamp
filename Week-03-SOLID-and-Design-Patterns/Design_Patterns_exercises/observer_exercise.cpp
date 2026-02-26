
#include <stdio.h>
#include <iostream>
#include <memory>
#include <list>
#include <string>

class Socket;

// BLANK 1: This is the Observer INTERFACE (abstract base class)
class SocketObserver
{
public:
	SocketObserver() {}
	virtual ~SocketObserver() {}
	
	// BLANK 2: Complete the pure virtual update method
	// Hint: This is called when the Subject notifies observers
	virtual void update(Socket& socket, const std::string& data) ________; 
};

// This is the SUBJECT that observers watch
class Socket
{
public:
	Socket() {}
	virtual ~Socket() {}
	
	// When data arrives, notify all observers
	void onData(const std::string& data)
	{
		// BLANK 3: Call the method that notifies all registered observers
		________(data);
	}

	// BLANK 4: Method to add an observer to the list
	// Hint: This is how observers "subscribe" to updates
	void registerObserver(std::shared_ptr<SocketObserver> observer)
	{
		m_observers.________(observer);
	}

private:
	void notifyObservers(const std::string& data)
	{
		// BLANK 5: Loop through observers and call their update method
		// Hint: Use reference to avoid copying shared_ptr
		for (const auto& observer : m_observers)
		{
			observer->________(________, data);
		}
	}
	
	// BLANK 6: Container to store all registered observers
	// Hint: Use std::list of shared_ptr<SocketObserver>
	std::list<________> m_observers;
};

// CONCRETE Observer 1
class WindowObserver : public SocketObserver
{
public:
	WindowObserver() {}
	virtual ~WindowObserver() {}
	
	// BLANK 7: Implement the update method (override from base)
	void update(Socket& socket, const std::string& data) ________
	{
		std::cout << "Data received by WindowObserver. Data:" << data << std::endl;
	}
};

// CONCRETE Observer 2
class ContainerObserver : public SocketObserver
{
public:
	ContainerObserver() {}
	virtual ~ContainerObserver() {}
	
	void update(Socket& socket, const std::string& data) override
	{
		std::cout << "Data received by ContainerObserver. Data:" << data << std::endl;
	}
};

int main()
{
	std::unique_ptr<Socket> socket = std::make_unique<Socket>();
	
	// BLANK 8: Create observers - what smart pointer allows shared ownership?
	// Hint: Multiple places might hold references to the same observer
	________<SocketObserver> observer1 = std::make_shared<WindowObserver>();
	std::shared_ptr<SocketObserver> observer2 = std::make_shared<ContainerObserver>();
	
	// BLANK 9: Register (subscribe) observers to the socket
	socket->________(observer1);
	socket->registerObserver(observer2);

	// When data arrives, ALL registered observers will be notified
	socket->onData("Test data");

	std::cin.get();
}

/*
=== OBSERVER PATTERN KEY POINTS ===
1. Subject (Socket): Maintains list of observers, notifies them of changes
2. Observer (SocketObserver): Interface with update() method
3. One-to-Many: One subject, multiple observers
4. Loose coupling: Subject doesn't know concrete observer types
5. Push model: Subject pushes data to observers via update()
*/
