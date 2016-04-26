#pragma once
#include <string>
#include "Player.h"

class PlayerHandler
{
public:
	PlayerHandler();
	~PlayerHandler();

	void addPlayer(Player& player);
	void addPlayerDeserialize(std::fstream& file);
	void addPlayerDeserialize(std::stringstream& file);
	void addLocalPlayer(Player player);

	void update();


	bool playerAt(Position pos);
	bool getPlayerAt(Position pos, Player** player);

	bool getPlayer(std::string name, Player** player);

	Player& getLocalPlayer();

	void clear();
	void serializeAll(std::stringstream& file);
	void serializeAll(std::fstream& file);
private:
	map <std::string, Player> m_players;
};

