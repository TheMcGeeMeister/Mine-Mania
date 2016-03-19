#include "..\include\TurretAIComponent.h"
#include <Display.h>
#include <Underlord.h>

namespace game
{
	extern Display game;
	extern Underlord player;
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
		targetFound_ = true;
		targetPosition_ = pPos;
		return;
	}
	else if (player_x >= lPos.getX() && player_x < rPos.getX()) // Player is 3 left, or right of turret
	{
		targetFound_ = true;
		targetPosition_ = pPos;
		return;
	}
}

void TurretAIComponent::shoot(DIRECTION direction, Position sPos)
{

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
