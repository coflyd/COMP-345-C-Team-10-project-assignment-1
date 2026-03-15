#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>
#include <map>
#include <array>
#include <fstream>
#include "Map.h"
#include "Player.h"

struct WarZone {
	Map* map;
	std::vector<Player*> players;
	OrdersList* ol;
	Deck* deck;
	WarZone();
	WarZone(const WarZone& other);
	~WarZone();
};

class State {
private:
	std::string name;

public:
	static const std::array<std::string, 8> allowedStates;

	State(const std::string& n);
	State(const State& other);
	State& operator=(const State& other);
	~State() = default;

	std::string getName() const;

	friend std::ostream& operator<<(std::ostream& os, const State& s);
};

class Command {
private:
	std::string type;
	std::string effect;

public:
	static const std::array<std::string, 10> allowedCommands;

	Command(const std::string& cmdType);
	Command(const Command& other);
	Command& operator=(const Command& other);
	~Command() = default;

	std::string getType() const;
	std::string getEffect() const;
	void saveEffect(const std::string& e);

	friend std::ostream& operator<<(std::ostream& os, const Command& c);
};

class CommandProcessor;

class GameEngine {
private:
	static const std::string MAPS_DIRECTORY;
	State currentState;
	std::map<std::pair<std::string, std::string>, std::string> transitions;
	WarZone* warZone;

public:
	CommandProcessor* commandProcessor;

	GameEngine();
	GameEngine(const GameEngine& other);
	GameEngine& operator=(const GameEngine& other);
	~GameEngine();

	// Parts 1 & 2
	void setCommandProcessor(CommandProcessor* processor);
	void transition(const std::string& commandType);
	State getCurrentState() const;
	CommandProcessor* getCommandProcessor() const;
	bool isValidTransition(const std::string& commandType) const;
	void startupPhase();
	void loadmap();
	void validatemap();
	void addplayer();
	void gamestart();
	void initialRandomCountryDistribution();
	void createInitialDeck(int bo, int bl, int ai, int di, int re, int te);
	void displayOwnedCountries();
	void drawInitialCards(int nbCards);

	// Part 3 main game loop
	void mainGameLoop();
	void reinforcementPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();
	bool checkWinCondition(Player*& winner) const;
	void removeDefeatedPlayers();

	friend std::ostream& operator<<(std::ostream& os, const GameEngine& ge);
};

class CommandProcessor {
private:
	std::vector<Command*> commands;
	GameEngine* gameEngine;
	virtual void readCommand();
public:
	CommandProcessor();
	CommandProcessor(GameEngine* engine);
	CommandProcessor(const CommandProcessor& other);
	CommandProcessor& operator=(const CommandProcessor& other);
	virtual ~CommandProcessor();

	bool saveCommand(const std::string& commandType);
	virtual std::string getCommandFromConsole();
	Command* getCommand();
	bool validate(const std::string& commandType,
		const State& currentState) const;
	const std::vector<Command*>& getCommands() const;

	friend std::ostream& operator<<(std::ostream& os,
		const CommandProcessor& cp);
};

class FileLineReader {
private:
	std::ifstream file;

public:
	FileLineReader(const std::string& filename);
	~FileLineReader();

	std::string readLine();
	const std::ifstream& getFile() const;
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
	FileLineReader* fileReader;
	void readCommand() override;
public:
	FileCommandProcessorAdapter(const std::string& filename);
	~FileCommandProcessorAdapter();
	std::string getCommandFromConsole() override;
	bool hasMoreCommands() const;
};

#endif