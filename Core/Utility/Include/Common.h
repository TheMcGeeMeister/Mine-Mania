#pragma once
#include <utility>
#include <string>
#include <Windows.h>
#include "Position.h"

class Entity;
class Turret;
class Bullet;
class Player;
class Core;
class Tile;

enum COLOR
{
	COLOR_RED = 4, COLOR_GREEN = 2,
};

void SetCursorPosition(int x, int y);

void SendServerLiteral(std::string& msg);

void CreateMultiplayerWorld(int player_amount, std::string names[]);

namespace Common
{
	extern void AddPlayer(Player* player);
	extern void AddLocalPlayer(Player* player);
	extern void CreatePlayerCore(std::string name, Position pos);
	extern void CleanGameOverlay();
	extern void SendBullet(Bullet* bullet);
	extern void SendTurret(Turret* turret);
	extern void SendPlayer(Player* player, int playerAmount, int player_);
	extern void SetStoneFloorAt(Position pos, WORD color, std::string owner = "NO_OWNER");
	extern void SetCursorPosition(int x, int y);
	extern void SetFontSize(int fontSize);
	extern void SetFont(int font);
	extern void SendSound(std::string sound_name);
	extern void ResizeWindowUntilFit(int x, int y);
	extern inline void DisplayLetterAt(Position pos, std::string letter);
	extern void DisplayTextCentered(int x, int line, std::string text);
	extern bool GetPlayerByName(std::string name, Player** player);
	extern bool ShootFrom(Position pos, int direction);
	extern bool ShootFrom(Position pos, int direction, int bullet_range);
	extern bool GetTileAt(Position _in_pos, Tile** _out_tile);
	extern bool isConnected();
	extern bool isFocused();
	extern int GetBulletDamageFromEntity(Entity* entity);
	extern int GetBulletDirectionFromEntity(Entity* entity);
	extern int GetDisplayMaxWidth();
	extern int GetDisplayMaxHeight();
	extern int GetWindowWidth();
	extern int GetWindowHeight();
	extern std::pair<int, int> GetWindowSize();
}