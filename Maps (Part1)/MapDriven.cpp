#include "Map.h"
#include <iostream>
using namespace std;

int main() {
	Map *europe = new Map("data/europe.map");
	europe->display();
	return 0;
}
