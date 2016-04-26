#include "Common.h"
#include "Display.h"
#include "Bullet.h"
#include "Turret.h"
#include "Entity.h"
#include "Packet.h"
#include "Player.h"
#include "SimpleNetClient.h"

#define EndLine "\n"

namespace game
{
	extern SimpleNetClient server;
	extern Display game;
	extern System system;
}

void SendServerLiteral(std::string msg)
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
			if (x == player_)
			{
				msg << x << EndLine << AddPlayerLocal << EndLine;
				player->serialize(msg);
			}
			else
			{
				msg << x << EndLine << AddPlayer << EndLine;
				player->serialize(msg);
			}
		}
	}

	bool ShootFrom(Position pos, int direction)
	{
		Position nPos = pos;

		nPos.go((DIRECTION)direction);

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

				SendBullet(bullet.get());
				return true;
			}
		}
		return false;
	}

	bool ShootFrom(Position pos, int direction, int bullet_range)
	{
		Position nPos = pos;

		nPos.go((DIRECTION)direction);

		if (game::game.getTileRefAt(nPos).isWalkable() == true)
		{
			if (game::system.entityAt(nPos) == false)
			{
				shared_ptr<Bullet> bullet = make_shared<Bullet>();
				bullet->setDirection((DIRECTION)direction);
				bullet->setPosition(nPos);
				bullet->setGraphic(250);
				bullet->setBulletRange(bullet_range);
				bullet->addKeyWord(KEYWORD_BULLET);

				game::system.addEntity(bullet, "Bullet");
				return true;
			}
		}
		return false;
	}

	int GetBulletDamage(Entity * entity)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(entity);
		if (bullet == nullptr)
		{
			return 0;
		}
		return bullet->getDamage();
	}
	int GetBulletDirection(Entity * entity)
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
}