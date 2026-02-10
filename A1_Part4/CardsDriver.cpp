
#include <iostream>
#include "Cards.h"

using namespace std;
int main() {
    Deck deck;
    Hand hand;
    
    deck.returnCard(new Card(CardType::bomb));
    deck.returnCard(new Card(CardType::reinforcement));
    deck.returnCard(new Card(CardType::blockade));
    deck.returnCard(new Card(CardType::airlift));
    deck.returnCard(new Card(CardType::diplomacy));
    deck.returnCard(new Card(CardType::bomb));
    deck.returnCard(new Card(CardType::reinforcement));
    deck.returnCard(new Card(CardType::blockade));
    deck.returnCard(new Card(CardType::airlift));
    deck.returnCard(new Card(CardType::diplomacy));
    
    cout << "Deck size after creation: " << deck.size() << std::endl;

    for (int i = 0; i < 5; i++) {
            Card* c = deck.draw();
                hand.addCard(c);
        }
    std::cout << "Deck size after drawing: " << deck.size() << std::endl;
    std::cout << "Hand size after drawing: " << hand.size() << std::endl;

    while (hand.size() > 0) {
            Card* c = hand.getCards()->back();
            hand.removeCard(c);
            c->play(nullptr, &deck);
        }

        std::cout << "Hand size after playing: " << hand.size() << std::endl;
        std::cout << "Deck size after returning: " << deck.size() << std::endl;

    
    return 0;
}
