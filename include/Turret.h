#pragma once
#include "Entity.h"
#include <string>
#include <TurretAIComponent.h>
#include <Position.h>

class Turret : public Entity
{
public:
	Turret();
	Turret(Position position, std::string name);
	~Turret();


	void setGraphic(char g);


	char getGraphic();



	virtual void update();
	virtual bool hasComponent(int component);
private:
	std::string owner;

	char graphic;

	Position position;

	TurretAIComponent ai;
};

