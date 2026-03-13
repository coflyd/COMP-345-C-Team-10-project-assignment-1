#include "GameEngine.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <random>

using namespace std;

// State

const std::array<std::string, 8> State::allowedStates = { "start", "maploaded",
		"mapvalidated", "playersadded", "assignreinforcement", "issueorders",
		"executeorders", "win" };

State::State(const std::string &n) :
		name(n) {
}

State::State(const State &other) :
		name(other.name) {
}

State& State::operator=(const State &other) {
	if (this != &other) {
		name = other.name;
	}
	return *this;
}

std::string State::getName() const {
	return name;
}

std::ostream& operator<<(std::ostream &os, const State &s) {
	os << s.name;
	return os;
}

// Command

const std::array<std::string, 10> Command::allowedCommands = { "loadmap",
		"validatemap", "addplayer", "gamestart", "issueorder", "endissueorders",
		"execorder", "win", "play", "end" };

Command::Command(const std::string &cmdType) :
		type(cmdType), effect("") {
}

Command::Command(const Command &other) :
		type(other.type), effect(other.effect) {
}

Command& Command::operator=(const Command &other) {
	if (this != &other) {
		type = other.type;
		effect = other.effect;
	}
	return *this;
}

std::string Command::getType() const {
	return type;
}

std::string Command::getEffect() const {
	return effect;
}

void Command::saveEffect(const std::string &e) {
	effect = e;
}

std::ostream& operator<<(std::ostream &os, const Command &c) {
	os << "Command: " << c.type << " | Effect: " << c.effect;
	return os;
}

// WarZone

WarZone::WarZone() :
		map(nullptr), ol(nullptr), deck(new Deck()) {
}

WarZone::WarZone(const WarZone &other) :
		map(other.map ? new Map(*other.map) : nullptr), ol(
				other.ol ? new OrdersList(*other.ol) : nullptr), deck(
				other.deck ? new Deck(*other.deck) : nullptr) {
	for (Player *player : other.players) {
		players.push_back(new Player(*player));
	}
}

WarZone::~WarZone() {
	delete map;
	delete ol;
	delete deck;
	for (Player *player : players) {
		delete player;
	}
}

// GameEngine

const std::string GameEngine::MAPS_DIRECTORY = "data";

GameEngine::GameEngine() :
		currentState("start"), warZone(new WarZone()) {
	transitions = { { { "start", "loadmap" }, "maploaded" }, { { "maploaded",
			"validatemap" }, "mapvalidated" }, {
			{ "mapvalidated", "addplayer" }, "playersadded" }, { {
			"playersadded", "gamestart" }, "assignreinforcement" }, { {
			"assignreinforcement", "issueorder" }, "issueorders" }, { {
			"issueorders", "endissueorders" }, "executeorders" }, { {
			"executeorders", "execorder" }, "executeorders" }, { {
			"executeorders", "win" }, "win" }, { { "win", "play" }, "start" } };
	commandProcessor = new CommandProcessor(this);
}

GameEngine::GameEngine(const GameEngine &other) :
		currentState(other.currentState), transitions(other.transitions), warZone(
				new WarZone(*other.warZone)) {
	commandProcessor = new CommandProcessor(this);
}

GameEngine& GameEngine::operator=(const GameEngine &other) {
	if (this != &other) {
		currentState = other.currentState;
		transitions = other.transitions;
		delete warZone;
		warZone = new WarZone(*other.warZone);
		delete commandProcessor;
		commandProcessor = new CommandProcessor(this);
	}
	return *this;
}

GameEngine::~GameEngine() {
	delete warZone;
	delete commandProcessor;
}

bool GameEngine::isValidTransition(const std::string &commandType) const {
	auto it = transitions.find( { currentState.getName(), commandType });
	return it != transitions.end();
}

