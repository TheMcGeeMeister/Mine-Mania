#include "..\include\Turret.h"
#include <ObjectIds.h>


Turret::Turret()
{
}

Turret::Turret(Position position, std::string owner)
{
	this->position = position;
	this->owner = owner;
	ai.setPosition(position);
}


Turret::~Turret()
{
}

void Turret::setGraphic(char g)
{
	graphic = g;
}

char Turret::getGraphic()
{
	return graphic;
}

void Turret::update()
{
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
