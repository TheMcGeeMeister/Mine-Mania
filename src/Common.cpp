#include <Common.h>
#include <Display.h>
#include <SimpleNetClient.h>
#include <Bullet.h>
#include <Entity.h>

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
}