#include "Map.h"
#include <iostream>
using namespace std;

int main() {
	auto mapLoader = std::make_unique<MapLoader>("data/europe_map/europe.map");
	Map europe(std::move(mapLoader));
	europe.display();
	count << europe << endl; 
	if(europe.isGraphConnected()){
		cout <<"Map is connected"<<endl; 
	}else{
		cerr <<"Map is NOT connected"<<endl; 
	}
	return 0;
}
