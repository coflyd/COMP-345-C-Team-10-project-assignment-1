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
			continent.id = continent_number++;
			iss >> continent.name >> continent.army >> continent.color;
			this->continents.push_back(std::make_unique<Continent>(continent));
			std::cout << "Continent added : " << continent.id <<" "<< continent.name 
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
			this->borders[countryId] = neighbors;
			
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

//const std::vector<int> &MapLoader::getBorders() { return borders; }
const std::vector<std::unique_ptr<Continent>> &MapLoader::getContinents() {
	return continents;
}
const std::vector<std::unique_ptr<Country>> &MapLoader::getCountries() {
	return countries;
}
const std::map<int, std::vector<int>>& MapLoader::getBorders() {
	return borders;
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
		os << "  - ID " << continent->id << " " << continent->name << " (Army: " << continent->army
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
	for (const auto &[countryId, neighbors] : mapLoader.borders) {
	        os << "  - Country ID " << countryId << " neighbors: ";
	        for (int neighborId : neighbors) {
	            os << neighborId << " ";
			}
			os << std::endl;
		}
	return os;
}

// Printing of MapLoader
void MapLoader::display() {
	std::cout << "=== Continent(s) ===" << std::endl;
	for (const auto &continent : continents) {
		std::cout << "ID: " << continent->id << ", Name: " << continent->name
				<< ", Army: " << continent->army << ", Color: "
				<< continent->color << std::endl;
	}

	std::cout << "\n=== Country ===" << std::endl;
	for (const auto &country : countries) {
		std::cout << "ID: " << country->id << ", Name: " << country->name
				  << ", Continent: " << country->continentNumber
				  << ", Coords: (" << country->x << ", " << country->y
				  << ")" << std::endl;
	}

	std::cout << "\n=== Borders ===" << std::endl;
	for (const auto &[countryId, neighbors] : borders) {
        std::cout << "Country ID " << countryId << " is connected to: ";
        for (int neighborId : neighbors) {
            std::cout << neighborId << " ";
        }
        std::cout << std::endl;
    }
}

// ======== Map ========

//Map Constructor
Map::Map(std::unique_ptr<MapLoader> mapLoader) :
	mapObjects(std::move(mapLoader)) {
		// Fill countryNode with countries
	for (const auto &country : mapObjects->getCountries()) {
		countryNode[country->id] = std::make_unique<Country>(*country);
	}

	// Build neighboursEdge from the borders
	neighboursEdge = mapObjects->getBorders();
}

// Copy constructor Map
Map::Map(const Map &other) :
		mapObjects(std::make_unique<MapLoader>(*other.mapObjects)) {
	// Deep copy countryNode
	for (const auto& [id, country] : other.countryNode) {
		countryNode[id] = std::make_unique<Country>(*country);
	}
	// Copie neighboursEdge
	neighboursEdge = other.neighboursEdge;
}

//Map assignement operator
Map &Map::operator=(const Map &other) {
	if (this != &other) {
		mapObjects = std::make_unique<MapLoader>(*other.mapObjects);
		//Deep copy countryNode
		countryNode.clear();
		for (const auto& [id, country] : other.countryNode) {
			countryNode[id] = std::make_unique<Country>(*country);
		}
		// Copy neighboursEdge
		neighboursEdge = other.neighboursEdge;
	}
	return *this;
}

// Insert operator in a stream for Map
std::ostream &operator<<(std::ostream &os, const Map &map) {
	os << "=== Map Summary ===" << std::endl;
	os << *map.mapObjects << std::endl; // Print data of MapLoader

	os << "Country Nodes (" << map.countryNode.size() << "):" << std::endl;
	for (const auto& [id, country] : map.countryNode) {
		os << "  - ID: " << id << ", Name: " << country->name << ", Continent: "
				<< country->continentNumber << ", Coords: (" << country->x
				<< ", " << country->y << ")" << std::endl;
	}

	os << "Neighbours Edge (" << map.neighboursEdge.size() << " entries):"
			<< std::endl;
	for (const auto& [id, neighbors] : map.neighboursEdge) {
		os << "  - Country ID " << id << " neighbors: ";
		for (int neighborId : neighbors) {
			os << neighborId << " ";
		}
		os << std::endl;
	}

	return os;
}

