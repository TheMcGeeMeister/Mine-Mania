#include <fstream>
#include "PlayerHandler.h"


PlayerHandler::PlayerHandler()
{
	Player player;
	m_players["Local"] = player;
}


PlayerHandler::~PlayerHandler()
{
}

void PlayerHandler::addPlayer(Player& player)
{
	m_players[player.getName()] = player;
}

void PlayerHandler::addLocalPlayer(Player player)
{
	m_players["Local"] = player;
}

void PlayerHandler::update()
{
	m_players["Local"].update();
}

bool PlayerHandler::playerAt(Position pos)
{
	for (auto& iter : m_players)
	{
		if (iter.second.getHandPosition() == pos)
		{
			return true;
		}
	}
	return false;
}

bool PlayerHandler::getPlayerAt(Position pos, Player** player)
{
	for (auto& iter : m_players)
	{
		if (iter.second.getHandPosition() == pos)
		{
			*player = &iter.second;
			return true;
		}
	}
	return false;
}

bool PlayerHandler::getPlayer(std::string name, Player** player)
{
	if (m_players.count(name))
	{
		*player = &m_players[name];
		return true;
	}
	return false;
}

Player & PlayerHandler::getLocalPlayer()
{
	return m_players["Local"];
}

void PlayerHandler::clear()
{
	m_players.clear();
	Player player;
	m_players["Local"] = player;
}
