#pragma once
#include <iostream>

template <class... Args>
void print(Args... args) {
	((std::cout << args << " "), ...);
	std::cout << std::endl;
}

class SomeClass {
   public:
	SomeClass() : value(0) { print("Default constructor called"); }
	SomeClass(int v) : value(v) { print("Parameterized constructor called with value: ", v); }
	SomeClass(const SomeClass &other) : value(other.value) {
		print("Copy constructor called with value: ", other.value);
	}
	SomeClass &operator=(const SomeClass &other) {
		if (this != &other) {
			value = other.value;
			print("Copy assignment operator called with value: ", other.value);
		}
		return *this;
	}

	SomeClass(SomeClass &&other) noexcept : value(other.value) {
		print("Move constructor called with value: ", other.value);
		other.value = 0;	 // Reset the moved-from object
	}
	SomeClass &operator=(SomeClass &&other) noexcept {
		if (this != &other) {
			value = other.value;
			print("Move assignment operator called with value: ", other.value);
			other.value = 0;	 // Reset the moved-from object
		}
		return *this;
	}

	int getValue() const { return value; }
	~SomeClass() { print("Destructor called for value: ", value); }

   private:
	int value;
};
