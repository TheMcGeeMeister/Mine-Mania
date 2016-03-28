#include "..\include\BulletAIComponent.h"
#include <Display.h>
#include <PlayerHandler.h>

namespace game
{
	extern Display game;
	extern System system;
	extern PlayerHandler pHandler;
}



BulletAIComponent::BulletAIComponent()
{
	movementCoolDownTime = 0.100;
	range = 10;
	rangeIndex = 0;
	damage = 10;
	direction = DIRECTION_UP;
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
	game::game.getTileRefAt(position).updateOverlay(true, graphic_);
}

void BulletAIComponent::setRange(int range)
{
	this->range = range;
}

void BulletAIComponent::setDamage(int damage)
{
	this->damage = damage;
}

void BulletAIComponent::isDestroyed(bool is)
{
	isDestroyed_ = is;
}

void BulletAIComponent::setGraphic(char g)
{
	graphic_ = g;
	Tile& tile = game::game.getTileRefAt(position);
	tile.updateOverlay(true, g);
}

int BulletAIComponent::getDamage()
{
	return damage;
}

void BulletAIComponent::clean()
{
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

void BulletAIComponent::serialize(fstream & file)
{
	file << movementCoolDownTime << std::endl
		<< direction << std::endl
		<< position.getX() << std::endl
		<< position.getY() << std::endl
		<< range << std::endl
		<< rangeIndex << std::endl
		<< damage << std::endl
		<< isDestroyed_ << std::endl
		<< isCleaned_ << std::endl
		<< (int)graphic_ << std::endl;
}

void BulletAIComponent::deserialize(fstream & file)
{
	int direction_;
	int graphic;
	file >> movementCoolDownTime
		>> direction_
		>> position.getRefX()
		>> position.getRefY()
		>> range
		>> rangeIndex
		>> damage
		>> isDestroyed_
		>> isCleaned_
		>> graphic;
	graphic_ = graphic;
	direction = (DIRECTION)direction_;
}

void BulletAIComponent::update()
{
	if (isDestroyed_ == true) return;
	if (movementCoolDown.Update() == true)
	{
		if (game::pHandler.playerAt(position) == true)
		{
			Player *player = nullptr;
			if (game::pHandler.getPlayerAt(position, &player) == true)
			{
				if (player == nullptr) return;
				player->damage(damage);
				isDestroyed(true);
				clean();
				return;
			}
		}
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

		if (game::pHandler.playerAt(nPos) == true)
		{
			Player *player = nullptr;
			if (game::pHandler.getPlayerAt(nPos, &player) == true)
			{
				if (player == nullptr) return;
				player->damage(damage);
				isDestroyed(true);
				clean();
				return;
			}
		}

		tile.updateOverlay(true, graphic_);

		position = nPos;

		rangeIndex++;
		movementCoolDown.StartNewTimer(movementCoolDownTime);
	}
}
