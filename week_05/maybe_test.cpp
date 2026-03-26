
#include <iostream>
#include "someclass.hpp"
#include "maybe.hpp"
//#include "maybe_full.hpp"

int main() {

	//Maybe<SomeClass> maybeValue(42);
	//if (maybeValue.hasValue()) {
	//	std::cout << "Value: " << maybeValue.getValue().getValue() << std::endl;
	//} else {
	//	std::cout << "No value present" << std::endl;
	//}

	//Maybe<SomeClass> emptyValue;
	//if (emptyValue.hasValue()) {
	//	std::cout << "Value: " << emptyValue.getValue().getValue() << std::endl;
	//} else {
	//	std::cout << "No value present" << std::endl;
	//}

	//Maybe<std::string> maybeString("Hello, Maybe!");
	//maybeString = Maybe<std::string>();

	//print("---------------------------");

	//maybeValue = Maybe<SomeClass>(100);

	//print("---------------------------");
	//
	//print("---------------------------");
	//Maybe<SomeClass> copyValue{ 55 };
	//print("---------------------------");
	//Maybe<SomeClass> assignedValue{ copyValue };


	//print("---------------------------");
	//assignedValue = std::move(copyValue);

	//print("---------------------------");


	Maybe<SomeClass> empty;
	Maybe<SomeClass> value(123);
	print("---------------------------");
	empty = value;
	print("---------------------------");
	Maybe<SomeClass> copyConstructed(empty);
	print("---------------------------");


	return 0;
}
