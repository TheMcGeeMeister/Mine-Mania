#include "Core.h"
#include "Display.h"
#include "Common.h"
#include "LoadEnums.h"

namespace game
{
	extern Display game;
}



Core::Core()
{
	pos_(255, 255);
	health_.setHealth(2500);
	health_.getHealthRegenRef() = 2;
	graphic_ = 'C';
	name_ = "NO_OWNER";
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
	name_ = name;
}

void Core::updatePos()
{
	game::game.getTileRefAt(pos_).updateOverlay(true, graphic_);
}

void Core::serialize(std::stringstream & file)
{
	file << L_Core << EndLine
		 << (int)graphic_ << EndLine
		 << isDead_ << EndLine;
	health_.serialize(file);
}

void Core::deserialize(std::stringstream & file)
{
	int g;
	file >> g >> isDead_;
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
		<<(int)graphic_ << EndLine 
		<< isDead_ << EndLine;
	health_.serialize(file);
}

void Core::deserialize(std::fstream & file)
{
	int g;
	file >> g >> isDead_;
	graphic_ = g;
	health_.deserialize(file);
}

void Core::damage(int damage, std::string name)
{
	if (isSetToUpdate())
	{
		if (name_ == name) return;
		health_.damage(damage);
		if (health_.isDead())
		{
			kill();
		}
	}
	else
	{
		std::stringstream msg;
		msg << SendDefault << EndLine << EntityDamage << EndLine << pos_.serializeR() << damage << EndLine  << name << EndLine;
		SendServerLiteral(msg.str());
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
	return Position();
}
