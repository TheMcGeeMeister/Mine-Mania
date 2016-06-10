#include "game.h"
#include "Core.h"
#include "Common.h"
#include "Bullet.h"
#include "Turret.h"
#include "Entity.h"
#include "Packet.h"
#include "Display.h"
#include "PlayerHandler.h"
#include "SimpleNetClient.h"
#include "TileEnums.h"

#define EndLine "\n"

namespace game
{
	extern SimpleNetClient server;
	extern Display game;
	extern System system;
	extern PlayerHandler pHandler;
}

void SendServerLiteral(std::string& msg)
{
	game::server.SendLiteral(msg);
}

void SetCursorPosition(int x, int y)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(h, pos);
}

void CreateMultiplayerWorld(int player_amount, std::string names[])
{
	game::game.newWorldMulti(player_amount, names);
}

namespace Common
{
	void CleanGameOverlay()
	{
		game::game.cleanOverlays();
	}

	bool GetPlayerByName(std::string name, Player ** player)
	{
		if (game::pHandler.getPlayer(name, player))
		{
			return true;
		}
		else
		{
			*player = nullptr;
			return false;
		}
	}

	void SendBullet(Bullet* bullet)
	{
		std::stringstream msg;
		msg << SendDefault << EndLine << EntityAdd << EndLine << EBullet << EndLine; bullet->serialize(msg);
		game::server.SendLiteral(msg.str());
	}

	void SendTurret(Turret* turret)
	{
		std::stringstream msg;
		msg << SendDefault << EndLine << EntityAdd << EndLine << ETurret << EndLine; turret->serialize(msg);
		game::server.SendLiteral(msg.str());
	}

	void SendPlayer(Player* player, int playerAmount, int player_)
	{
		std::stringstream msg;
		for (int x = 0; x < playerAmount; x++)
		{
			if (x != game::server.getId())
			{
				if (x == player_)
				{
					msg << x << EndLine << AddPlayerLocal << EndLine;
					player->serialize(msg);
				}
				else
				{
					msg << x << EndLine << PacketNames::AddPlayer << EndLine;
					player->serialize(msg);
				}
				SendServerLiteral(msg.str());
				msg.str(string());
			}
		}
	}

	void AddPlayer(Player * player)
	{
		game::pHandler.addPlayer(*player);
	}

	void AddLocalPlayer(Player * player)
	{
		game::pHandler.addLocalPlayer(*player);
	}

	void CreatePlayerCore(std::string name, Position pos)
	{
		Tile& tile = game::game.getTileRefAt(pos);
		tile = Tile();
		tile.setColor(C_Black);
		tile.setPos(pos);
		shared_ptr<Core> NewCore = make_shared<Core>();
		NewCore->setPos(pos);
		NewCore->setOwner(name);
		NewCore->setGraphic('C');
		game::system.addEntity(NewCore);
	}

	void SetStoneFloorAt(Position pos, WORD color, std::string owner)
	{
		Tile tile;
		tile.setPos(pos);
		tile.forceClaim(owner);
		tile.setClaimColor(color);
		game::game.setTileAtNoSend(pos, tile);
	}

