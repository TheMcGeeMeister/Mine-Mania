#pragma once
#include "BulletAIComponent.h"
#include "Position.h"

class Bullet : public Entity
{
public:
	Bullet();
	Bullet(Position sPos, double damage, DIRECTION direction);
	~Bullet();

	void setDamage(double damage);
	void setBulletRange(int range);
	
	double getDamage();

	void setDirection(DIRECTION direction);
	void setPosition(Position pos);
	void setPositionNoUpdate(Position pos);

	virtual void setID(int id);

	void setGraphic(char g);

	DIRECTION getDirection();

	void serialize(std::stringstream&);
	void deserialize(std::stringstream&);

	virtual void serialize(fstream&);
	virtual void deserialize(fstream&);

	virtual void update();
	virtual bool hasComponent(int id);
	virtual bool isKilled();
	virtual void kill();
	virtual void clean();
	virtual bool damage(int damage, std::string name, bool server=false);
	virtual void setPos(Position pos);
	virtual void updateOverlay();
	virtual void updateID();
	virtual void send();
	virtual Position getPos();
private:
	BulletAIComponent ai;
};

