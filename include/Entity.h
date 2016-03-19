#pragma once
#include <list>
#include <memory>

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
private:
	int id_;
};

