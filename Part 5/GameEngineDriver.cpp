#include <iostream>
#include <string>
#include "GameEngine.h"

int main() {
    GameEngine* engine = new GameEngine();
    std::string input;
    std::cout << "=== Warzone Game Engine ===\n";
    while (true) {
        std::cout << "\nCurrent State: "
                  << engine->getCurrentState()->getName()
                  << "\n";
        std::cout << "Enter command: ";
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << "Empty command. Try again.\n";
            continue;
        }
        engine->transition(input);
        std::cout << "\n--- Engine Status ---\n";
        std::cout << *engine << "\n";
        if (engine->getCurrentState()->getName() == "end") {
            std::cout << "\nGame ended.\n";
            break;
        }
    }
    delete engine;
    return 0;
}
