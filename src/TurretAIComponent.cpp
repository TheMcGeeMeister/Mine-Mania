#include "..\include\TurretAIComponent.h"
#include "..\include\BulletAIComponent.h"
#include "..\include\Bullet.h"
#include <Display.h>
#include <Player.h>
#include <fstream>
#include <PlayerHandler.h>


namespace game
{
	extern Display game;
	extern Player player;
	extern System system;
	extern PlayerHandler pHandler;
}


TurretAIComponent::TurretAIComponent()
{
	visionRange_ = 3;
	shootCoolDownTime_ = 4;
	owner_ = "None";
}


TurretAIComponent::~TurretAIComponent()
{
}

void TurretAIComponent::setPosition(Position pos)
{
	curPosition_ = pos;
}

void TurretAIComponent::setShootCoolDownTime(int secs)
{
	shootCoolDownTime_ = secs;
}

void TurretAIComponent::setVisionRange(int range)
{
	visionRange_ = range;
}

void TurretAIComponent::setOwner(std::string owner)
{
	owner_ = owner;
}

void TurretAIComponent::search() // Currently Only Shoots at the Player
{
	{ // Old Code
		/*Position cPos = curPosition_; // Current Position
		Position pPos = game::player.getHandPosition(); // Player Position

		int player_x = pPos.getX(); // For Readability
		int player_y = pPos.getY(); // For Readability

		Position uPos = curPosition_; uPos.go(DIRECTION_UP, visionRange_); // Up Position
		Position dPos = curPosition_; dPos.go(DIRECTION_DOWN, visionRange_); // Down Position
		Position lPos = curPosition_; lPos.go(DIRECTION_LEFT, visionRange_); // Left Position
		Position rPos = curPosition_; rPos.go(DIRECTION_RIGHT, visionRange_); // Right Position

		if (player_y >= uPos.getY() && player_y < dPos.getY()) // Player is 3 under, or above turret
		{
			if (player_x == cPos.getX())
			{
				targetFound_ = true;
				targetPosition_ = pPos;
				if (player_y > curPosition_.getY())
					shoot(DIRECTION_DOWN);
				else
					shoot(DIRECTION_UP);
				return;
			}
		}
		/*else if (player_y <= dPos.getY() && player_y > uPos.getY())
		{
			targetFound_ = true;
			targetPosition_ = pPos;
			shoot(DIRECTION_UP);
			return;
		}
		else if (player_x >= lPos.getX() && player_x < rPos.getX()) // Player is 3 left, or right of turret
		{
			if (player_y == cPos.getY())
			{
				targetFound_ = true;
				targetPosition_ = pPos;
				if (player_x < curPosition_.getX())
					shoot(DIRECTION_LEFT);
				else
					shoot(DIRECTION_RIGHT);

				return;
			}
		}
		/*else if (player_x <= rPos.getX() && player_x > lPos.getX())
		{
			targetFound_ = true;
			targetPosition_ = pPos;
			shoot(DIRECTION_RIGHT);
			return;
		}*/
	}
	bool foundTarget = false;
	Position pos = curPosition_;
	Position tPos(0,0); // Target Position;

	/*for (int x = 0; x < 4; x++)
	{
		pos = curPosition_;
		pos.go((DIRECTION)x);
		searchLine(pos, (DIRECTION)x, visionRange_, tPos, foundTarget);
		if (foundTarget == true)
		{
			targetPosition_ = tPos;
			shoot((DIRECTION)x);

			/* Debug 
			//////////////////////
			std::fstream file("Logs\\Log.txt", std::ios::app);
			file << "Turret: Shoot: DIRECTION ->" << x << std::endl;
			//////////////////////
			return;
		}
	}*/

	pos = curPosition_;
	pos.go(DIRECTION_UP);
	searchLine(pos, DIRECTION_UP, visionRange_, tPos, foundTarget);
	if (foundTarget == true)
	{
		targetPosition_ = tPos;
		shoot(DIRECTION_UP);
		targetFound_ = true;
		return;
	}

	pos = curPosition_;
	pos.go(DIRECTION_DOWN);
	searchLine(pos, DIRECTION_DOWN, visionRange_, tPos, foundTarget);

	if (foundTarget == true)
	{
		targetPosition_ = tPos;
		shoot(DIRECTION_DOWN);
		targetFound_ = true;
		return;
	}

	pos = curPosition_;
	pos.go(DIRECTION_LEFT);
	searchLine(pos, DIRECTION_LEFT, visionRange_, tPos, foundTarget);

	if (foundTarget == true)
	{
		targetPosition_ = tPos;
		shoot(DIRECTION_LEFT);
		targetFound_ = true;
		return;
	}

	pos = curPosition_;
	pos.go(DIRECTION_RIGHT);
	searchLine(pos, DIRECTION_RIGHT, visionRange_, tPos, foundTarget);

	if (foundTarget == true)
	{
		targetPosition_ = tPos;
		shoot(DIRECTION_RIGHT);
		targetFound_ = true;
		return;
	}
}

void TurretAIComponent::searchLine(Position sPos, DIRECTION direction, int amount, Position& fPos, bool& isFind)
{
	Position cPos = sPos; // Current Position
	Position hPos = game::pHandler.getLocalPlayer().getHandPosition(); // Hand Position

	for (int x = 0; x < amount; x++)
	{
		cPos.go(direction);
		Tile& tile = game::game.getTileRefAt(cPos);
		if (tile.isWall() == true)
		{
			isFind = false;
			return;
		}
		else
		{
			if (game::pHandler.playerAt(cPos))
			{
				Player* player = nullptr;
				if (game::pHandler.getPlayerAt(cPos, &player) == true)
				{
					if (player->getName() == owner_)
					{
						return;
					}
					else
					{
						isFind = true;
						fPos = cPos;
						return;
					}
				}
			}
		}
	}
	isFind = false;
	return;
}

void TurretAIComponent::shoot(DIRECTION direction)
{
	Position nPos = curPosition_; // New Position
	switch (direction)
	{
	case DIRECTION_UP: nPos.go(DIRECTION_UP); break;
	case DIRECTION_DOWN: nPos.go(DIRECTION_DOWN); break;
	case DIRECTION_LEFT: nPos.go(DIRECTION_LEFT); break;
	case DIRECTION_RIGHT: nPos.go(DIRECTION_RIGHT); break;
	default: return;
	}

	shared_ptr<Bullet> bullet = make_shared<Bullet>();
	bullet->setDirection(direction);
	bullet->setPosition(nPos);
	bullet->setGraphic(158);

	game::system.addEntity(bullet);
}

Position TurretAIComponent::getPosition()
{
	return curPosition_;
}

std::string TurretAIComponent::getOwner()
{
	return owner_;
}

void TurretAIComponent::update()
{
	if (shootCoolDown_.Update() == true)
	{
		if (targetSearchCoolDown_.Update() == true)
		{
			search();
			targetSearchCoolDown_.StartNewTimer(0.10);
			if (targetFound_)
			{
				targetFound_ = false;
				shootCoolDown_.StartNewTimer(shootCoolDownTime_);
			}
		}
	}
}
