#include "..\include\TurretAIComponent.h"
#include "..\include\BulletAIComponent.h"
#include "..\include\Bullet.h"
#include <Display.h>
#include <Player.h>
#include <fstream>


namespace game
{
	extern Display game;
	extern Player player;
	extern System system;
}


TurretAIComponent::TurretAIComponent()
{
	visionRange_ = 3;
	shootCoolDownTime_ = 4;
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

void TurretAIComponent::search() // Currently Only Shoots at the Player
{
	Position cPos = curPosition_; // Current Position
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
	}*/
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

void TurretAIComponent::shoot(DIRECTION direction)
{
	Position nPos = curPosition_; // New Position
	switch (direction)
	{
	case DIRECTION_UP: nPos.go(DIRECTION_UP); break;
	case DIRECTION_DOWN: nPos.go(DIRECTION_DOWN); break;
	case DIRECTION_LEFT: nPos.go(DIRECTION_LEFT); break;
	case DIRECTION_RIGHT: nPos.go(DIRECTION_RIGHT); break;
	}

	shared_ptr<Bullet> bullet = make_shared<Bullet>();
	bullet->setDirection(direction);
	bullet->setPosition(nPos);
	bullet->setGraphic('@');

	game::system.addEntity(bullet);

	fstream file("Logs\\Log.txt", ios::app);
	file << "TurretAI:" << "Bullet Shot " << direction << endl;
	file << "TurretAI:" << "Pos:" << nPos.getX() << "," << nPos.getY() << endl;
	file.close();
}

Position TurretAIComponent::getPosition()
{
	return curPosition_;
}

void TurretAIComponent::update()
{
	if (shootCoolDown_.Update() == true)
	{
		if (targetSearchCoolDown_.Update() == true)
		{
			search();
			targetSearchCoolDown_.StartNewTimer(0.250);
			if (targetFound_)
			{
				shootCoolDown_.StartNewTimer(shootCoolDownTime_);
			}
		}
	}
}
