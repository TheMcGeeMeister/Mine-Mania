#include "Core.h"
#include "Display.h"
#include "Common.h"
#include "LoadEnums.h"
#include "GameManager.h"

namespace game
{
	extern Display game;
	extern int CoreDestroyedAmount;
	extern GameManager GameHandler;
}



Core::Core() :
health_(2500, 2500, 2, true)
{
	pos_(0, 0);
	graphic_ = 'C';
	owner_ = "NO_OWNER";
}


Core::~Core()
{

}

void Core::setGraphic(char g)
{
	graphic_ = g;
	game::game.getTileRefAt(pos_).updateOverlay(true, g);
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
		<< pos_.serializeR() << EndLine
		<< owner_ << EndLine;
	health_.serialize(file);
}

void Core::deserialize(std::stringstream & file)
{
	int g;
	int id;
	file >> g >> isDead_ >> id; pos_.deserialize(file);
	file >> owner_;
	setID(id);
	graphic_ = g;
	health_.deserialize(file);
}

void Core::update()
{
	health_.update();
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

void Core::serialize(std::fstream & file)
{
	file << L_Core << EndLine
		<< (int)graphic_ << EndLine
		<< isDead_ << EndLine
		<< getID() << EndLine
		<< pos_.serializeR() << EndLine
		<< owner_ << EndLine;
	health_.serialize(file);
}

void Core::deserialize(std::fstream & file)
{
	int g;
	int id;
	file >> g >> isDead_ >> id; pos_.deserialize(file);
	file >> owner_;
	setID(id);
	graphic_ = g;
	health_.deserialize(file);
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
	}
}

void Core::setPos(Position iPos)
{
	game::game.getTileRefAt(pos_).updateOverlay(false, ' ');
	pos_ = iPos;
	game::game.getTileRefAt(iPos).updateOverlay(true, graphic_);
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
	msg << SendDefault << EndLine << EntityAdd << EndLine << ECore << EndLine; serialize(msg);
	SendServerLiteral(msg.str());
}

Position Core::getPos()
{
	return pos_;
}