void GameEngine::transition(const std::string &commandType) {
	if (!commandProcessor->validate(commandType, currentState)) {
		std::cout << "Invalid command -- " << commandType
				<< " -- for current state : " << currentState << endl;
		return;
	}

	auto it = transitions.find( { currentState.getName(), commandType });
	if (it != transitions.end()) {
		if (commandType == "loadmap") {
			cout << "Executing loadmap command" << endl;
			this->loadmap();
		} else if (commandType == "validatemap") {
			cout << "Executing validatemap command" << endl;
			this->validatemap();
		} else if (commandType == "addplayer") {
			cout << "Executing addplayer command" << endl;
			this->addplayer();
		} else if (commandType == "gamestart") {
			cout << "Executing gamestart command" << endl;
			this->gamestart();
		} else if (commandType == "issueorder") {
			cout << "Executing issueorder command" << endl;
		} else if (commandType == "endissueorders") {
			cout << "Executing endissueorders command" << endl;
		} else if (commandType == "execorder") {
			cout << "Executing execorder command" << endl;
		} else if (commandType == "win") {
			cout << "Executing win command" << endl;
		} else if (commandType == "play") {
			cout << "Executing play command" << endl;
		}

		if (commandProcessor->saveCommand(commandType)) {
			currentState = State(it->second);
			commandProcessor->getCommand()->saveEffect(
					"Transitioned to " + it->second);
		}
	} else {
		std::cout << "Invalid transition for current state.\n";
	}
}

State GameEngine::getCurrentState() const {
	return currentState;
}

void GameEngine::setCommandProcessor(CommandProcessor *processor) {
	if (commandProcessor != processor) {
		delete commandProcessor;
		commandProcessor = processor;
	}
}

CommandProcessor* GameEngine::getCommandProcessor() const {
	return commandProcessor;
}

std::ostream& operator<<(std::ostream &os, const GameEngine &ge) {
	os << "Current State: " << ge.currentState << "\n";
	os << "Commands history:\n";
	for (Command *c : ge.commandProcessor->getCommands()) {
		os << *c << "\n";
	}
	return os;
}

void GameEngine::startupPhase() {
	cout << "=== Phase startup begin ===" << endl;
	this->transition("loadmap");
	cout << *(warZone->map) << endl;
	this->transition("validatemap");
	this->transition("addplayer");
	this->transition("gamestart");
	cout << "=== Phase startup ended ===" << endl;
}

void GameEngine::loadmap() {
	string mapFilePath = MapLoader::selectMapFile(MAPS_DIRECTORY);
	MapLoader *mapLoader = new MapLoader(mapFilePath);
	cout << "Phase mapLoader ended" << endl;
	warZone->map = new Map(mapLoader);
	delete mapLoader;
	cout << "Phase mapCreation ended" << endl;
}

void GameEngine::validatemap() {
	warZone->map->validate();
}

void GameEngine::addplayer() {
	int numPlayers;
	while (true) {
		std::cout << "Enter the number of players (2 to 6): ";
		std::cin >> numPlayers;

		if (std::cin.fail() || numPlayers < 2 || numPlayers > 6) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout
					<< "Invalid input. Please enter a number between 2 and 6.\n";
		} else {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
	}

	for (Player *player : warZone->players) {
		delete player;
	}
	warZone->players.clear();

	for (int i = 0; i < numPlayers; ++i) {
		std::string name;
		std::cout << "Enter the name of player " << (i + 1) << ": ";
		std::getline(std::cin, name);
		warZone->players.push_back(new Player(name));
	}
}

void GameEngine::gamestart() {
	initialRandomCountryDistribution();
	displayOwnedCountries();
	cout << "Phase initialRandomCountryDistribution ended" << endl;
	std::cout << "\nGame order before random :\n";
	for (Player *player : warZone->players) {
		std::cout << player->getName() << "\n";
	}
	std::random_device rd;
	std::mt19937 genAleat(rd());
	std::shuffle(warZone->players.begin(), warZone->players.end(), genAleat);
	cout << "Phase Randomize order between Players ended" << endl;
	std::cout << "\nGame order after random :\n";
	for (Player *player : warZone->players) {
		std::cout << player->getName() << "\n";
	}
	for (Player *player : warZone->players) {
		player->setReinforcementPool(50);
	}
	cout << "Phase to give 50 initial armies to the players ended" << endl;
	createInitialDeck(4, 4, 4, 4, 14, 18);
	cout << "Phase to generate the 44 cards deck ended" << endl;
	drawInitialCards(2);
	cout << "Phase to draw 2 cards to all players ended" << endl;
}

