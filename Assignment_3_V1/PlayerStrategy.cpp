/*
 * PlayerStrategy.cpp
 *
 *  Created on: 21 mars 2026
 *      Author: yves
 */
#include <algorithm>
#include "PlayerStrategy.h"
#include "Player.h"

HumanPlayerStrategy::HumanPlayerStrategy(Player *p, Map * m) :
		PlayerStrategy(p,m)  {
}
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *p, Map * m) :
		PlayerStrategy(p,m) {
}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *p, Map * m) :
		PlayerStrategy(p,m) {
}
NeutralPlayerStrategy::NeutralPlayerStrategy(Player *p, Map * m) :
		PlayerStrategy(p,m) {
}
CheaterPlayerStrategy::CheaterPlayerStrategy(Player *p, Map * m) :
		PlayerStrategy(p,m) {
}

/*********************************************************
 * HumanPlayerStrategy
 *********************************************************/
void HumanPlayerStrategy::issueOrder(std::vector<Player*> &allPlayers,
		Deck *deck) {
	int choice;
	std::cout
			<< "Que voulez-vous faire? (1: Déployer, 2: Avancer, 3: Attaquer, 4: Jouer une carte): ";
	std::cin >> choice;
	// Logique basée sur le choix de l'utilisateur
}

std::vector<Country*> HumanPlayerStrategy::toDefend() {
	std::vector<Country*> defend = player->getOwnedCountries();
	std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
		return a->armies < b->armies;
	});
	return defend;
}

std::vector<Country*> HumanPlayerStrategy::toAttack(
		std::vector<Player*> &allPlayers) {
	std::vector<Country*> targets;
	for (Player *other : allPlayers) {
		if (other == player)
			continue;
		for (Country *c : other->getOwnedCountries()) {
			if (map->isAdjacentToAnyOwnedCountry(c,
					player->getOwnedCountries())) {
				targets.push_back(c);
			}
		}
	}
	return targets;
}

/*********************************************************
 * AggressivePlayerStrategy
 *********************************************************/
void AggressivePlayerStrategy::issueOrder(
		std::vector<Player*> &allPlayers, Deck *deck) {
	// Phase A Deploy
	if (player->getReinforcementPool() > 0) {
		std::vector<Country*> defend = toDefend();
		if (!defend.empty()) {
			Country *target = defend.back(); // strongest territory
			int armiesToDeploy = player->getReinforcementPool();
			player->getOrders()->add(new Deploy(target->name, armiesToDeploy));
			std::cout << "[" << player->getName() << "] Deploy order: "
					<< armiesToDeploy << " armies -> " << target->name << "\n";
			player->setReinforcementPool(0);
		}
		return;
	}

	// Phase B Advance to defend
	std::vector<Country*> defend = toDefend();
	if (defend.size() >= 2) {
		Country *weakest = defend[0];
		Country *strongest = defend.back();
		int available = strongest->armies;
		if (available > 1) {
			int toMove = available / 2;
			player->getOrders()->add(
					new Advance(strongest->name, weakest->name, toMove));
			std::cout << "[" << player->getName() << "] Advance (defend): "
					<< toMove << " armies " << strongest->name << " -> "
					<< weakest->name << "\n";
		}
	}

	// Phase C Advance to attack
	std::vector<Country*> attackTargets = toAttack(allPlayers);
	for (Country *target : attackTargets) {
		Country *source = map->findStrongestAdjacentCountry(target,
				player->getOwnedCountries());
		if (source != nullptr && source->armies > 1) {
			player->getOrders()->add(
					new Advance(source->name, target->name,
							source->armies - 1));
			std::cout << "[" << player->getName() << "] Advance (attack): "
					<< source->armies - 1 << " armies " << source->name
					<< " -> " << target->name << "\n";
		}
	}

	// Phase D Play one card
	const std::vector<Card*> *cards = player->getHand()->getCards();
	if (cards != nullptr && !cards->empty() && deck != nullptr) {
		Card *card = (*cards)[0];
		std::cout << "[" << player->getName() << "] Playing card: " << *card
				<< "\n";
		card->play(player, deck);
	}

	player->setDoneIssuing(true);
	std::cout << "[" << player->getName() << "] Done issuing orders.\n";
}

