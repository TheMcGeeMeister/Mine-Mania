#include "BulletAIComponent.h"
#include "Display.h"
#include "PlayerHandler.h"
#include "SoundManager.h"
#include "Common.h"
#include "TileEnums.h"

namespace game
{
	extern Display game;
	extern System system;
	extern PlayerHandler pHandler;
	extern SoundManager m_sounds;
}


BulletAIComponent::BulletAIComponent()
{
	movementCoolDownTime = 0.100;
	realMovementCoolDownTime = movementCoolDownTime;
	range = 10;
	rangeIndex = 0;
	damage = 10;
	direction = DIRECTION_UP;
	isDestroyed_ = false;
	isCleaned_ = false;
	position(0, 0);
	graphic_ = 250;
	movementCoolDown.StartNewTimer(movementCoolDownTime);
}


BulletAIComponent::~BulletAIComponent()
{

}

void BulletAIComponent::setDirection(DIRECTION direction)
{
	this->direction = direction;
	if (direction == DIRECTION_LEFT | DIRECTION_RIGHT)
	{
		realMovementCoolDownTime = movementCoolDownTime / 2;
	}
	else
	{
		realMovementCoolDownTime = movementCoolDownTime;
	}
}

void BulletAIComponent::setPosition(Position position)
{
	game::game.getTileRefAt(this->position).removeOverlay();
	this->position = position;
	Tile& tile = game::game.getTileRefAt(position);
	tile.updateOverlay(true, graphic_);
	tile.setOverlayColor(C_Black);
}

void BulletAIComponent::setPositionNoUpdate(Position position)
{
	this->position = position;
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
	tile.setOverlayColor(C_Black);
}

void BulletAIComponent::setGraphicNoUpdate(char g)
{
	graphic_ = g;
}

void BulletAIComponent::updateOverlay()
{
	Tile& tile = game::game.getTileRefAt(position);
	tile.updateOverlay(true, graphic_);
	tile.setOverlayColor(C_Black);
}

void BulletAIComponent::setID(int id)
{
	this->id = id;
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

void BulletAIComponent::serialize(std::stringstream & file)
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
	if (direction == DIRECTION_LEFT || DIRECTION_RIGHT)
	{
		realMovementCoolDownTime = movementCoolDownTime / 2;
	}
	else
	{
		realMovementCoolDownTime = movementCoolDownTime;
	}
}

void BulletAIComponent::deserialize(std::stringstream & file)
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
	if (direction == DIRECTION_LEFT || DIRECTION_RIGHT)
	{
		realMovementCoolDownTime = movementCoolDownTime / 2;
	}
	else
	{
		realMovementCoolDownTime = movementCoolDownTime;
	}
}

void BulletAIComponent::update()
{
	if (isDestroyed_ == true) return;
	if (movementCoolDown.Update() == true)
	{
		/* Bullet Moved too far */
		///////////////////////////////////////////
		if (rangeIndex == range)
		{
			isDestroyed(true);
			clean();
			return;
		}
		///////////////////////////////////////////


		Position nPos = position; // New Position 
		if (nPos.go(direction) == false)
		{
			isDestroyed(true);
			clean();
			return;
		}
		
		game::game.getTileRefAt(position).removeOverlay();
		
		Tile& tile = game::game.getTileRefAt(nPos);

		/* Hit Wall */
		if (tile.isWall() == true)
		{
			isDestroyed(true);
			clean();
			return;
		}

		/* Hit Player */
		if (game::pHandler.playerAt(nPos) == true)
		{
			Player *player = nullptr;
			if (game::pHandler.getPlayerAt(nPos, &player) == true)
			{
				if (player == nullptr) return;
				player->knockbackTo(direction, 1);
				player->disableMovementFor(1.5);
				player->damage(damage);
				game::m_sounds.PlaySoundR("Bullet");
				std::stringstream msg;
				msg << SendDefault << EndLine << Sound << EndLine << "Bullet" << EndLine;
				SendServerLiteral(msg.str());
				isDestroyed(true);
				clean();
				return;
			}
		}

		/* Hit Entity */
		if (game::system.entityAt(nPos) == true)
		{
			Entity *entity = nullptr;
			if (game::system.getEntityAt(nPos, &entity))
			{
				entity->damage(damage, string());
				isDestroyed(true);
				clean();
				return;
			}
		}

		tile.updateOverlay(true, graphic_);
		tile.setOverlayColor(C_Black);

		std::stringstream msg;

		msg << SendDefault << EndLine << EntityUpdatePosition << EndLine << id << EndLine << nPos.serializeR();

		SendServerLiteral(msg.str());

		position = nPos;

		rangeIndex++;
		movementCoolDown.StartNewTimer(realMovementCoolDownTime);
	}
}
