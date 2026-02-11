// ========================= OrdersDriver.cpp =========================
// Quick driver to show:
// - create each order
// - add to list
// - move + remove
// - execute to show effects

#include <iostream>
#include "Orders.h"

int main() {
    OrdersList list;

    // Add one of each order type
    list.add(new Deploy("Alaska", 5));
    list.add(new Advance("Alaska", "Kamchatka", 3));
    list.add(new Bomb("Brazil"));
    list.add(new Blockade("Peru"));
    list.add(new Airlift("Ontario", "Quebec", 7));
    list.add(new Negotiate("Player1", "Player2"));

    std::cout << "=== Initial ===\n" << list << "\n";

    // Move last to first
    list.move(5, 0);
    std::cout << "=== After move(5->0) ===\n" << list << "\n";

    // Remove something in the middle
    list.remove(2);
    std::cout << "=== After remove(2) ===\n" << list << "\n";

    // Execute a couple so the effect prints
    if (list.at(0)) list.at(0)->execute();
    if (list.at(1)) list.at(1)->execute();

    std::cout << "=== After execute some ===\n" << list << "\n";

    // Deep copy test
    OrdersList copy = list;
    if (copy.at(0)) copy.at(0)->execute();

    std::cout << "=== Original ===\n" << list << "\n";
    std::cout << "=== Copy ===\n" << copy << "\n";

    return 0;
}
