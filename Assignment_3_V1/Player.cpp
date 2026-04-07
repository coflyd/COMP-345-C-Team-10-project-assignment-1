#include "Player.h"
#include "PlayerStrategy.h"
#include <algorithm>

// Constructor: initializes player with a name and empty collections
Player::Player(const std::string &n) {
	name = new std::string(n);
	hand = new Hand();
	orders = new OrdersList();
	reinforcementPool = 0;
	doneIssuing = false; // Part 3
	conqueredThisTurn = false; // Part 4

}

Player::Player(const std::string &n, PlayerStrategy &playerStrategy) {
	name = new std::string(n);
	hand = new Hand();
	orders = new OrdersList();
	reinforcementPool = 0;
	doneIssuing = false; // Part 3
	conqueredThisTurn = false; // Part 4
	this->playerStrategy = &playerStrategy;

}
// Copy constructor: creates a deep copy of another player
Player::Player(const Player &other) {
	name = new std::string(*other.name);
	for (Country *country : other.owned) {
		owned.push_back(new Country(*country));
	}
	hand = new Hand(*other.hand);
	orders = new OrdersList(*other.orders);
	reinforcementPool = other.reinforcementPool;
	doneIssuing = other.doneIssuing; // Part 3
	conqueredThisTurn = other.conqueredThisTurn; // Part 4
	trucePlayers = other.trucePlayers; // Part 4
	playerStrategy = other.playerStrategy;
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
		owned.push_back(new Country(*country));
	}
	hand = new Hand(*other.hand);
	orders = new OrdersList(*other.orders);
	reinforcementPool = other.reinforcementPool;
	doneIssuing = other.doneIssuing; // Part 3
	conqueredThisTurn = other.conqueredThisTurn; // Part 4
	trucePlayers = other.trucePlayers; // Part 4
	return *this;
}

// Destructor: frees all dynamically allocated memory
Player::~Player() {
	delete name;
	// Note: owned territories are NOT deleted here because the Map owns them
	delete hand;
	delete orders;
}

// Adds a country to the player's list of owned territories
void Player::addCountry(Country *c) {
	owned.push_back(c);
}

// Part 3: Removes a country from this player's owned list (does not delete it)
void Player::removeCountry(Country *c) {
	owned.erase(std::remove(owned.begin(), owned.end(), c), owned.end());
}

// Returns list of territories to defend (weakest first - fewest armies)
std::vector<Country*> Player::toDefend() {
	return playerStrategy->toDefend();
}

// Part 3 Returns list of enemy territories adjacent to any of this player's territories
std::vector<Country*> Player::toAttack(std::vector<Player*> &allPlayers) {
	return playerStrategy->toAttack(allPlayers);
}

// Part 3 Full issueOrder with real decision logic
void Player::issueOrder( std::vector<Player*> &allPlayers,
		Deck *deck) {
	playerStrategy->issueOrder( allPlayers, deck);
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

// Part 3 getter for reinforcement pool
int Player::getReinforcementPool() const {
	return reinforcementPool;
}

// Part 3 done issuing flag getters/setters
bool Player::isDoneIssuing() const {
	return doneIssuing;
}

void Player::setDoneIssuing(bool done) {
	doneIssuing = done;
}

const std::vector<Country*>& Player::getOwnedCountries() const {
	return owned;
}

// Stream insertion operator: outputs player info
std::ostream& operator<<(std::ostream &out, const Player &p) {
	out << "Player: " << *p.name << " | Countries: " << p.owned.size()
			<< " | Reinforcements: " << p.reinforcementPool;
	return out;
}

void Player::setPlayerStrategy(PlayerStrategy *playerStrategy) {
	this->playerStrategy = playerStrategy;
}

// Part 4 additions
int Player::getReinforcements() const {
	return reinforcementPool;
}
void Player::setReinforcements(int amount) {
	reinforcementPool = amount;
}
bool Player::hasConqueredThisTurn() const {
	return conqueredThisTurn;
}
void Player::setConqueredThisTurn(bool val) {
	conqueredThisTurn = val;
}
bool Player::isTruceWith(const std::string &p) const {
	for (const std::string &t : trucePlayers)
		if (t == p)
			return true;
	return false;
}
void Player::addTruce(const std::string &p) {
	trucePlayers.push_back(p);
}

void Player::clearTruces() {
	trucePlayers.clear();
}

bool Player::getAttacked() {
	return this->attacked;
}
void Player::setAttacked(bool value) {
	this->attacked = value;
}
