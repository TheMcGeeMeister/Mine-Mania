#include "Core.h"
#include "Display.h"
#include "Common.h"
#include "LoadEnums.h"
#include "GameManager.h"
#include "Player.h"
#include "TileEnums.h"

namespace game
{
	extern Display game;
	extern int CoreDestroyedAmount;
	extern GameManager GameHandler;
}



Core::Core() :
health_(2500, 2500, 2, true)
{
	graphic_ = 'C';
	owner_ = "NO_OWNER";
	isDead_ = false;
	m_repair_.SetSound("Repair");
}

Core::~Core()
{

}

void Core::setGraphic(char g)
{
	graphic_ = g;
}

void Core::setOwner(std::string name)
{
	owner_ = name;
}

void Core::updatePos()
{
	game::game.getTileRefAt(pos_).updateOverlay(true, graphic_);
}

bool Core::hasKeyWord(KEYWORD key)
{
	return false;
}

void Core::serialize(std::stringstream & file)
{
	file << L_Core << EndLine
		<< (int)graphic_ << EndLine
		<< isDead_ << EndLine
		<< getID() << EndLine
		<< owner_ << EndLine;
	pos_.serialize(file);
	health_.serialize(file);
}

void Core::serialize(std::fstream & file)
{
	file << L_Core << EndLine
		<< (int)graphic_ << EndLine
		<< isDead_ << EndLine
		<< getID() << EndLine
		<< owner_ << EndLine;
	pos_.serialize(file);
	health_.serialize(file);
}

void Core::deserialize(std::stringstream & file)
{
	int graphic = 0;
	int id = 0;
	file >> graphic
		>> isDead_
		>> id
		>> owner_;
	pos_.deserialize(file);
	health_.deserialize(file);
	graphic_ = graphic;
	setID(id);
}

char Core::getGraphic()
{
	return graphic_;
}

std::string Core::getOwner()
{
	return owner_;
}

void Core::deserialize(std::fstream & file)
{
	int graphic = 0;
	int id = 0;
	file >> graphic
		>> isDead_
		>> id
		>> owner_;
	pos_.deserialize(file);
	health_.deserialize(file);
	graphic_ = graphic;
	setID(id);
}

void Core::update()
{
	health_.update();
	m_repair_.Update();
}

bool Core::hasComponent(int component)
{
	return false;
}

bool Core::isKilled()
{
	return isDead_;
}

void Core::kill()
{
	isDead_ = true;
	clean();
}

void Core::clean()
{
	game::game.getTileRefAt(pos_).removeOverlay();
}

bool Core::damage(int damage, std::string name, bool isServer)
{
	if (owner_ == name) return false;
	if (isServer == false)
	{
		if (isSetToUpdate())
		{
			health_.damage(damage);
			std::stringstream msg;
			msg << SendDefault << EndLine << EntityDamage << EndLine << pos_.serializeR() << damage << EndLine << name << EndLine;
			SendServerLiteral(msg.str());
			if (health_.isDead())
			{
				game::GameHandler.CoreDestroyed(owner_);
				kill();
			}
		}
		else
		{
			std::stringstream msg;
			msg << SendDefault << EndLine << EntityDamage << EndLine << pos_.serializeR() << damage << EndLine << name << EndLine;
			SendServerLiteral(msg.str());
		}
	}
	else
	{
		health_.damage(damage);
		if (isSetToUpdate())
		{
			if (health_.isDead())
			{
				kill();
				game::GameHandler.CoreDestroyed(owner_);
			}
		}
	}
	return true;
}

void Core::setPos(Position iPos)
{
	pos_ = iPos;
}

void Core::updateOverlay()
{
	game::game.getTileRefAt(pos_).updateOverlay(true, graphic_);
}

void Core::updateID()
{
	return;
}

void Core::send()
{
	std::stringstream msg;
	msg << SendDefault << EndLine << EntityAdd << EndLine << ECore << EndLine;
	serialize(msg);
	SendServerLiteral(msg.str());
}

void Core::render()
{
	game::game.getTileRefAt(pos_).updateOverlay(true, graphic_);
	game::game.getTileRefAt(pos_).setOverlayColor(C_Black);
}

void Core::activate(Player* player)
{
	if (player->getName() == owner_)
	{
		if (health_.getHealth() != health_.getMaxHealth())
		{
			health_.heal(1);
			m_repair_.SetTimer(0.200);
		}
	}
}

Position Core::getPos()
{
	return pos_;
}
