#pragma once
#include <string>
#include "Player.h"

class PlayerHandler
{
public:
	PlayerHandler();
	~PlayerHandler();

	void updateAllPositions();

	void setLocalID(int id);

	void addPlayer(Player& player);
	void addPlayerDeserialize(std::fstream& file);
	void addPlayerDeserialize(std::stringstream& file);
	void addLocalPlayer(Player player);

	void update();


	bool playerAt(Position pos);
	bool getPlayerAt(Position pos, Player** player);

	bool getPlayer(std::string name, Player** player);
	bool getPlayerByID(int id, Player** player);

	Player& getLocalPlayer();

	void clear();
	void serializeAll(std::stringstream& file);
	void serializeAll(std::fstream& file);
private:
	map <int, Player> m_players;
	int local;
};

