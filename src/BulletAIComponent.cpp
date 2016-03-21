#include "..\include\BulletAIComponent.h"
#include <Display.h>

namespace game
{
	extern Display game;
	extern System system;
}



BulletAIComponent::BulletAIComponent()
{
	movementCoolDownTime = 0.100;
	int range = 5;
	int rangeIndex = 0;
	direction = DIRECTION_LEFT;
	isDestroyed_ = false;
	movementCoolDown.StartNewTimer(movementCoolDownTime);
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
	game::game.getTileRefAt(this->position).removeOverlay();
	this->position = position;
}

void BulletAIComponent::setRange(int range)
{
	this->range = range;
}

void BulletAIComponent::isDestroyed(bool is)
{
	isDestroyed_ = is;
}

void BulletAIComponent::setGraphic(char g)
{
	graphic_ = g;
	Tile& tile = game::game.getTileRefAt(position);
	tile.setOverlayEnabled(true);
	tile.setOverlayGraphic(g);
}

void BulletAIComponent::clean()
{
	if (isCleaned_ == true)
	{
		return;
	}
	game::game.getTileRefAt(position).removeOverlay();
	isCleaned_ = true;
}

bool BulletAIComponent::isDestroyed()
{
	return isDestroyed_;
}

DIRECTION BulletAIComponent::getDirection()
{
	return direction;
}

Position BulletAIComponent::getPosition()
{
	return position;
}

void BulletAIComponent::update()
{
	if (isDestroyed_ == true) return;
	if (movementCoolDown.Update() == true)
	{
		if (rangeIndex == range)
		{
			isDestroyed(true);
			clean();
			return;
		}
		Position nPos = position;
		nPos.go(direction);
		
		game::game.getTileRefAt(position).removeOverlay();
		
		Tile& tile = game::game.getTileRefAt(nPos);

		if (tile.isWall() == true)
		{
			isDestroyed(true);
			clean();
			return;
		}

		tile.setOverlayEnabled(true);
		tile.setOverlayGraphic(graphic_);

		position = nPos;

		rangeIndex++;
		movementCoolDown.StartNewTimer(movementCoolDownTime);
	}
}
