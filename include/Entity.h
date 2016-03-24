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
	void setPos(Position pos);

	int getID();
	Position getPos();

	virtual bool hasComponent(int component) = 0;
	virtual bool isKilled() = 0;
	virtual void kill() = 0;
	virtual void clean() = 0;
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

	int addEntity(std::shared_ptr<Entity>, std::string txt);
	
	//int addPlayer(std::shared_ptr<Player>);

	//bool getPlayerAt(Position pos, Player& player);

	//bool playerAt(Position pos);
private:
	int id_index;
	int id_player_index;
	std::map<int, std::shared_ptr<Entity>> m_system;
	//std::map<int, std::shared_ptr<Player>> m_players;
};

