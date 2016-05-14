#include "GoldSpawn.h"
#include "Player.h"
#include "Display.h"

namespace game
{
	extern Display game;
}


GoldSpawn::GoldSpawn()
{

}


GoldSpawn::~GoldSpawn()
{

}

std::string GoldSpawn::getOwner()
{
	return owner_;
}

void GoldSpawn::setOwner(std::string owner)
{
	owner_ = owner;
	claimedPercentage_ = 100;
}

bool GoldSpawn::hasComponent(int component)
{
	return false;
}

bool GoldSpawn::isKilled()
{
	return false;
}

void GoldSpawn::kill()
{

}

void GoldSpawn::clean()
{

}

void GoldSpawn::serialize(std::fstream &)
{

}

void GoldSpawn::deserialize(std::fstream &)
{

}

bool GoldSpawn::damage(int damage, std::string name, bool server)
{
	return false;
}

void GoldSpawn::setPos(Position pos)
{
	pos_ = pos;
}

void GoldSpawn::updateOverlay()
{
	game::game.getTileRefAt(pos_).updateOverlay(true, '@');
}

void GoldSpawn::updateID()
{

}

void GoldSpawn::send()
{

}

void GoldSpawn::render()
{

}

void GoldSpawn::activate(Player* player)
{
	if (currentClaimer_ != player->getName())
	{
		claimedPercentage_ -= 10;
		if (claimedPercentage_ <= 0)
		{
			claimedPercentage_ = 0;
			currentClaimer_ = player->getName();
			isClaimed_ = false;
			owner_ = "NONE";
		}
	}
	else
	{
		claimedPercentage_ += 10;
		if (claimedPercentage_ >= 100)
		{
			claimedPercentage_ = 100;
			isClaimed_ = true;
			owner_ = player->getName();
		}
	}
}

Position GoldSpawn::getPos()
{
	return Position();
}
