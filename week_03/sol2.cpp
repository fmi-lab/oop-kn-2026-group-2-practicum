#include <iostream>

class Dipper {
	int		   number;
	static int counter;

   public:
	Dipper() { number = counter++; }

	Dipper(const Dipper &) { number = counter++; }
	// Dipper(Const Dipper &) : Dipper() {} // същото

	Dipper &operator=(const Dipper &other) { return *this; }
	int		getNumber() const { return number; }
};

int Dipper::counter = 0;

int main() {
	Dipper dipper1, dipper2;

	std::cout << dipper1.getNumber() << std::endl;	   // 1
	std::cout << dipper2.getNumber() << std::endl;	   // 2

	{
		Dipper dipper3;
		std::cout << dipper3.getNumber() << std::endl;	   // 3
	}
	Dipper dipper3;
	std::cout << dipper3.getNumber() << std::endl;	   // 4

	return 0;
}
