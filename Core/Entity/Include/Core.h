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


	virtual void update();
	virtual bool hasComponent(int component);
	virtual bool isKilled();
	virtual void kill();
	virtual void clean();
	virtual void serialize(std::fstream&);
	virtual void deserialize(std::fstream&);
	virtual void damage(int damage, std::string name);
	virtual void setPos(Position);
	virtual void updateOverlay();
	virtual Position getPos();
private:
	Position pos_;
	bool isDead_;
	char graphic_;
	std::string name_;
	HealthComponent health_;
};

