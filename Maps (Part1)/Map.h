#ifndef MAP_H_
#define MAP_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>


struct Continent {
	int id;
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

class MapLoader {
private : 
	std::vector<std::string> files; 
	std::vector<std::unique_ptr<Continent>> continents; 
	std::vector<std::uniqu_ptr<Continent>> countries; 
	std::vector<int, std::vector<int>> borders; 

public : 
	MapLoader(const std::string &mapFileName);

	const std::vector<int, std::vector<int>> &getBorders();
	const std::vector<std::unique_ptr<Continent>> &getContinents(); 
	const std::vector<std::unique_ptr<Country>> &getCountries();

	MapLoader(const MapLoader &other);
	MapLoader &operator=(const MapLoader &other);
	friend std::ostream& operator<<(std::ostream &os, const MapLoader &map);
	void display();
};

class Map {
private:
	std::unique_ptr<MapLoader> mapObjects;
	std::map<int, std::unique_ptr<Country>> countryNode;
	std::map<int, std::vector<int>> neighboursEdge;

public:
	Map(std::unique_ptr<MapLoader> mapObjects);
	Map(const Map &other);
	Map &operator=(const Map &other);
	friend std::ostream &operator<<(std::ostream &os, const Map &map);
	void display();
	bool isGraphConnected();
	static bool isThisGraphConnected(
			const std::map<int, std::unique_ptr<Country>> &nodes,
			const std::map<int, std::vector<int>> &edges);
	bool isContinentsSubGraphsConnected();
};

#endif /* MAP_H_ */
