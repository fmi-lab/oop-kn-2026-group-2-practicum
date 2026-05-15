#include "cities.hpp"
#include <iostream>

int main()
{
	Mayor mayor("Alice", 10.0f);

	Park   park("Central Park", 5.0f);
	School school("Greenwood School", 3.0f);

	Municipality municipality("Downtown", 20.0f, &mayor);
	municipality.add_building(park);
	municipality.add_building(school);

	City city;
	city.add_municipality(municipality);
	city.add_municipality(municipality);

	city.print(std::cout);
}
