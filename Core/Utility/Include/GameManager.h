#pragma once
#include <string>
#include <map>

struct PlayerInfo
{
	int score;
	int lives;
	bool isCoreDestroyed;
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	void SetLives(std::string name, int lives);
	void SetLivesAll(std::string name, int lives);
	void AddScore(std::string name, int score);
	bool PlayerDied(std::string name);
	bool RemoveLife(std::string name);

	bool isPlayerDead(std::string name);

	void Reset();
	void Update();
	void StartGame();
	void AddPlayer(std::string name);
	void CoreDestroyed(std::string name);
private:
	std::map<std::string, PlayerInfo> g_players;
	int CoreDestroyedAmount;
	int PlayerAmount;
	bool GameOver;
};

