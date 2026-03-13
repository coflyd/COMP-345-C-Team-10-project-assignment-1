#include "Player.h"

// Creates a default deploy order and adds it to the player's order list
void Player::issueOrder() {
	orders->add(new Deploy("Default", 5));
	std::cout << "Order issued for " << *name << std::endl;
}

// Constructor: initializes player with a name and empty collections
Player::Player(const std::string &n) {
	name = new std::string(n);
	hand = new Hand();
	orders = new OrdersList();
	reinforcementPool = 0;
}

// Copy constructor: creates a deep copy of another player
Player::Player(const Player &other) {
	name = new std::string(*other.name);
	for (Country *country : other.owned) {
		owned.push_back(new Country(*country)); // Copie profonde
	}
	hand = new Hand(*other.hand);
	orders = new OrdersList(*other.orders);
	reinforcementPool = other.reinforcementPool;
}

// Assignment operator: deep copy with cleanup of existing data
Player& Player::operator=(const Player &other) {
	if (this == &other)
		return *this;

	delete name;
	for (Country *country : owned) {
		delete country;
	}
	owned.clear();
	delete hand;
	delete orders;

	name = new std::string(*other.name);
	for (Country *country : other.owned) {
		owned.push_back(new Country(*country)); // Copie profonde
	}
	hand = new Hand(*other.hand);
	orders = new OrdersList(*other.orders);
	reinforcementPool = other.reinforcementPool;
	return *this;
}

// Destructor: frees all dynamically allocated memory
Player::~Player() {
	delete name;
	for (Country *country : owned) {
		delete country;
	}
	delete hand;
	delete orders;
}

// Adds a country to the player's list of owned territories
void Player::addCountry(Country *c) {
	owned.push_back(c);
}

// Returns list of territories to defend
std::vector<Country*> Player::toDefend() {
	return owned;
}

// Returns list of territories to attack
std::vector<Country*> Player::toAttack() {
	return std::vector<Country*>();
}

// Returns the player's list of orders
OrdersList* Player::getOrders() const {
	return orders;
}

// Returns the player's name
std::string Player::getName() const {
	return *name;
}

// Returns the player's hand of cards
Hand* Player::getHand() const {
	return hand;
}

void Player::setReinforcementPool(int rp) {
	this->reinforcementPool = rp;
}

const std::vector<Country*>& Player::getOwnedCountries() const {
	return owned;
}
// Stream insertion operator: outputs player info
std::ostream& operator<<(std::ostream &out, const Player &p) {
	out << "Player: " << *p.name << " | Countries: " << p.owned.size();
	return out;
}
