#include "Map.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <queue> //verify the connectivity
#include <sstream>
#include <unordered_set>
#include <utility>

using namespace std;

// ====== MAPLOADER =======

MapLoader::MapLoader(const std::string &mapFileName) {
	std::ifstream mapFile(mapFileName);
	if (!mapFile.is_open()) {
		std::cerr << "Error: Unable to open the file " << mapFileName 
			<< std::endl;
		return;
	}

	std::cout << "Opening the file: " << mapFileName << std::endl;

	std::string line;
	std::string currentSection;

	while (getline(mapFile, line)) {
		if (!line.empty() && line.back() == '\r') {
			line.pop_back(); // For files in Windows format (\r\n)
		}
		if (!line.empty() && line.back() == '\n') {
			line.pop_back(); // For files in Unix format (\n)
		}
		std::cout << "Ligne lue : " << line << std::endl; // Debug
		// Ignore comments and blank lines
		if (line.empty() || line[0] == ';' || line[0] == ' ') {
			continue;
		}

		// Section detection
		if (line[0] == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.size() - 2);
			std::cout << "\n--- Start of section : [" << currentSection
					  << "] ---" << std::endl;
			continue;
		}

		// Processing of sections
		if (currentSection == "files") {
			this->files.push_back(line);
			std::cout << "File added : " << line << std::endl;

		} else if (currentSection == "continents") {
			std::istringstream iss(line);
			Continent continent;
			iss >> continent.name >> continent.army >> continent.color;
			this->continents.push_back(std::make_unique<Continent>(continent));
			std::cout << "Continent added : " << continent.name 
					  << " (army: " << continent.army 
					  << ", color: " << continent.color << ")" << std::endl;
		} else if (currentSection == "countries") {
			std::istringstream iss(line);
			Country country;
			iss >> country.id >> country.name >> country.continentNumber >>
				country.x >> country.y;
			this->countries.push_back(std::make_unique<Country>(country));
			std::cout << "Country added : " << country.id << " - " << country.name 
				<< " (continent: " << country.continentNumber 
				<< ", contact details: " << country.x << ", " << country.y 
				<< ")" << std::endl;
		} else if (currentSection == "borders") {
			std::istringstream iss(line);
			int countryId;
			iss >> countryId;
			std::vector<int> neighbors;
			int neighborId;
			while (iss >> neighborId) {
				neighbors.push_back(neighborId);
			}
			this->borders.insert(this->borders.end(), neighbors.begin(),neighbors.end());
			std::cout << "Connections added for country ID " << countryId << " : ";
			for (int neighbor : neighbors) {
				std::cout << neighbor << " ";
			}
			std::cout << std::endl;
		}
	}

	mapFile.close();
	std::cout << "\nClosing the file. Loading complete." << std::endl;
}

const std::vector<int> &MapLoader::getBorders() { return borders; }
const std::vector<std::unique_ptr<Continent>> &MapLoader::getContinents() {
	return continents;
}
const std::vector<std::unique_ptr<Country>> &MapLoader::getCountries() {
	return countries;
}

//Copy constructor MapLoader
MapLoader::MapLoader(const MapLoader &other) : 
	files(other.files), borders(other.borders){
	//Copy continents
	for (const auto &continent : other.continents) {
			continents.push_back(std::make_unique<Continent>(*continent));
		}
	//Copy countries
	for (const auto &country : other.countries) {
		countries.push_back(std::make_unique<Country>(*country));
	}
}

//MapLoader assigment operator
MapLoader &MapLoader::operator=(const MapLoader &other) {
	if(this != &other){
		files = other.files;
		borders = other.borders;
		continents.clear(); 
		countries.clear();
		//Deep copy continents
		for (const auto &continent : other.continents) {
			continents.push_back(std::make_unique<Continent>(*continent));
		}
		//Deep copy countries
		for (const auto &country : other.countries) {
			countries.push_back(std::make_unique<Country>(*country));
		}
	}
	return *this;
}