	void SetCursorPosition(int x, int y)
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos = { x,y };
		SetConsoleCursorPosition(h, pos);
	}

	void SetFontSize(int fontSize)
	{
		int font = game::game.getFont();
		if (fontSize < 5 || fontSize > 72)
			fontSize = 28;
		if (font == 0)
		{
			CONSOLE_FONT_INFOEX info = { 0 };
			info.cbSize = sizeof(info);
			info.dwFontSize.Y = fontSize; // leave X as zero
			info.FontWeight = FW_NORMAL;
			wcscpy(info.FaceName, L"Lucida Console");
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
		}
		else
		{
			CONSOLE_FONT_INFOEX info = { 0 };
			info.cbSize = sizeof(info);
			info.dwFontSize.Y = fontSize; // leave X as zero
			info.FontWeight = FW_NORMAL;
			wcscpy(info.FaceName, L"Consolas");
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
		}
		game::game.setFontSize(fontSize);
	}

	void SetFont(int font)
	{
		int fontSize = game::game.getFontSize();
		if (font == 0)
		{
			CONSOLE_FONT_INFOEX info = { 0 };
			info.cbSize = sizeof(info);
			info.dwFontSize.Y = fontSize; // leave X as zero
			info.FontWeight = FW_NORMAL;
			wcscpy(info.FaceName, L"Lucida Console");
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
		}
		else
		{
			CONSOLE_FONT_INFOEX info = { 0 };
			info.cbSize = sizeof(info);
			info.dwFontSize.Y = fontSize; // leave X as zero
			info.FontWeight = FW_NORMAL;
			wcscpy(info.FaceName, L"Consolas");
			SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
		}
		game::game.setFont(font);
	}

	void SendSound(std::string sound_name)
	{
		std::stringstream msg;
		msg << SendDefault << EndLine << Sound << EndLine << sound_name << EndLine;
		SendServerLiteral(msg.str());
	}

	void ResizeWindowUntilFit(int x, int y)
	{
		int fontSize = game::game.getFontSize();
		pair<int, int> size = GetWindowSize();
		while (size.first < x && size.second < y)
		{
			fontSize--;
			SetFontSize(fontSize);
		}
	}

	void Log(std::string message)
	{
		std::fstream stream("Logs\\Log.txt");
		if (stream.is_open())
		{
			stream << message;
		}
		return;
	}

	inline void DisplayLetterAt(Position pos, std::string g)
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD rPos;
		rPos.X = pos.getX();
		rPos.Y = pos.getY();
		DWORD o;
		WORD attribute = C_White | B_Black;
		WriteConsoleOutputCharacter(h, g.c_str(), 1, rPos, &o);
		WriteConsoleOutputAttribute(h, &attribute, 1, rPos, &o);
	}

	void DisplayTextCentered(int x, int line, std::string text)
	{
		int start_x;
		if (text.length() > x) return;
		if (text.length() != x)
		{
			start_x = (x - text.length()) / 2;
		}
		else
		{
			start_x = 0;
		}
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD rPos;
		DWORD o;
		rPos.Y = line;
		rPos.X = start_x;
		vector<WORD> attributes(text.length(), C_White);
		WriteConsoleOutputCharacter(h, text.c_str(), text.length(), rPos, &o);
		WriteConsoleOutputAttribute(h, &attributes[0], text.length(), rPos, &o);
	}

	bool ShootFrom(Position pos, int direction)
	{
		Position nPos = pos;

		if (nPos.go((DIRECTION)direction))
		{
			if (game::game.getTileRefAt(nPos).isWalkable() == true)
			{
				if (game::system.entityAt(nPos) == false)
				{
					shared_ptr<Bullet> bullet = make_shared<Bullet>();
					bullet->setDirection((DIRECTION)direction);
					bullet->setPosition(nPos);
					bullet->setGraphic(250);
					bullet->addKeyWord(KEYWORD_BULLET);

					game::system.addEntity(bullet, "Bullet");
					return true;
				}
			}
		}
		return false;
	}

	bool ShootFrom(Position pos, int direction, int bullet_range)
	{
		Position nPos = pos;

		if (nPos.go((DIRECTION)direction))
		{
			if (game::game.getTileRefAt(nPos).isWall() == false)
			{
				if (game::system.entityAt(nPos) == false)
				{
					shared_ptr<Bullet> bullet = make_shared<Bullet>();
					bullet->setDirection((DIRECTION)direction);
					bullet->setPositionNoUpdate(nPos);
					bullet->setBulletRange(bullet_range);

					game::system.addEntity(bullet);
					return true;
				}
			}
		}
		return false;
	}

	bool isConnected()
	{
		return game::server.isConnected();
	}

	bool isFocused()
	{
		HANDLE h = GetActiveWindow();
		HANDLE focus = GetFocus();
		if (h != focus)
		{
			return false;
		}
		else
			return true;
	}

	int GetBulletDamageFromEntity(Entity * entity)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(entity);
		if (bullet == nullptr)
		{
			return 0;
		}
		return bullet->getDamage();
	}
	int GetBulletDirectionFromEntity(Entity * entity)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(entity);
		if (bullet == nullptr)
		{
			return 0;
		}
		return bullet->getDirection();
	}
	int GetDisplayMaxWidth()
	{
		return game::game.getMaxWidth();
	}
	int GetDisplayMaxHeight()
	{
		return game::game.getMaxHeight();
	}
	int GetWindowWidth()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int columns, rows;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		return columns;
	}
	int GetWindowHeight()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int columns, rows;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		return rows;
	}

	bool GetTileAt(Position _in_pos, Tile** _out_tile)
	{
		Tile* temp;
		game::game.getTilePAt(_in_pos, &temp);
		*_out_tile = temp;
		return true;
	}

	std::pair<int, int> GetWindowSize()
	{
		pair<int, int> size;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int columns, rows;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		size.first = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		size.second = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		return size;
	}
	std::pair<int, int> GetDesktopResolution()
	{
		pair<int, int> size;
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		size.first = desktop.right;
		size.second = desktop.bottom;
		return size;
	}
}