#pragma once
#include <vector>
#include "Map.h"
#include "Player.h"
#include "Cards.h"

class PlayerStrategy {
protected:
	Player *player;
	Map *map;
public:
	PlayerStrategy(Player *p, Map *m) :
			player(p) ,map(m){
	}
	virtual std::vector<Country*> toDefend()=0;
	virtual std::vector<Country*> toAttack(std::vector<Player*> &allPlayers)=0; // Part 3
	virtual void issueOrder( std::vector<Player*> &allPlayers,
			Deck *deck)=0; // Part 3
	virtual ~PlayerStrategy() = default;
};

class HumanPlayerStrategy: public PlayerStrategy {
public:
	HumanPlayerStrategy(Player *p, Map *);
	std::vector<Country*> toDefend() override;
	std::vector<Country*> toAttack(std::vector<Player*> &allPlayers) override;
	void issueOrder(std::vector<Player*> &allPlayers, Deck *deck)
			override;
};

class AggressivePlayerStrategy: public PlayerStrategy {
public:
	AggressivePlayerStrategy(Player *p, Map *);
	std::vector<Country*> toDefend() override;
	std::vector<Country*> toAttack(std::vector<Player*> &allPlayers) override;
	void issueOrder(std::vector<Player*> &allPlayers, Deck *deck)
			override;
};

class BenevolentPlayerStrategy: public PlayerStrategy {
public:
	BenevolentPlayerStrategy(Player *p, Map *);
	std::vector<Country*> toDefend() override;
	std::vector<Country*> toAttack(std::vector<Player*> &allPlayers) override;
	void issueOrder( std::vector<Player*> &allPlayers, Deck *deck)
			override;
};

class NeutralPlayerStrategy: public PlayerStrategy {
public:
	NeutralPlayerStrategy(Player *p, Map *);
	std::vector<Country*> toDefend() override;
	std::vector<Country*> toAttack(std::vector<Player*> &allPlayers) override;
	void issueOrder(std::vector<Player*> &allPlayers, Deck *deck)
			override;
};

class CheaterPlayerStrategy: public PlayerStrategy {
public:
	CheaterPlayerStrategy(Player *p, Map *);
	std::vector<Country*> toDefend() override;
	std::vector<Country*> toAttack(std::vector<Player*> &allPlayers) override;
	void issueOrder( std::vector<Player*> &allPlayers, Deck *deck)
			override;
};
