#pragma once
#include "Entity.h"
#include "HealthComponent.h"

class Core :
	public Entity
{
public:
	Core();
	~Core();

	void setGraphic(char g);
	void setOwner(std::string name);
	void updatePos();

	void serialize(std::stringstream& file);
	void deserialize(std::stringstream& file);


	virtual void update();
	virtual bool hasComponent(int component);
	virtual bool isKilled();
	virtual void kill();
	virtual void clean();
	virtual void serialize(std::fstream&);
	virtual void deserialize(std::fstream&);
	virtual void damage(int damage, std::string name, bool server=false);
	virtual void setPos(Position);
	virtual void updateOverlay();
	virtual void updateID(); // Not used for core
	virtual void send();
	virtual Position getPos();
private:
	Position pos_;
	bool isDead_;
	char graphic_;
	std::string name_;
	HealthComponent health_;
};

