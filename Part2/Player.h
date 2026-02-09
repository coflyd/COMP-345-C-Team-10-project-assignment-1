#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Map.h"

class Order;
class OrdersList;
class Hand;

class Player {
private:
    std::string name;
    std::vector<Country*> ownedTerritories;
    Hand* hand;
    OrdersList* orders;

public:
    Player(const std::string& name);
    ~Player();

    void addTerritory(Country* t);

    std::vector<Country*> toDefend();
    std::vector<Country*> toAttack();

    void issueOrder();

    void displayOwnedTerritories() const;
};

class Hand {
public:
    Hand() {}
};

class Order {
public:
    std::string description;
    Order(const std::string& desc) : description(desc) {}
};

class OrdersList {
private:
    std::vector<Order*> orders;

public:
    void addOrder(Order* o);
    void printOrders() const;
};

#endif
