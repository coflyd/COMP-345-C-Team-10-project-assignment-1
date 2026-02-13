#include "Map.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {

	std::vector<std::unique_ptr<Player>> players;
	for(int i = 1;i<=4;i++){
		players.push_back(std::make_unique<Player>(i, "player_"+std::to_string(i)));
	}
	for (const auto &p:players){
		cout << "Player " << p->getId()<< ", pseudo : "<<p->getPseudo()<<std::endl;
	}
	auto mapLoader = std::make_unique<MapLoader>("data/europe_map/europe.map");
	Map europe(std::move(mapLoader));
	europe.initialCountryDistribution(players);
	europe.display();
	cout << europe<<endl;

	europe.validate();
	
	return (0);
}
