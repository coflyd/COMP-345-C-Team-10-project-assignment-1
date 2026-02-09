#include "Player.h"
#include <iostream>
// UNFINISHED CODE *ATTACKS PLAYER'S OWN COUNTRIES* *HAS PROBLEMS WITH MAP.H (NEEDS GETTER METHOD FOR MAP)
using namespace std;

Player::Player(const std::string& name)
    : name(name)
{
    hand = new Hand();
    orders = new OrdersList();
}

Player::~Player() {
    delete hand;
    delete orders;
}

void Player::addTerritory(Country* t) {
    ownedTerritories.push_back(t);
}

vector<Country*> Player::toDefend() {
    cout << name << " deciding territories to DEFEND..." << endl;
    return ownedTerritories; // arbitrary choice
}

vector<Country*> Player::toAttack() {
    cout << name << " deciding territories to ATTACK..." << endl;

    vector<Country*> attackList;

    // Attacks first 2 territories if available
    for (int i = 0; i < 2 && i < ownedTerritories.size(); i++) {
        attackList.push_back(ownedTerritories[i]);
    }

    return attackList;
}

void Player::issueOrder() {
    cout << name << " is issuing an order..." << endl;
    Order* newOrder = new Order("Dummy order issued");
    orders->addOrder(newOrder);
}

void Player::displayOwnedTerritories() const {
    cout << name << " owns:" << endl;
    for (auto t : ownedTerritories) {
        cout << " - " << t->name << endl;
    }
}

void OrdersList::addOrder(Order* o) {
    orders.push_back(o);
}

void OrdersList::printOrders() const {
    cout << "Orders list:" << endl;
    for (auto o : orders) {
        cout << " - " << o->description << endl;
    }
}
