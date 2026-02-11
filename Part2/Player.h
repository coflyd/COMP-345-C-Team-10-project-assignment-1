#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Map.h"
#include "Orders.h"

class Player {
private:
    std::string* name;
    std::vector<Country*>* owned;
    void* hand;
    OrdersList* orders;

public:
    Player(const std::string& n);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    void addCountry(Country* c);
    std::vector<Country*> toDefend();
    std::vector<Country*> toAttack();
    void issueOrder();

    OrdersList* getOrders() const;
    std::string getName() const;

    friend std::ostream& operator<<(std::ostream& out, const Player& p);
};