void Map::display() {
	mapObjects->display(); //Print data of MapLoader
	
	std::cout << "\n=== Country Nodes ===" << std::endl;
	for (const auto& [id, country] : countryNode) {
		std::cout << "ID: " << id << ", Name: " << country->name
				<< ", Continent: " << country->continentNumber << ", Coords: ("
				<< country->x << ", " << country->y << ")" << std::endl;
	}

	std::cout << "\n=== Neighbours Edge ===" << std::endl;
	for (const auto& [id, neighbors] : neighboursEdge) {
		std::cout << "Country ID " << id << " is connected to: ";
		for (int neighborId : neighbors) {
			std::cout << neighborId << " ";
		}
		std::cout << std::endl;
	}
}

// ====== CONNECTIVITY's GRAPH =====
bool Map::isGraphConnected() {
	if (neighboursEdge.empty()) {
		return false;
	}

	std::queue<int> queue;
	std::unordered_set<int> visited;
	
	int startCountry = neighboursEdge.begin()->first;
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
	return visited.size() == neighboursEdge.size();
}

// ====== CONNECTIVITY's SUBGRAPH =====
bool Map::isThisGraphConnected(
		const std::map<int, std::unique_ptr<Country>> &nodes,
		const std::map<int, std::vector<int>> &edges) {

	if (edges.empty() || nodes.empty()) {
		return false;
	}

	std::queue<int> queue;
	std::unordered_set<int> visited;

	int startNode = edges.begin()->first; // We start with the first node of the subgraph
	queue.push(startNode);
	visited.insert(startNode);

	while (!queue.empty()) {
		int current = queue.front();
		queue.pop();

		for (int neighbor : edges.at(current)) {	// Browse the neighbors of the current node
			
			if (nodes.find(neighbor) != nodes.end()	// Check that the neighbor exists in the subgraph
					&& visited.find(neighbor) == visited.end()) {
				visited.insert(neighbor);
				queue.push(neighbor);
			}
		}
	}

return visited.size() == nodes.size();		// The subgraph is connected if all nodes have been visited

}

// ====== CONNECTIVITY's SUBGRAPH by CONTINENT =====
bool Map::isContinentsSubGraphsConnected() {
	//Browse by all continent
	for (const auto &continent : this->mapObjects->getContinents()) {
		int currentContinentId = continent->id;

		// Extract the nodes (countries) of the continent
		std::map<int, std::unique_ptr<Country>> currentCountryNode;
		std::map<int, std::vector<int>> currentNeighboursEdge;

		// Fill in currentCountryNode and currentNeighborsEdge
		for (const auto& [id, country] : this->countryNode) {
			if (country->continentNumber == currentContinentId) {
				currentCountryNode[id] = std::make_unique<Country>(*country);
			}
		}

		// Fill currentNeighborsEdge, checking that neighbors are on the same continent
		for (const auto& [id, neighbors] : this->neighboursEdge) {
			if (currentCountryNode.find(id) != currentCountryNode.end()) {
				std::vector<int> continentNeighbors;
				for (int neighborId : neighbors) {
					if (currentCountryNode.find(neighborId)
							!= currentCountryNode.end()) {
						continentNeighbors.push_back(neighborId);
					}
				}
				currentNeighboursEdge[id] = continentNeighbors;
			}
		}

		// Check the connectivity of the subgraph
		if (!Map::isThisGraphConnected(currentCountryNode,
				currentNeighboursEdge)) {
			std::cerr << "The continent " << continent->name << " (ID: "
					<< currentContinentId << ") is not connected !"
					<< std::endl;
			return false;
		} else {
			std::cout << "The continent " << continent->name << " (ID: "
					<< currentContinentId << ") is fully connected !" << std::endl;
		}
	}
	
	return true; //all continent are connected
}
