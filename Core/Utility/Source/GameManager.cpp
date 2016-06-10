#include <sstream>
#include "SimpleNetClient.h"
#include "GameManager.h"
#include "Common.h"
#include "Packet.h"

namespace game
{
	extern bool gameWon;
	extern std::string winnerName;
	extern SimpleNetClient server;
}


GameManager::GameManager()
{
	PlayerAmount = 0;
	CoreDestroyedAmount = 0;
	GameOver = true;
}


GameManager::~GameManager()
{
}

void GameManager::SetLives(std::string name, int lives)
{
	if (g_players.count(name))
	{
		g_players[name].lives = lives;
	}
}

void GameManager::SetLivesAll(std::string name, int lives)
{
	for (auto& iter : g_players)
	{
		iter.second.lives = lives;
	}
}

void GameManager::AddScore(std::string name, int score)
{
	if (g_players.count(name))
	{
		g_players[name].score += score;
	}
}

bool GameManager::PlayerDied(std::string name)
{
	g_players[name].lives--;
	if (g_players[name].lives == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameManager::RemoveLife(std::string name)
{
	g_players[name].lives--;
	if (g_players[name].lives == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameManager::isPlayerDead(std::string name)
{
	if (g_players.count(name))
	{
		if(g_players[name].lives == 0)
		{
			return true;
		}
		else
			return false;
	}
}

void GameManager::Reset()
{
	PlayerAmount = 0;
	CoreDestroyedAmount = 0;
	GameOver = true;
	g_players.clear();
}

void GameManager::AddPlayer(std::string name)
{
	g_players[name].isCoreDestroyed = false;
	g_players[name].lives = 1;
	g_players[name].score = 0;
	PlayerAmount++;
}

void GameManager::CoreDestroyed(std::string name)
{
	g_players[name].isCoreDestroyed = true;
	CoreDestroyedAmount++;
}

void GameManager::Update()
{
	if (game::server.isConnected() == true)
	{
		if (game::server.isHost() == true)
		{
			if (PlayerAmount > 1)
			{
				if (GameOver == false)
				{
					if (CoreDestroyedAmount == (PlayerAmount - 1))
					{
						GameOver = true;
						for (auto& iter : g_players)
						{
							if (iter.second.isCoreDestroyed == false)
							{
								game::winnerName = iter.first;
								game::gameWon = true;
								std::stringstream msg;
								msg << SendDefault << EndLine << Win << EndLine << game::winnerName << EndLine;
								SendServerLiteral(msg.str());
								break;
							}
						}
					}
				}
			}
		}
	}
}

void GameManager::StartGame()
{
	GameOver = false;
}
