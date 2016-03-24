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

	void setOwner(std::string owner);

	void setPosition(Position pos);

	void setRange(int);


	char getGraphic();

	std::string getOwner();



	virtual void update();
	virtual bool hasComponent(int component);
	virtual bool isKilled();
	virtual void kill();
	virtual void clean();
private:
	std::string owner;

	char graphic;


	bool isDestroyed_;

	TurretAIComponent ai;
};

