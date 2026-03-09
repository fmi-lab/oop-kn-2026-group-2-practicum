#include <cstring>
#include <iostream>

class Str {
	char *str;

   public:
	Str() : str(nullptr) {}
	Str(const char *s) : str(new char[strlen(s) + 1]) { std::strcpy(str, s); }
	Str(const Str &other) : Str(new char[strlen(other.str) + 1]) { std::strcpy(str, other.str); }
	Str &operator=(const Str &other) {
		if (this != &other) {
			char *new_str = new char[strlen(other.str) + 1];
			std::strcpy(new_str, other.str);
			delete[] str;
			str = new_str;
		}
		return *this;
	}

	~Str() { delete[] str; }

	const char *c_str() const { return str; }
	char	   *c_str() { return str; }

	static Str cat(const Str &s1, const Str &s2) {
		char *buffer = new char[strlen(s1.str) + strlen(s2.str) + 1];
		strcpy(buffer, s1.str);
		strcat(buffer, s2.str);
		Str result;
		result.str = buffer;
		return result;
	}

	static int cmp(const Str &s1, const Str &s2) { return std::strcmp(s1.str, s2.str); }

	int cmp(const Str &other) const { return Str::cmp(*this, other); }

	Str cat(const Str &other) const { return Str::cat(*this, other); }

	std::size_t length() const { return std::strlen(str); }
};

int main() {
	Str str1("Hello"), str2("World");

	std::cout << str1.c_str() << " " << str2.c_str() << std::endl;	   // Hello World

	std::cout << Str::cat(str1, str2).c_str() << std::endl;		// HelloWorld

	std::cout << Str::cmp(str1, str2) << std::endl;		// <0

	str1 = str1.cat(str2);
	std::cout << str1.c_str() << std::endl;		// HelloWorld

	std::cout << str1.cmp(str2) << std::endl;	  // <0

	return 0;
}
