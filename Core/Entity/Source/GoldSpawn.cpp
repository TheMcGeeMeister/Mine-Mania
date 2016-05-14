#include "GoldSpawn.h"
#include "Player.h"
#include "Display.h"
#include "PlayerHandler.h"
#include "Common.h"
#include "TileEnums.h"
#include "LoadEnums.h"

namespace game
{
	extern Display game;
	extern PlayerHandler pHandler;
}


GoldSpawn::GoldSpawn()
{
	isClaimed_ = false;
	isKilled_ = false;
	currentClaimer_ = "";
	owner_ = "";
	claimedPercentage_ = 0;
	color_ = C_White;
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

Position GoldSpawn::getPosition()
{
	return pos_;
}

void GoldSpawn::setPosition(Position pos)
{
	pos_ = pos;
}

bool GoldSpawn::hasComponent(int component)
{
	return false;
}

bool GoldSpawn::isKilled()
{
	return isKilled_;
}

void GoldSpawn::kill()
{
	clean();
	isKilled_ = true;
}

void GoldSpawn::clean()
{
	game::game.getTileRefAt(pos_).updateOverlayS(false, ' ');
}

void GoldSpawn::serialize(std::stringstream & stream)
{
	stream << L_GoldSpawn
		<< claimedPercentage_
		<< currentClaimer_
		<< owner_
		<< isClaimed_
		<< color_
		<< pos_.serializeR();
}

void GoldSpawn::serialize(std::fstream & stream)
{
	stream << L_GoldSpawn
		<< claimedPercentage_
		<< currentClaimer_
		<< owner_
		<< isClaimed_
		<< color_
		<< pos_.serializeR();
}

void GoldSpawn::deserialize(std::stringstream & stream)
{
	stream >> claimedPercentage_
		>> currentClaimer_
		>> owner_
		>> isClaimed_
		>> color_;
	pos_.deserialize(stream);
}

void GoldSpawn::deserialize(std::fstream & stream)
{
	stream >> claimedPercentage_
		>> currentClaimer_
		>> owner_
		>> isClaimed_
		>> color_;
	pos_.deserialize(stream);
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
	std::stringstream msg;
	msg << SendDefault << EndLine << EntityAdd << EndLine << EGoldSpawn << EndLine;
	serialize(msg);
	SendServerLiteral(msg.str());
}

void GoldSpawn::render()
{
	game::game.getTileRefAt(pos_).updateOverlay(true, '@');
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = pos_.getX();
	pos.Y = pos_.getY();
	DWORD output;
	WriteConsoleOutputAttribute(h, &color_, 1, pos, &output);
}

void GoldSpawn::activate(Player* player)
{
	if (isClaimed_)
	{
		if (owner_ == player->getName())
		{
			if (claimedPercentage_ != 100)
			{
				claimedPercentage_ += 10;
			}
		}
		else
		{
			claimedPercentage_ -= 10;
			if (claimedPercentage_ <= 0)
			{
				isClaimed_ = false;
				Player* player;
				if (game::pHandler.getPlayer(owner_, &player))
				{
					player->removePassiveGold(1);
					color_ = C_White;
					render();
				}
				claimedPercentage_ = 0;
			}
		}
	}
	else
	{
		if (currentClaimer_ == "")
		{
			currentClaimer_ = player->getName();
		}

		if (currentClaimer_ == player->getName())
		{
			claimedPercentage_ += 10;
			if (claimedPercentage_ >= 100)
			{
				isClaimed_ = true;
				player->addPassiveGold(1);
				claimedPercentage_ = 100;
				owner_ = player->getName();
				color_ = player->getClaimedColor();
				render();
			}
		}
		else
		{
			claimedPercentage_ -= 10;
			if (claimedPercentage_ <= 10)
			{
				currentClaimer_ = player->getName();
				claimedPercentage_ = 0;
			}
		}
	}
}

void GoldSpawn::update()
{
	return;
}

Position GoldSpawn::getPos()
{
	return pos_;
}
