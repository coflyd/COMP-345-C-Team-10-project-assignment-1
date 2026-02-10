#include "Cards.h"
using namespace std;

Card:: Card(){
    type=CardType::bomb;
}
Card::Card(CardType t){
    type=t;
 }
 Card::Card(const Card& other){
     type=other.type;
 }
 Card& Card::operator=(const Card& other){
     if(this !=&other){
         type=other.type;
     }
     return *this;
 }
 Card::~Card(){

 }
void Card:: play(Player* player, Deck* deck){
    if (deck == nullptr) return;
            deck->returnCard(this);
}
 CardType Card::getType() const{
     return type;
 }



Deck::Deck(){
    cards=new vector<Card*>();
}

Deck::Deck(const Deck& other){
    cards= new vector<Card*>;
    for (Card* c: *other.cards){
        cards->push_back(new Card(*c));
    }
}

Deck& Deck::operator=(const Deck& other){
    if (this != &other) {
        for (Card* c : *cards) {
            delete c;
        }
        cards->clear();
        for (Card* c : *other.cards){
            cards->push_back(new Card(*c));
        }
    }
    return *this;
}
Deck::~Deck(){
    for (Card* c: *cards){
        delete c;
    }
    delete cards;
}

Card* Deck::draw(){
    if (cards->empty()) return nullptr;

        Card* top = cards->back();
        cards->pop_back();
            return top;
}
void Deck::returnCard(Card* card){
    cards->push_back(card);
}
int Deck::size()const{
    return cards->size();
}





Hand::Hand(){
    cards=new vector<Card*>();

}
Hand::Hand(const Hand& other){
    cards = new std::vector<Card*>();
        for (Card* c : *other.cards) {
            cards->push_back(new Card(*c));
        }
}
Hand& Hand::operator=(const Hand& other){
    if (this != &other) {
            for (Card* c : *cards) {
                delete c;
            }
            cards->clear();

            for (Card* c : *other.cards) {
                cards->push_back(new Card(*c));
            }
        }
        return *this;
}
Hand::~Hand(){
    for (Card* c : *cards) {
            delete c;
        }
        delete cards;
}

void Hand::addCard(Card* card){
    if (card == nullptr) return;
        cards->push_back(card);
}
bool Hand::removeCard(Card* card){
    if (card == nullptr) return false;
    
    for (auto it = cards->begin(); it != cards->end(); it++) {
        if (*it == card) {
            cards->erase(it);
            return true;
        }
    }
    return false;
}
const vector<Card*>* Hand::getCards() const {
    return cards;
}

int Hand::size() const{
    return cards->size();
}