void GameEngine::initialRandomCountryDistribution() {
	int nbCountries = warZone->map->getCountriesNumber();
	int nbPlayers = warZone->players.size();
	std::vector<int> countryIdBag;
	for (int i = 1; i <= nbCountries; i++) {
		countryIdBag.push_back(i);
	}
	std::shuffle(countryIdBag.begin(), countryIdBag.end(), std::mt19937 {
			std::random_device { }() });
	int i = 0;
	for (int idCountry : countryIdBag) {
		Country *countryPtr = warZone->map->getCountryById(idCountry);
		if (countryPtr) {
			warZone->players[i % nbPlayers]->addCountry(countryPtr);
		}
		i++;
	}
}

void GameEngine::createInitialDeck(int bo, int bl, int ai, int di, int re,
		int te) {
	for (int i = 0; i < bo; i++) {
		warZone->deck->returnCard(new Card(CardType::bomb));
	}
	for (int i = 0; i < bl; i++) {
		warZone->deck->returnCard(new Card(CardType::blockade));
	}
	for (int i = 0; i < ai; i++) {
		warZone->deck->returnCard(new Card(CardType::airlift));
	}
	for (int i = 0; i < di; i++) {
		warZone->deck->returnCard(new Card(CardType::diplomacy));
	}
	for (int i = 0; i < re; i++) {
		warZone->deck->returnCard(new Card(CardType::reinforcement));
	}
	vector<int> territoriesId;
	for (int i = 1; i <= warZone->map->getCountriesNumber(); i++) {
		territoriesId.push_back(i);
	}
	std::random_device rd;
	std::mt19937 genAleat(rd());
	std::shuffle(territoriesId.begin(), territoriesId.end(), genAleat);

	for (int i = 0; i < te; i++) {
		Card *card = new Card(CardType::territory);
		card->setTerritory(territoriesId[i],
				warZone->map->getCountryName(territoriesId[i]));
		warZone->deck->returnCard(card);
	}
	warZone->deck->shuffle();
}

void GameEngine::displayOwnedCountries() {
	for (Player *player : warZone->players) {
		std::cout << "Countries owned by " << player->getName() << " : "
				<< std::endl;
		for (Country *country : player->getOwnedCountries()) {
			std::cout << "- " << country->name << std::endl;
		}
	}
}

void GameEngine::drawInitialCards(int nbCards) {
	for (Player *player : warZone->players) {
		cout << "\nPlayer " << player->getName() << endl;
		cout << "Initial deck size: " << warZone->deck->size() << "\n";
		cout << "Initial hand size: " << player->getHand()->size() << "\n";
		cout << "Initial orders size: " << player->getOrders()->size()
				<< "\n\n";

		cout << "Drawing " << nbCards << " cards into player's hand\n";
		for (int i = 0; i < nbCards; i++) {
			Card *c = warZone->deck->draw();
			if (c) {
				player->getHand()->addCard(c);
			}
		}

		cout << "After draw deck size: " << warZone->deck->size() << "\n";
		cout << "After draw hand size: " << player->getHand()->size() << "\n";
		cout << "After draw orders size: " << player->getOrders()->size()
				<< "\n\n";
	}
}

// CommandProcessor

CommandProcessor::CommandProcessor() :
		gameEngine(nullptr) {
}

CommandProcessor::CommandProcessor(GameEngine *engine) :
		gameEngine(engine) {
	cout << "Command processor created" << endl;
}

