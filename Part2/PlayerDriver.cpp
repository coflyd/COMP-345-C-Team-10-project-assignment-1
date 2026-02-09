#include "Player.h"
#include "Map.h"
#include <iostream>

using namespace std;

int main() {
    // Ensuring Data folder and Map do exist
    Map* europe = new Map("data/europe.map");

    Player* p1 = new Player("Alice");

    cout << "\nAssigning territories to Alice..." << endl;
    
    // Use the should be implemented getter to access the list of countries
    const auto& allCountries = europe->getCountries();

    for (int i = 0; i < 3 && i < allCountries.size(); i++) {
        p1->addTerritory(allCountries[i].get());
    }

    p1->displayOwnedTerritories();

    cout << "\n--- toDefend() ---" << endl;
    auto defendList = p1->toDefend();
    for (auto t : defendList) cout << "Defend: " << t->name << endl;

    cout << "\n--- toAttack() ---" << endl;
    auto attackList = p1->toAttack();
    for (auto t : attackList) cout << "Attack: " << t->name << endl;

    cout << "\n--- issueOrder() ---" << endl;
    p1->issueOrder();
    p1->issueOrder();
    p1->issueOrder();

    cout << "\n--- OrdersList ---" << endl;
    p1->issueOrder();

    // Clean up memory
    delete europe;
    delete p1;

    return 0;
}