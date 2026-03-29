#include <iostream>
#include <cstring>

#include "functions.hpp"

class Person {
	char *name;
	int	  age;

   public:
	Person() : name(nullptr), age(0) {}
	Person(const char *name, int age) : name(new char[std::strlen(name) + 1]), age(age) {
		std::strcpy(this->name, name);
	}
	Person(const Person &other) : name(new char[std::strlen(other.name) + 1]), age(other.age) {
		std::strcpy(this->name, other.name);
	}
	~Person() { delete[] name; }
	Person &operator=(const Person &other) {
		if (this == &other) return *this;
		char *new_name = new char[std::strlen(other.name) + 1];
		std::strcpy(new_name, other.name);
		delete[] name;
		name = new_name;
		age	 = other.age;
		return *this;
	}

	const char *getName() const { return name; }
	int			getAge() const { return age; }

	friend std::ostream &operator<<(std::ostream &os, const Person &p) {
		os << p.name << " (" << p.age << " years old)";
		return os;
	}
};

int main() {
	Function<int, Person> f;
	f.addPair(1, {"Alice", 30});
	f.addPair(2, {"Bob", 25});
	f.addPair(3, {"Charlie", 35});

	std::cout << f[1] << std::endl;		// Output: one
	std::cout << f[2] << std::endl;		// Output: two
	std::cout << f[3] << std::endl;		// Output: three

	return 0;
}