CommandProcessor::CommandProcessor(const CommandProcessor &other) :
		gameEngine(other.gameEngine) {
	for (Command *cmd : other.commands) {
		commands.push_back(new Command(*cmd));
	}
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor &other) {
	if (this != &other) {
		gameEngine = other.gameEngine;
		for (Command *cmd : commands) {
			delete cmd;
		}
		commands.clear();
		for (Command *cmd : other.commands) {
			commands.push_back(new Command(*cmd));
		}
	}
	return *this;
}

CommandProcessor::~CommandProcessor() {
	for (Command *cmd : commands) {
		delete cmd;
	}
}

void CommandProcessor::readCommand() {
	std::string inputCommand;
	std::cout << "Enter command: ";
	std::getline(std::cin, inputCommand);
	if (inputCommand.empty()) {
		return;
	}
	if (std::find(Command::allowedCommands.begin(),
			Command::allowedCommands.end(), inputCommand)
			== Command::allowedCommands.end()) {
		std::cout << "\nInvalid command: " << inputCommand << ", try again.\n";
	} else {
		this->saveCommand(inputCommand);
	}
}

bool CommandProcessor::saveCommand(const std::string &commandType) {
	if (std::find(Command::allowedCommands.begin(),
			Command::allowedCommands.end(), commandType)
			!= Command::allowedCommands.end()) {
		commands.push_back(new Command(commandType));
		return true;
	}
	return false;
}

Command* CommandProcessor::getCommand() {
	if (!commands.empty()) {
		return commands.back();
	}
	return nullptr;
}

std::string CommandProcessor::getCommandFromConsole() {
	readCommand();
	if (!commands.empty()) {
		return commands.back()->getType();
	}
	return "";
}

bool CommandProcessor::validate(const std::string &commandType,
		const State &currentState) const {
	if (std::find(Command::allowedCommands.begin(),
			Command::allowedCommands.end(), commandType)
			== Command::allowedCommands.end()) {
		return false;
	}
	if (gameEngine != nullptr && !gameEngine->isValidTransition(commandType)) {
		return false;
	}
	return true;
}

const std::vector<Command*>& CommandProcessor::getCommands() const {
	return commands;
}

std::ostream& operator<<(std::ostream &os, const CommandProcessor &cp) {
	os << "CommandProcessor history:\n";
	for (Command *c : cp.commands) {
		os << *c << "\n";
	}
	return os;
}

// FileLineReader

FileLineReader::FileLineReader(const std::string &filename) {
	file.open(filename);
	if (!file.is_open()) {
		throw std::runtime_error(
				"Impossible d'ouvrir le fichier : " + filename);
	}
}

FileLineReader::~FileLineReader() {
	if (file.is_open()) {
		file.close();
	}
}

std::string FileLineReader::readLine() {
	std::string line;
	if (std::getline(file, line)) {
		return line;
	}
	return "";
}

const std::ifstream& FileLineReader::getFile() const {
	return file;
}

// FileCommandProcessorAdapter

FileCommandProcessorAdapter::FileCommandProcessorAdapter(
		const std::string &filename) :
		CommandProcessor(), fileReader(new FileLineReader(filename)) {
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
	delete fileReader;
}

void FileCommandProcessorAdapter::readCommand() {
	std::string line = fileReader->readLine();
	if (!line.empty()) {
		saveCommand(line);
	}
}

std::string FileCommandProcessorAdapter::getCommandFromConsole() {
	std::string line;
	while (hasMoreCommands()) {
		line = fileReader->readLine();
		if (line.empty()) {
			continue;
		}
		if (std::find(Command::allowedCommands.begin(),
				Command::allowedCommands.end(), line)
				!= Command::allowedCommands.end()) {
			saveCommand(line);
			return line;
		}
		std::cout << "Ignoring invalid command: " << line << std::endl;
	}
	return "";
}

bool FileCommandProcessorAdapter::hasMoreCommands() const {
	return !fileReader->getFile().eof();
}
