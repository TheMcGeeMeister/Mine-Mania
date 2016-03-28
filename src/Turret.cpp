#include "..\include\Turret.h"
#include "LoadEnums.h"
#include <ObjectIds.h>
#include <Display.h>
#include <TileChangeManager.h>
#include <fstream>

namespace game
{
	extern Display game;
	extern TileChangeManager TileHandler;
}


Turret::Turret()
{
	isDestroyed_ = false;
	graphic = ' ';
	owner = "None";
}

Turret::Turret(Position position, std::string owner)
{
	this->owner = owner;
	ai.setPosition(position);
	isDestroyed_ = false;
	graphic = ' ';
	owner = "None";
}


Turret::~Turret()
{
}

void Turret::setGraphic(char g)
{
	graphic = g;
	Tile& tile = game::game.getTileRefAt(ai.getPosition());
	tile.setOverlayEnabled(true);
	tile.setOverlayGraphic(g);
}

void Turret::setOwner(std::string owner)
{
	this->owner = owner;
	ai.setOwner(owner);
}

void Turret::setPosition(Position pos)
{
	ai.setPosition(pos);
}

void Turret::setRange(int range)
{
	ai.setVisionRange(range);
}

void Turret::setShootCoolDown(int seconds)
{
	ai.setShootCoolDownTime(seconds);
}

char Turret::getGraphic()
{
	return graphic;
}

std::string Turret::getOwner()
{
	return owner;
}

void Turret::serialize(fstream & file)
{
	file << LOAD::L_Turret << std::endl
		<< owner << std::endl
		<< (int)graphic << std::endl
		<< isDestroyed_ << std::endl;
	ai.serialize(file);
}

void Turret::deserialize(fstream & file)
{
	int graphic_;
	file >> owner
		>> graphic_
		>> isDestroyed_;
	ai.deserialize(file);
	graphic = graphic_;
}

void Turret::update()
{
	if (isDestroyed_ == true) return;
	ai.update();
}

bool Turret::hasComponent(int component)
{
	switch (component)
	{
	case TURRET_ID: return true;
	default: return false;
	}
}

bool Turret::isKilled()
{
	return isDestroyed_;
}

void Turret::kill()
{
	isDestroyed_ = true;
	clean();
}

void Turret::clean()
{
	Tile &tile = game::game.getTileRefAt(ai.getPosition());
	tile.removeOverlay();
	game::TileHandler.push_back(ai.getPosition());
}

void Turret::setPos(Position pos)
{
	ai.setPosition(pos);
}

Position Turret::getPos()
{
	return ai.getPosition();
}
