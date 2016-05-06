#pragma once
#include <string>
#include <fstream>
#include "Entity.h"
#include "TurretAIComponent.h"
#include "HealthComponent.h"
#include "Position.h"

class Turret : public Entity
{
public:
	Turret();
	Turret(Position position, std::string name);
	~Turret();

	HealthComponent& getHealthRef();

	void setGraphic(char g);
	void setOwner(std::string owner);
	void setPosition(Position pos);
	void setRange(int);
	void setShootCoolDown(int);


	char getGraphic();

	std::string getOwner();

	void serialize(std::stringstream& file);
	void deserialize(std::stringstream& file);


	virtual void serialize(std::fstream& file);
	virtual void deserialize(std::fstream& file);



	virtual void update();
	virtual bool hasComponent(int component);
	virtual bool isKilled();
	virtual void kill();
	virtual void clean();
	virtual bool damage(int amount, std::string name, bool server=false);
	virtual void setPos(Position pos);
	virtual void updateOverlay();
	virtual void updateID();
	virtual void send();
	virtual Position getPos();
private:
	std::string owner;

	HealthComponent health;

	char graphic;


	bool isDestroyed_;

	TurretAIComponent ai;
};