std::vector<Country*> AggressivePlayerStrategy::toDefend() {
	std::vector<Country*> defend = player->getOwnedCountries();
	std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
		return a->armies > b->armies;
	});
	return defend;
}

std::vector<Country*> AggressivePlayerStrategy::toAttack(
		std::vector<Player*> &allPlayers) {
	std::vector<Country*> targets;
	for (Player *other : allPlayers) {
		if (other == player)
			continue;
		for (Country *c : other->getOwnedCountries()) {
			if (map->isAdjacentToAnyOwnedCountry(c,
					player->getOwnedCountries())) {
				targets.push_back(c);
			}
		}
	}
	return targets;
}

/*********************************************************
 * BenevolentPlayerStrategy
 *********************************************************/
void BenevolentPlayerStrategy::issueOrder(
		std::vector<Player*> &allPlayers, Deck *deck) {
	// Phase A Deploy
	if (player->getReinforcementPool() > 0) {
		std::vector<Country*> defend = toDefend();
		if (!defend.empty()) {
			Country *target = defend.front(); // weakest territory
			int armiesToDeploy = player->getReinforcementPool();
			player->getOrders()->add(new Deploy(target->name, armiesToDeploy));
			std::cout << "[" << player->getName() << "] Deploy order: "
					<< armiesToDeploy << " armies -> " << target->name << "\n";
			player->setReinforcementPool(0);
		}
		return;
	}

	// Phase B Advance to defend
	std::vector<Country*> defend = toDefend();
	if (defend.size() >= 2) {
		Country *weakest = defend[0];
		Country *strongest = defend.back();
		int available = strongest->armies;
		if (available > 1) {
			int toMove = available / 2;
			player->getOrders()->add(
					new Advance(strongest->name, weakest->name, toMove));
			std::cout << "[" << player->getName() << "] Advance (defend): "
					<< toMove << " armies " << strongest->name << " -> "
					<< weakest->name << "\n";
		}
	}

	// Phase D Play one card
	const std::vector<Card*> *cards = player->getHand()->getCards();
	if (cards != nullptr && !cards->empty() && deck != nullptr) {
		Card *card = (*cards)[0];
		std::cout << "[" << player->getName() << "] Playing card: " << *card
				<< "\n";
		card->play(player, deck);
	}

	player->setDoneIssuing(true);
	std::cout << "[" << player->getName() << "] Done issuing orders.\n";
}

std::vector<Country*> BenevolentPlayerStrategy::toDefend() {
	std::vector<Country*> defend = player->getOwnedCountries();
	std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
		return a->armies < b->armies;
	});
	return defend;
}

std::vector<Country*> BenevolentPlayerStrategy::toAttack(
		std::vector<Player*> &allPlayers) {
	return {};
}

/*********************************************************
 * NeutralPlayerStrategy
 *********************************************************/
void NeutralPlayerStrategy::issueOrder(
		std::vector<Player*> &allPlayers, Deck *deck) {
	if (player->getAttacked() == true) {
		player->setPlayerStrategy(new AggressivePlayerStrategy(player, map));
	}

	player->setDoneIssuing(true);
	std::cout << "[" << player->getName() << "] Done issuing orders.\n";
}

std::vector<Country*> NeutralPlayerStrategy::toDefend() {
	std::vector<Country*> defend = player->getOwnedCountries();
	std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
		return a->armies < b->armies;
	});
	return defend;
}

std::vector<Country*> NeutralPlayerStrategy::toAttack(
		std::vector<Player*> &allPlayers) {
	return {};
}

/*********************************************************
 * CheaterPlayerStrategy
 *********************************************************/
void CheaterPlayerStrategy::issueOrder(
		std::vector<Player*> &allPlayers, Deck *deck) {
	std::vector<Country*> attackTargets = toAttack(allPlayers);
	for (Country *target : attackTargets) {
		Country *source = map->findStrongestAdjacentCountry(target,
				player->getOwnedCountries());
		if (source != nullptr) {
			player->getOrders()->add(
					new Advance(source->name, target->name, 1000));
			std::cout << "[" << player->getName() << "] Cheat: Conquer "
					<< target->name << " from " << source->name << "\n";
		}
	}

	player->setDoneIssuing(true);
	std::cout << "[" << player->getName() << "] Done issuing orders.\n";
}

