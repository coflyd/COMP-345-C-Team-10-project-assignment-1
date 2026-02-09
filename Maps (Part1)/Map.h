#ifndef MAP_H_
#define MAP_H_

#include <map>
#include <string>
#include <vector>
#include <memory>


struct Continent {
	std::string name;
	int army;
	std::string color;
};

struct Country {
	int id;
	std::string name;
	int continentNumber;
	int x;
	int y;
};

class Map {
private:
	std::vector<std::string> files;
	std::vector<std::unique_ptr<Continent>> continents;
	std::vector<std::unique_ptr<Country>> countries;
	std::vector<int> borders;
	std::map<int, std::vector<int>> countriesAdjGraph;

public:
	Map(const std::string &mapFileName);
	void display(); 
};

#endif /* MAP_H_ */
