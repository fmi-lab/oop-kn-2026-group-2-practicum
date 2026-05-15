#include <iostream>

#include "lazymap.hpp"

int square(int x) { return x * x; }

int main()
{
	std::vector<int> v = {1, 2, 3, 4};

	// for(int x : LazyMap<int, int>(v, square)) {
	//	std::cout << x << std::endl;
	// }

	auto container = LazyMap<int, int>(v, square);
	for (auto it = container.begin(); it != container.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	return 0;
}
