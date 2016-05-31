#include <fstream>
#include "PlayerHandler.h"


PlayerHandler::PlayerHandler()
{
	local = 0;
	m_players[local] = Player();
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

void PlayerHandler::setLocalID(int id)
{
	Player player = m_players[local];
	m_players.erase(local);
	local = id;
	m_players[local] = player;
}

void PlayerHandler::addPlayer(Player& player)
{
	m_players[player.getID()] = player;
}

void PlayerHandler::addPlayerDeserialize(std::fstream & file)
{
	Player player;
	player.deserialize(file);
	m_players[player.getID()] = player;
}

void PlayerHandler::addPlayerDeserialize(std::stringstream & file)
{
	Player player;
	player.deserialize(file);
	m_players[player.getID()] = player;
}

void PlayerHandler::addLocalPlayer(Player player)
{
	m_players[local] = player;
	m_players[local].setID(local);
}

void PlayerHandler::update()
{
	m_players[local].update();
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

bool PlayerHandler::getPlayerByID(int id, Player ** player)
{
	if (m_players.count(id))
	{
		*player = &m_players[id];
		return true;
	}
	else
	{
		*player = nullptr;
		return false;
	}
}

Player & PlayerHandler::getLocalPlayer()
{
	return m_players[local];
}

void PlayerHandler::clear()
{
	m_players.clear();
	local = 0;
	std::string name = m_players[local].getName();
	m_players[local] = Player();
	m_players[local].setName(name);
}

void PlayerHandler::serializeAll(std::stringstream & file)
{
	m_players[local].serialize(file);
	for (auto& iter : m_players)
	{
		if (iter.first != local)
			iter.second.serialize(file);
	}
}

void PlayerHandler::serializeAll(std::fstream & file)
{
	m_players[local].serialize(file);
	for (auto& iter : m_players)
	{
		if(iter.first != local)
			iter.second.serialize(file);
	}
}
