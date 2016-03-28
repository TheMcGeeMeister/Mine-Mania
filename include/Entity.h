#pragma once
#include <list>
#include <memory>
#include <map>
#include <Position.h>

enum COMPONENT
{
	COMPONENT_HEALTH, COMPONENT_HITTABLE
};

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

	int getID();

	virtual bool hasComponent(int component) = 0;
	virtual bool isKilled() = 0;
	virtual void kill() = 0;
	virtual void clean() = 0;
	virtual void serialize(std::fstream&) = 0;
	virtual void deserialize(std::fstream&) = 0;
	virtual void setPos(Position) = 0;
	virtual Position getPos() = 0;
private:
	int id_;
	bool kill_;
};

class System
{
public:
	System();
	~System();

	void update();

	bool entityAt(Position pos);
	
	int addEntity(std::shared_ptr<Entity>);

	int addEntity(std::shared_ptr<Entity>, std::string txt);

	bool getEntityAt(Position pos, Entity* entity);

	void serialize(std::fstream&);

	void clear();
private:
	int id_index;
	std::map<int, std::shared_ptr<Entity>> m_system;
};

