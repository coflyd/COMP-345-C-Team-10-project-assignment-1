#include "Map.h"
#include <iostream>
using namespace std;

int main() {
	//auto mapLoader = std::make_unique<MapLoader>("data/europe_map/europe.map");
	auto mapLoader = std::make_unique<MapLoader>("data/big_europe_map/bigeurope.map");
	Map europe(std::move(mapLoader));
	europe.display();
	cout << europe<<endl;
	if (europe.isGraphConnected()){
		cout << "Map is connected"<<endl;
	}else{
		cerr << "Map is not connected"<<endl;
	}
	cout << "\nVerification by continent"<<endl;
	europe.isContinentsSubGraphsConnected();
	return 0;
}
