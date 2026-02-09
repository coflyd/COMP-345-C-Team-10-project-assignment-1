#include "Map.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

using namespace std;

Map::Map(const std::string &mapFileName) {
	std::ifstream mapFile(mapFileName);
	if (!mapFile.is_open()) {
		std::cerr << "Error: Unable to open the file " << mapFileName << std::endl;
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
			std::cout << "\n--- Start of section : [" << currentSection << "] ---" << std::endl;
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
			std::cout << "Continent added : " << continent.name << " (army: " << continent.army << ", color: " << continent.color << ")" << std::endl;

		} else if (currentSection == "countries") {
			std::istringstream iss(line);
			Country country;
			iss >> country.id >> country.name >> country.continentNumber >>
				country.x >> country.y;
			this->countries.push_back(std::make_unique<Country>(country));
			std::cout << "Country added : " << country.id << " - " << country.name << " (continent: " << country.continentNumber << ", contact details: " << country.x << ", " << country.y << ")" << std::endl;

		} else if (currentSection == "borders") {
			std::istringstream iss(line);
			int countryId;
			iss >> countryId;
			std::vector<int> neighbors;
			int neighborId;
			while (iss >> neighborId) {
				neighbors.push_back(neighborId);
			}
			this->countriesAdjGraph[countryId] = neighbors;
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

void Map::display() {
	// Print the continents
	std::cout << "=== Continent(s) ===" << std::endl;
	for (const auto &continent : continents) {
		std::cout << "Name: " << continent->name
				  << ", Army: " << continent->army
				  << ", Colors: " << continent->color << std::endl;
	}

	// Print the countries
	std::cout << "\n=== Pays ===" << std::endl;
	for (const auto &country : countries) {
		std::cout << "ID: " << country->id << ", Name: " << country->name
				  << ", Continent: " << country->continentNumber
				  << ", Coordonates: (" << country->x << ", " << country->y
				  << ")" << std::endl;
	}

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
