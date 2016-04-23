#pragma once
#include <string>
#include "Player.h"

class PlayerHandler
{
public:
	PlayerHandler();
	~PlayerHandler();

	void addPlayer(Player& player);
	void addLocalPlayer(Player player);

	void update();


	bool playerAt(Position pos);
	bool getPlayerAt(Position pos, Player** player);

	bool getPlayer(std::string name, Player** player);

	Player& getLocalPlayer();

	void clear();
private:
	map <std::string, Player> m_players;
};

