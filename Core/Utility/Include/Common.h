#pragma once
#include <string>
#include <Windows.h>
#include "Position.h"

class Entity;
class Turret;
class Bullet;
class Player;
class Core;

enum COLOR
{
	COLOR_RED = 4, COLOR_GREEN = 2,
};

void SetCursorPosition(int x, int y);

void SendServerLiteral(std::string& msg);

void CreateMultiplayerWorld(int player_amount, std::string names[]);

namespace Common
{
	extern void CleanGameOverlay();
	extern void SendBullet(Bullet* bullet);
	extern void SendTurret(Turret* turret);
	extern void SendPlayer(Player* player, int playerAmount, int player_);
	extern void AddPlayer(Player* player);
	extern void AddLocalPlayer(Player* player);
	extern void CreatePlayerCore(std::string name, Position pos);
	extern void SetStoneFloorAt(Position pos, std::string owner = "NO_OWNER");
	extern bool ShootFrom(Position pos, int direction);
	extern bool ShootFrom(Position pos, int direction, int bullet_range);
	extern bool isConnected();
	extern int GetBulletDamage(Entity* entity);
	extern int GetBulletDirection(Entity* entity);
	extern int GetDisplayMaxWidth();
	extern int GetDisplayMaxHeight();
}

namespace Network
{
	extern void SendCore(Core* core);
}
