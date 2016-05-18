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

void PlayerHandler::updateAllPositions()
{
	for (auto& iter : m_players)
	{
		iter.second.updateHandPos();
	}
}

void PlayerHandler::addPlayer(Player& player)
{
	m_players[player.getName()] = player;
}

void PlayerHandler::addPlayerDeserialize(std::fstream & file)
{
	Player player;
	player.deserialize(file);
	m_players[player.getName()] = player;
}

void PlayerHandler::addPlayerDeserialize(std::stringstream & file)
{
	Player player;
	player.deserialize(file);
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
	for (auto& iter : m_players)
	{
		if (iter.second.getName() == name)
		{
			*player = &iter.second;
			return true;
		}
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
	std::string name = m_players["Local"].getName();
	m_players["Local"] = Player();
	m_players["Local"].setName(name);
}

void PlayerHandler::serializeAll(std::stringstream & file)
{
	for (auto& iter : m_players)
	{
		iter.second.serialize(file);
	}
}

void PlayerHandler::serializeAll(std::fstream & file)
{
	m_players["Local"].serialize(file);
	for (auto& iter : m_players)
	{
		if(iter.first != "Local")
			iter.second.serialize(file);
	}
}
