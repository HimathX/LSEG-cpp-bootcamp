
#include <stdio.h>
#include <iostream>
#include <memory>
#include <string>

class Message
{
public:
	Message() {}
	virtual ~Message() {}
};

// TARGET INTERFACE - what the client expects
class Encoder
{
public:
	Encoder() {}
	virtual ~Encoder() {}

	// BLANK 1: The interface the client code expects
	// Hint: Returns encoded string
	virtual const std::string encode(Message& msg) ________;
};

// This encoder already conforms to the Target interface
class JsonEncoder : public Encoder
{
public:
	JsonEncoder() {}
	virtual ~JsonEncoder() {}

	const std::string encode(Message& msg) override
	{
		std::cout << "JsonEncoder::encode" << std::endl;
		return "JsonEncoder - encoded buffer";
	}
};

// ADAPTEE - the OLD/LEGACY class with INCOMPATIBLE interface
// Notice: Different method signature - uses output parameter instead of return value
class OldXMLEncoder
{
public:
	OldXMLEncoder() {}
	virtual ~OldXMLEncoder() {}

	// BLANK 2: This is the INCOMPATIBLE interface we need to adapt
	// Hint: It uses an OUTPUT parameter (encoded) instead of returning the result
	void encode(Message& msg, std::string& ________)
	{
		std::cout << "OldXMLEncoder::encode" << std::endl;
		encoded = "OldXMLEncoder - encoded buffer";
	}
};

// ADAPTER - makes OldXMLEncoder work with the Encoder interface
// BLANK 3: The Adapter inherits from the ________ interface
class XMLEncoder : public ________
{
public:
	XMLEncoder() {}
	virtual ~XMLEncoder() {}

	// BLANK 4: Implement the TARGET interface by DELEGATING to the adaptee
	const std::string encode(Message& msg) override
	{
		std::cout << "XMLEncoder::encode" << std::endl;
		std::string encodedBuffer;
		
		// BLANK 5: Call the OLD encoder's method (the adaptee)
		// Hint: The old encoder uses output parameter, so pass encodedBuffer
		m_oldEncoder.________(msg, encodedBuffer);
		
		return encodedBuffer;
	}

private:
	// BLANK 6: The adapter CONTAINS (has-a) the adaptee
	// Hint: This is called ________ (as opposed to inheritance)
	// This is the "Object Adapter" pattern
	OldXMLEncoder m_oldEncoder;
};


int main()
{
	Message msg;
	
	// BLANK 7: Create encoder using BASE class pointer (polymorphism)
	std::unique_ptr<________> e1 = std::make_unique<JsonEncoder>();
	std::cout << e1->encode(msg) << std::endl;

	std::cout << std::endl;
	
	// BLANK 8: XMLEncoder ADAPTS OldXMLEncoder to work with Encoder interface
	// Client code doesn't know it's using an adapted legacy class!
	std::unique_ptr<Encoder> e2 = std::make_unique<________>();
	std::cout << e2->encode(msg) << std::endl;

	std::cin.get();
}

/*
=== ADAPTER PATTERN KEY POINTS ===
1. Target: The interface the client expects (Encoder)
2. Adaptee: The incompatible class to adapt (OldXMLEncoder)
3. Adapter: Wraps adaptee, implements target interface (XMLEncoder)
4. Object Adapter: Uses COMPOSITION (has-a) - more flexible
5. Class Adapter: Uses INHERITANCE (is-a) - less common in C++
6. Use when: Integrating legacy code or third-party libraries
*/