//Insertion operator in a stream for MapLoader
std::ostream &operator<<(std::ostream &os, const MapLoader &mapLoader) {
	os << "=== MapLoader Summary ===" << std::endl;
	os << "Files: ";
	for (const auto &file : mapLoader.files) {
		os << file << " ";
	}
	os << std::endl;

	os << "Continents (" << mapLoader.continents.size() << "):" << std::endl;
	for (const auto &continent : mapLoader.continents) {
		os << "  - " << continent->name << " (Army: " << continent->army
		   << ", Color: " << continent->color << ")" << std::endl;
	}

	os << "Countries (" << mapLoader.countries.size() << "):" << std::endl;
	for (const auto &country : mapLoader.countries) {
		os << "  - " << country->id << ": " << country->name
		   << " (Continent: " << country->continentNumber
		   << ", Coords: " << country->x << ", " << country->y << ")"
		   << std::endl;
	}

	os << "Borders (" << mapLoader.borders.size() << " entries):" << std::endl;
	for (size_t i = 0; i < mapLoader.borders.size(); ++i) {
		os << "  - " << mapLoader.borders[i] << " ";
	}
	os << std::endl;

	return os;
}

// Printing of MapLoader
void MapLoader::display() {
	std::cout << "=== Continent(s) ===" << std::endl;
	for (const auto &continent : continents) {
		std::cout << "Nom: " << continent->name
				  << ", Armée: " << continent->army
				  << ", Couleur: " << continent->color << std::endl;
	}

	std::cout << "\n=== Pays ===" << std::endl;
	for (const auto &country : countries) {
		std::cout << "ID: " << country->id << ", Nom: " << country->name
				  << ", Continent: " << country->continentNumber
				  << ", Coordonnées: (" << country->x << ", " << country->y
				  << ")" << std::endl;
	}

	std::cout << "\n=== Frontières ===" << std::endl;
	for (size_t i = 0; i < borders.size(); ++i) {
		std::cout << borders[i] << " ";
	}
	std::cout << std::endl;
}

// ======== Map ========

//Map Constructor
Map::Map(std::unique_ptr<MapLoader> mapLoader) :
	mapObjects(std::move(mapLoader)) {
	// Construction of the adjacency graph from the boundaries
	const std::vector<int> &borders = mapObjects->getBorders();
	for (size_t i = 0; i < borders.size(); i += 2) {
	    int country1 = borders[i];
	    int country2 = borders[i + 1];
	    countriesAdjGraph[country1].push_back(country2);
	    countriesAdjGraph[country2].push_back(country1);
	}
}

//Copy constructor
Map::Map(const Map &other) :
	mapObjects(std::make_unique<MapLoader>(*other.mapObjects)),
	countriesAdjGraph(other.countriesAdjGraph) {}

//Map assignement operator
Map &Map::operator=(const Map &other) {
	if (this != &other) {
		mapObjects = std::make_unique<MapLoader>(*other.mapObjects);
		countriesAdjGraph = other.countriesAdjGraph;
	}
	return *this;
}

// Insert operator in a stream for Map
std::ostream &operator<<(std::ostream &os, const Map &map) {
	os << "=== Map Summary ===" << std::endl;
	os << *map.mapObjects << std::endl; // Print data of MapLoader

	os << "Adjacency Graph (" << map.countriesAdjGraph.size()
	   << " entries):" << std::endl;
	for (const auto &[countryId, neighbors] : map.countriesAdjGraph) {
		os << "  - Country ID " << countryId << " neighbors: ";
		for (int neighborId : neighbors) {
			os << neighborId << " ";
		}
		os << std::endl;
	}

	return os;
}

void Map::display() {
	mapObjects->display(); //Print data of MapLoader
	
	// Print the connection between the countries
	std::cout << "\n=== Connections ===" << std::endl;
	for (const auto &[countryId, neighbors] : countriesAdjGraph) {
		std::cout << "Pays ID " << countryId << " is connected to : ";
		for (int neighborId : neighbors) {
			std::cout << neighborId << " ";
		}
		std::cout << std::endl;
	}
}

// ====== CONNECTIVITY's GRAPH =====
bool Map::isGraphConnected() {
	if (countriesAdjGraph.empty()) {
		return false;
	}

	std::queue<int> queue;
	std::unordered_set<int> visited;
	
	int startCountry = countriesAdjGraph.begin()->first;
	queue.push(startCountry);
	visited.insert(startCountry);

	while (!queue.empty()) {
		int current = queue.front();
		queue.pop();

		for (int neighbor : countriesAdjGraph[current]) {
			if (visited.find(neighbor) == visited.end()) {
				visited.insert(neighbor);
				queue.push(neighbor);
			}
		}
	}
	//Check all coutries has been visited
	return visited.size() == countriesAdjGraph.size();
}



