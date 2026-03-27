
#include <string>

template<class T>
class maybe {
	//union {
		T value;
	//	char x;
	//};
	bool hasValue;
public:
	maybe() : hasValue(false) {}
	
};

int main() {
	maybe<std::string> c;


}
