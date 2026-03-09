#include <iostream>

class Agent {
	static std::size_t count;

   public:
	Agent() { ++count; }
	Agent(const Agent &) { ++count; }
	~Agent() { --count; }
	Agent			  &operator=(const Agent &) { return *this; }
	static std::size_t getCount() { return count; }
};

std::size_t Agent::count = 0;

int main() {
	std::cout << Agent::getCount() << std::endl;	 // 0

	Agent  smith;
	Agent *jones;

	std::cout << Agent::getCount() << std::endl;	 // 1

	{
		Agent smith;
		jones = new Agent;
		std::cout << Agent::getCount() << std::endl;	 // 3
	}

	std::cout << Agent::getCount() << std::endl;	 // 2

	delete jones;

	std::cout << Agent::getCount() << std::endl;	 //  1

	Agent a3 = smith;

	smith = a3;

	std::cout << Agent::getCount() << std::endl;	 // 2

	return 0;
};