std::vector<Country*> CheaterPlayerStrategy::toDefend() {
	std::vector<Country*> defend = player->getOwnedCountries();
	std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
		return a->armies < b->armies;
	});
	return defend;
}

std::vector<Country*> CheaterPlayerStrategy::toAttack(
		std::vector<Player*> &allPlayers) {
	std::vector<Country*> targets;
	for (Player *other : allPlayers) {
		if (other == player)
			continue;
		for (Country *c : other->getOwnedCountries()) {
			if (map->isAdjacentToAnyOwnedCountry(c,
					player->getOwnedCountries())) {
				targets.push_back(c);
			}
		}
	}
	return targets;
}

/**
 * Phase A Deploy all armies first (no other orders while pool > 0)
 * Phase B Advance to defend (move armies between own territories)
 * Phase C Advance to attack (move armies to enemy territory)
 * Phase D Play one card from hand
 * Then mark doneIssuing = true
 */
/*
 void HumanPlayerStrategy::issueOrder(Map *map, std::vector<Player*> &allPlayers,
 Deck *deck) {
 // Phase A Deploy
 if (player->getReinforcementPool() > 0) {
 std::vector<Country*> defend = toDefend();
 if (!defend.empty()) {
 Country *target = defend[0]; // weakest territory
 int armiesToDeploy = player->getReinforcementPool();

 // Add armies to territory
 target->armies += armiesToDeploy;

 player->getOrders()->add(new Deploy(target->name, armiesToDeploy));
 std::cout << "[" << player->getName() << "] Deploy order: "
 << armiesToDeploy << " armies -> " << target->name << "\n";
 player->setReinforcementPool(0);
 // reinforcementPool = 0;
 }
 // Must finish deploying before any other order
 return;
 }
 // Phase B Advance to defend
 std::vector<Country*> defend = toDefend();
 if (defend.size() >= 2) {
 Country *weakest = defend[0];
 Country *strongest = defend[defend.size() - 1];
 int available = strongest->armies;
 if (available > 1) {
 int toMove = available / 2;
 player->getOrders()->add(
 new Advance(strongest->name, weakest->name, toMove));
 std::cout << "[" << player->getName() << "] Advance (defend): "
 << toMove << " armies " << strongest->name << " -> "
 << weakest->name << "\n";
 }
 }

 // Phase C Advance to attack
 std::vector<Country*> attackTargets = toAttack(allPlayers);
 if (!attackTargets.empty()) {
 // Find our strongest territory to attack from
 Country *source = nullptr;
 int maxArmies = 0;
 for (Country *mine : player->getOwnedCountries()) {
 int armies = mine->armies;
 if (armies > maxArmies) {
 maxArmies = armies;
 source = mine;
 }
 }
 if (source != nullptr && maxArmies > 1) {
 Country *target = attackTargets[0];
 int toSend = maxArmies - 1; // keep 1 army back
 player->getOrders()->add(
 new Advance(source->name, target->name, toSend));
 std::cout << "[" << player->getName() << "] Advance (attack): "
 << toSend << " armies " << source->name << " -> "
 << target->name << "\n";
 }
 }

 // Phase D Play one card
 const std::vector<Card*> *cards = player->getHand()->getCards();
 if (cards != nullptr && !cards->empty() && deck != nullptr) {
 Card *card = (*cards)[0];
 std::cout << "[" << player->getName() << "] Playing card: " << *card
 << "\n";
 card->play(player, deck);
 }

 // Done issuing for this turn
 player->setDoneIssuing(true);
 std::cout << "[" << player->getName() << "] Done issuing orders.\n";
 }

 std::vector<Country*> HumanPlayerStrategy::toDefend() {
 std::vector<Country*> defend = player->toDefend();
 std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
 return (a->armies < b->armies);
 });
 return (defend);
 }

 std::vector<Country*> HumanPlayerStrategy::toAttack(
 std::vector<Player*> &allPlayers) {
 std::vector<Country*> targets;
 for (Player *other : allPlayers) {
 if (other == this->player)
 continue;
 const std::vector<Country*> &enemyOwned = other->getOwnedCountries();
 for (Country *c : enemyOwned) {
 targets.push_back(c);
 }
 }
 return targets;
 }

 void AggressivePlayerStrategy::issueOrder(Map *map,
 std::vector<Player*> &allPlayers, Deck *deck) {
 // Phase A Deploy
 if (player->getReinforcementPool() > 0) {
 std::vector<Country*> defend = toDefend();
 if (!defend.empty()) {
 Country *target = defend[0]; // weakest territory
 int armiesToDeploy = player->getReinforcementPool();

 // Add armies to territory
 target->armies += armiesToDeploy;

 player->getOrders()->add(new Deploy(target->name, armiesToDeploy));
 std::cout << "[" << player->getName() << "] Deploy order: "
 << armiesToDeploy << " armies -> " << target->name << "\n";

 player->setReinforcements(0);
 }
 // Must finish deploying before any other order
 return;
 }
 // Phase B Advance to defend
 std::vector<Country*> defend = toDefend();
 if (defend.size() >= 2) {
 Country *weakest = defend[0];
 Country *strongest = defend[defend.size() - 1];
 int available = strongest->armies;
 if (available > 1) {
 int toMove = available / 2;
 player->getOrders()->add(
 new Advance(strongest->name, weakest->name, toMove));
 std::cout << "[" << player->getName() << "] Advance (defend): "
 << toMove << " armies " << strongest->name << " -> "
 << weakest->name << "\n";
 }
 }

 // Phase C Advance to attack
 std::vector<Country*> attackTargets = toAttack(allPlayers);
 if (!attackTargets.empty()) {
 // Find our strongest territory to attack from
 Country *source = nullptr;
 int maxArmies = 0;
 for (Country *mine : player->getOwnedCountries()) {
 int armies = mine->armies;
 if (armies > maxArmies) {
 maxArmies = armies;
 source = mine;
 }
 }
 if (source != nullptr && maxArmies > 1) {
 Country *target = attackTargets[0];
 int toSend = maxArmies - 1; // keep 1 army back
 player->getOrders()->add(
 new Advance(source->name, target->name, toSend));
 std::cout << "[" << player->getName() << "] Advance (attack): "
 << toSend << " armies " << source->name << " -> "
 << target->name << "\n";
 }
 }

 // Phase D Play one card
 const std::vector<Card*> *cards = player->getHand()->getCards();
 if (cards != nullptr && !cards->empty() && deck != nullptr) {
 Card *card = (*cards)[0];
 std::cout << "[" << player->getName() << "] Playing card: " << *card
 << "\n";
 card->play(player, deck);
 }

 // Done issuing for this turn
 player->setDoneIssuing(true);
 std::cout << "[" << player->getName() << "] Done issuing orders.\n";
 }

 std::vector<Country*> AggressivePlayerStrategy::toDefend() {
 std::vector<Country*> defend = player->toDefend();
 std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
 return a->armies < b->armies;
 });
 return (defend);
 }

 std::vector<Country*> AggressivePlayerStrategy::toAttack(
 std::vector<Player*> &allPlayers) {
 std::vector<Country*> targets;
 for (Player *other : allPlayers) {
 if (other == this->player)
 continue;
 const std::vector<Country*> &enemyOwned = other->getOwnedCountries();
 for (Country *c : enemyOwned) {
 targets.push_back(c);
 }
 }
 return targets;
 }

 void BenevolentPlayerStrategy::issueOrder(Map *map,
 std::vector<Player*> &allPlayers, Deck *deck) {
 // Phase A Deploy
 if (player->getReinforcementPool() > 0) {
 std::vector<Country*> defend = toDefend();
 if (!defend.empty()) {
 Country *target = defend[0]; // weakest territory
 int armiesToDeploy = player->getReinforcementPool();

 // Add armies to territory
 target->armies += armiesToDeploy;

 player->getOrders()->add(new Deploy(target->name, armiesToDeploy));
 std::cout << "[" << player->getName() << "] Deploy order: "
 << armiesToDeploy << " armies -> " << target->name << "\n";

 player->setReinforcements(0);
 }
 // Must finish deploying before any other order
 return;
 }
 // Phase B Advance to defend
 std::vector<Country*> defend = toDefend();
 if (defend.size() >= 2) {
 Country *weakest = defend[0];
 Country *strongest = defend[defend.size() - 1];
 int available = strongest->armies;
 if (available > 1) {
 int toMove = available / 2;
 player->getOrders()->add(
 new Advance(strongest->name, weakest->name, toMove));
 std::cout << "[" << player->getName() << "] Advance (defend): "
 << toMove << " armies " << strongest->name << " -> "
 << weakest->name << "\n";
 }
 }

 // Phase C Advance to attack
 std::vector<Country*> attackTargets = toAttack(allPlayers);
 if (!attackTargets.empty()) {
 // Find our strongest territory to attack from
 Country *source = nullptr;
 int maxArmies = 0;
 for (Country *mine : player->getOwnedCountries()) {
 int armies = mine->armies;
 if (armies > maxArmies) {
 maxArmies = armies;
 source = mine;
 }
 }
 if (source != nullptr && maxArmies > 1) {
 Country *target = attackTargets[0];
 int toSend = maxArmies - 1; // keep 1 army back
 player->getOrders()->add(
 new Advance(source->name, target->name, toSend));
 std::cout << "[" << player->getName() << "] Advance (attack): "
 << toSend << " armies " << source->name << " -> "
 << target->name << "\n";
 }
 }

 // Phase D Play one card
 const std::vector<Card*> *cards = player->getHand()->getCards();
 if (cards != nullptr && !cards->empty() && deck != nullptr) {
 Card *card = (*cards)[0];
 std::cout << "[" << player->getName() << "] Playing card: " << *card
 << "\n";
 card->play(player, deck);
 }

 // Done issuing for this turn
 player->setDoneIssuing(true);
 std::cout << "[" << player->getName() << "] Done issuing orders.\n";
 }

 std::vector<Country*> BenevolentPlayerStrategy::toDefend() {
 std::vector<Country*> defend = player->toDefend();
 std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
 return a->armies < b->armies;
 });
 return (defend);
 }

 std::vector<Country*> BenevolentPlayerStrategy::toAttack(
 std::vector<Player*> &allPlayers) {
 std::vector<Country*> targets;
 for (Player *other : allPlayers) {
 if (other == this->player)
 continue;
 const std::vector<Country*> &enemyOwned = other->getOwnedCountries();
 for (Country *c : enemyOwned) {
 targets.push_back(c);
 }
 }
 return targets;
 }

 void NeutralPlayerStrategy::issueOrder(Map *map,
 std::vector<Player*> &allPlayers, Deck *deck) {
 // Phase A Deploy
 if (player->getReinforcementPool() > 0) {
 std::vector<Country*> defend = toDefend();
 if (!defend.empty()) {
 Country *target = defend[0]; // weakest territory
 int armiesToDeploy = player->getReinforcementPool();

 // Add armies to territory
 target->armies += armiesToDeploy;

 player->getOrders()->add(new Deploy(target->name, armiesToDeploy));
 std::cout << "[" << player->getName() << "] Deploy order: "
 << armiesToDeploy << " armies -> " << target->name << "\n";

 player->setReinforcements(0);
 }
 // Must finish deploying before any other order
 return;
 }
 // Phase B Advance to defend
 std::vector<Country*> defend = toDefend();
 if (defend.size() >= 2) {
 Country *weakest = defend[0];
 Country *strongest = defend[defend.size() - 1];
 int available = strongest->armies;
 if (available > 1) {
 int toMove = available / 2;
 player->getOrders()->add(
 new Advance(strongest->name, weakest->name, toMove));
 std::cout << "[" << player->getName() << "] Advance (defend): "
 << toMove << " armies " << strongest->name << " -> "
 << weakest->name << "\n";
 }
 }

 // Phase C Advance to attack
 std::vector<Country*> attackTargets = toAttack(allPlayers);
 if (!attackTargets.empty()) {
 // Find our strongest territory to attack from
 Country *source = nullptr;
 int maxArmies = 0;
 for (Country *mine : player->getOwnedCountries()) {
 int armies = mine->armies;
 if (armies > maxArmies) {
 maxArmies = armies;
 source = mine;
 }
 }
 if (source != nullptr && maxArmies > 1) {
 Country *target = attackTargets[0];
 int toSend = maxArmies - 1; // keep 1 army back
 player->getOrders()->add(
 new Advance(source->name, target->name, toSend));
 std::cout << "[" << player->getName() << "] Advance (attack): "
 << toSend << " armies " << source->name << " -> "
 << target->name << "\n";
 }
 }

 // Phase D Play one card
 const std::vector<Card*> *cards = player->getHand()->getCards();
 if (cards != nullptr && !cards->empty() && deck != nullptr) {
 Card *card = (*cards)[0];
 std::cout << "[" << player->getName() << "] Playing card: " << *card
 << "\n";
 card->play(player, deck);
 }

 // Done issuing for this turn
 player->setDoneIssuing(true);
 std::cout << "[" << player->getName() << "] Done issuing orders.\n";
 }

 std::vector<Country*> NeutralPlayerStrategy::toDefend() {
 std::vector<Country*> defend = player->toDefend();
 std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
 return a->armies < b->armies;
 });
 return (defend);
 }

 std::vector<Country*> NeutralPlayerStrategy::toAttack(
 std::vector<Player*> &allPlayers) {
 std::vector<Country*> targets;
 for (Player *other : allPlayers) {
 if (other == this->player)
 continue;
 const std::vector<Country*> &enemyOwned = other->getOwnedCountries();
 for (Country *c : enemyOwned) {
 targets.push_back(c);
 }
 }
 return targets;
 }

 void CheaterPlayerStrategy::issueOrder(Map *map,
 std::vector<Player*> &allPlayers, Deck *deck) {
 // Phase A Deploy
 if (player->getReinforcementPool() > 0) {
 std::vector<Country*> defend = toDefend();
 if (!defend.empty()) {
 Country *target = defend[0]; // weakest territory
 int armiesToDeploy = player->getReinforcementPool();

 // Add armies to territory
 target->armies += armiesToDeploy;

 player->getOrders()->add(new Deploy(target->name, armiesToDeploy));
 std::cout << "[" << player->getName() << "] Deploy order: "
 << armiesToDeploy << " armies -> " << target->name << "\n";

 player->setReinforcements(0);
 }
 // Must finish deploying before any other order
 return;
 }
 // Phase B Advance to defend
 std::vector<Country*> defend = toDefend();
 if (defend.size() >= 2) {
 Country *weakest = defend[0];
 Country *strongest = defend[defend.size() - 1];
 int available = strongest->armies;
 if (available > 1) {
 int toMove = available / 2;
 player->getOrders()->add(
 new Advance(strongest->name, weakest->name, toMove));
 std::cout << "[" << player->getName() << "] Advance (defend): "
 << toMove << " armies " << strongest->name << " -> "
 << weakest->name << "\n";
 }
 }

 // Phase C Advance to attack
 std::vector<Country*> attackTargets = toAttack(allPlayers);
 if (!attackTargets.empty()) {
 // Find our strongest territory to attack from
 Country *source = nullptr;
 int maxArmies = 0;
 for (Country *mine : player->getOwnedCountries()) {
 int armies = mine->armies;
 if (armies > maxArmies) {
 maxArmies = armies;
 source = mine;
 }
 }
 if (source != nullptr && maxArmies > 1) {
 Country *target = attackTargets[0];
 int toSend = maxArmies - 1; // keep 1 army back
 player->getOrders()->add(
 new Advance(source->name, target->name, toSend));
 std::cout << "[" << player->getName() << "] Advance (attack): "
 << toSend << " armies " << source->name << " -> "
 << target->name << "\n";
 }
 }

 // Phase D Play one card
 const std::vector<Card*> *cards = player->getHand()->getCards();
 if (cards != nullptr && !cards->empty() && deck != nullptr) {
 Card *card = (*cards)[0];
 std::cout << "[" << player->getName() << "] Playing card: " << *card
 << "\n";
 card->play(player, deck);
 }

 // Done issuing for this turn
 player->setDoneIssuing(true);
 std::cout << "[" << player->getName() << "] Done issuing orders.\n";
 }

 std::vector<Country*> CheaterPlayerStrategy::toDefend() {
 std::vector<Country*> defend = player->toDefend();
 std::sort(defend.begin(), defend.end(), [](Country *a, Country *b) {
 return a->armies < b->armies;
 });
 return (defend);
 }

 std::vector<Country*> CheaterPlayerStrategy::toAttack(
 std::vector<Player*> &allPlayers) {
 std::vector<Country*> targets;
 for (Player *other : allPlayers) {
 if (other == this->player)
 continue;
 const std::vector<Country*> &enemyOwned = other->getOwnedCountries();
 for (Country *c : enemyOwned) {
 targets.push_back(c);
 }
 }
 return targets;
 }
 */
