#include <atomic>
#include <iostream>
#include <vector>

class Building
{
   public:
	std::string name;
	float		happiness;

	Building(const std::string &name, float happiness)
		: name(name), happiness(happiness)
	{
	}

	virtual float	  trigger_effect(float h) const = 0;
	virtual Building *clone() const					= 0;
	virtual ~Building()								= default;

	virtual void print(std::ostream &os) const
	{
		os << "Building: " << name << ", Happiness: " << happiness;
	}
};

class Mayor
{
   public:
	std::string name;
	float		happiness;

	Mayor(const std::string &name, float happiness)
		: name(name), happiness(happiness)
	{
	}

	virtual float influence(const Building **buildings, int num_buildings) const
	{
		return 0;
	}
	virtual ~Mayor() = default;
	virtual void print(std::ostream &os) const
	{
		os << "Mayor: " << name << ", Happiness: " << happiness;
	}
};

class Park : public Building
{
	using Building::Building;
	virtual float trigger_effect(float h) const override { return h; }
	Park		 *clone() const override { return new Park(*this); }
	void		  print(std::ostream &os) const override
	{
		os << "Park: " << name << ", Happiness: " << happiness;
	}
};
class School : public Building
{
	using Building::Building;
	virtual float trigger_effect(float h) const override { return h; }
	School		 *clone() const override { return new School(*this); }
	void		  print(std::ostream &os) const override
	{
		os << "School: " << name << ", Happiness: " << happiness;
	}
};
class Hospital : public Building
{
	using Building::Building;
	virtual float trigger_effect(float h) const override { return h; }
	Hospital	 *clone() const override { return new Hospital(*this); }
	void		  print(std::ostream &os) const override
	{
		os << "Hospital: " << name << ", Happiness: " << happiness;
	}
};

class Municipality : public Building
{
	std::vector<Building *> buildings;
	Mayor				   *mayor;

	void clearB()
	{
		for (Building *b : buildings)
		{
			delete b;
		}
		buildings.clear();
	}
	void copyB(const Municipality &other)
	{
		for (Building *b : other.buildings)
		{
			buildings.push_back(b->clone());
		}
	}

   public:
	Municipality(const std::string &name, float happiness, Mayor *mayor)
		: Building(name, happiness), mayor(mayor)
	{
	}

	Municipality(const Municipality &other)
		: Building(other), mayor(other.mayor)
	{
		copyB(other);
	}

	Municipality &operator=(const Municipality &other)
	{
		clearB();
		copyB(other);
		mayor = other.mayor;
		return *this;
	}

	~Municipality() { clearB(); }

	void add_building(const Building &building)
	{
		if (dynamic_cast<const Municipality *>(&building))
		{
			std::cerr << "Cannot add building to municipality: " << name
					  << std::endl;
			return;
		}
		// if (typeid(building) == typeid(const Municipality &)) {
		//   std::cerr << "Cannot add building to municipality: " << name <<
		//   std::endl; return;
		// }
		buildings.push_back(building.clone());
	}

	virtual float trigger_effect(float h) const override { return h; }

	Municipality *clone() const override { return new Municipality(*this); }

	void print(std::ostream &os) const override
	{
		os << "Municipality: " << name << ", Happiness: " << happiness
		   << ", Mayor: " << mayor->name << "\nBuildings:\n";
		for (const Building *b : buildings)
		{
			b->print(os);
			os << "\n";
		}
	}
};

class City
{
	std::vector<Municipality *> municipalities;

	void clearM()
	{
		for (Municipality *m : municipalities)
		{
			delete m;
		}
		municipalities.clear();
	}
	void copyM(const City &other)
	{
		for (Municipality *m : other.municipalities)
		{
			municipalities.push_back(m->clone());
		}
	}

   public:
	void add_municipality(const Municipality &municipality)
	{
		municipalities.push_back(municipality.clone());
	}

	City() = default;
	~City() { clearM(); }

	City(const City &other) { copyM(other); }

	City &operator=(const City &other)
	{
		clearM();
		copyM(other);
		return *this;
	}

	void print(std::ostream &os) const
	{
		os << "City:\n";
		for (const Municipality *m : municipalities)
		{
			m->print(os);
			os << "\n";
		}
	}
};
