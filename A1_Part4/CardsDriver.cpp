#include <iostream>
#include "Cards.h"
#include "Player.h"
#include "Orders.h"

using namespace std;

int main() {

    Deck deck;
    Player player("Joe");

    // Fill deck with 10 cards, 2 of each type
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
    
    cout << "Initial deck size: " << deck.size() << "\n";
    cout << "Initial hand size: " << player.getHand()->size() << "\n";
    cout << "Initial orders size: " << player.getOrders()->size() << "\n\n";

    cout << "Drawing 5 cards into player's hand\n";
    for (int i = 0; i < 5; i++) {
        Card* c = deck.draw();
        
        //  "if (c)" makes sure that the c getting drawn is not a nullptr
        if (c) player.getHand()->addCard(c);
    }

    cout << "After draw deck size: " << deck.size() << "\n";
    cout << "After draw hand size: " << player.getHand()->size() << "\n";
    cout << "After draw orders size: " << player.getOrders()->size() << "\n\n";

    cout << "Playing all cards\n";
    
    while (player.getHand()->size() > 0) {
        
        //  Returning the top card from the player's hand, which is the last card in the vector
        Card* c = player.getHand()->getCards()->back();
        c->play(&player, &deck);
    }

    cout << "After play deck size: " << deck.size() << "\n";
    cout << "After play hand size: " << player.getHand()->size() << "\n";
    cout << "After play orders size: " << player.getOrders()->size() << "\n\n";

    cout << "OrdersList content:\n";
    cout << *player.getOrders() << "\n";

    return 0;
}
