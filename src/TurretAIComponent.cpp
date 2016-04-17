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
	bool foundTarget = false;
	Position pos = curPosition_;
	Position tPos(0,0); // Target Position;

	pos = curPosition_;
	searchLine(pos, DIRECTION_UP, visionRange_, tPos, foundTarget);
	if (foundTarget == true)
	{
		targetPosition_ = tPos;
		shoot(DIRECTION_UP);
		targetFound_ = true;
		return;
	}

	pos = curPosition_;
	searchLine(pos, DIRECTION_DOWN, visionRange_, tPos, foundTarget);

	if (foundTarget == true)
	{
		targetPosition_ = tPos;
		shoot(DIRECTION_DOWN);
		targetFound_ = true;
		return;
	}

	pos = curPosition_;
	searchLine(pos, DIRECTION_LEFT, visionRange_, tPos, foundTarget);

	if (foundTarget == true)
	{
		targetPosition_ = tPos;
		shoot(DIRECTION_LEFT);
		targetFound_ = true;
		return;
	}

	pos = curPosition_;
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

	nPos.go(direction);

	shared_ptr<Bullet> bullet = make_shared<Bullet>();
	bullet->setDirection(direction);
	bullet->setPosition(nPos);
	bullet->setGraphic(250);
	bullet->addKeyWord(KEYWORD_BULLET);

	game::system.addEntity(bullet, "Bullet");
}

Position TurretAIComponent::getPosition()
{
	return curPosition_;
}

std::string TurretAIComponent::getOwner()
{
	return owner_;
}

void TurretAIComponent::serialize(fstream & file)
{
	file << visionRange_ << std::endl
		<< shootCoolDownTime_ << std::endl
		<< targetFound_ << std::endl
		<< targetPosition_.getX() << std::endl
		<< targetPosition_.getY() << std::endl
		<< curPosition_.getX() << std::endl
		<< curPosition_.getY() << std::endl
		<< owner_ << std::endl;
}

void TurretAIComponent::deserialize(fstream & file)
{
	file >> visionRange_
		>> shootCoolDownTime_
		>> targetFound_
		>> targetPosition_.getRefX()
		>> targetPosition_.getRefY()
		>> curPosition_.getRefX()
		>> curPosition_.getRefY()
		>> owner_;
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
