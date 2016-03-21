#include "..\include\BulletAIComponent.h"
#include <Display.h>

namespace game
{
	extern Display game;
	extern System system;
}



BulletAIComponent::BulletAIComponent()
{
	movementCoolDownTime = 0.250;
	direction = DIRECTION_LEFT;
}


BulletAIComponent::~BulletAIComponent()
{

}

void BulletAIComponent::setDirection(DIRECTION direction)
{
	this->direction = direction;
}

void BulletAIComponent::setPosition(Position position)
{
	this->position = position;
}

DIRECTION BulletAIComponent::getDirection()
{
	return direction;
}

void BulletAIComponent::update()
{
	if (movementCoolDown.Update() == true)
	{
		Position nPos = position;
		nPos.go(direction);

		Tile tile;
	}
}
