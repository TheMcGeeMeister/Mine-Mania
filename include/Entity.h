#pragma once
#include <list>
#include <memory>
#include <map>
#include <Position.h>

class Component
{
public:
	virtual void update() = 0;
private:

};

class Entity
{
public:
	Entity();
	~Entity();

	virtual void update() = 0;

	void setID(int id);
	void setPos(Position pos);
	void kill();

	int getID();
	bool isKilled();
	Position getPos();
private:
	int id_;
	bool kill_;
	Position position_;
};

class System
{
public:
	System();
	~System();

	void update();
	
	int addEntity(std::shared_ptr<Entity>);
private:
	int id_index;
	std::map<int, std::shared_ptr<Entity>> m_system;
};
