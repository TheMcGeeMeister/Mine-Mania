#pragma once
#include <iostream>
#include <string>
#include <list>
#include <Timer.h>

class ObjectProperties
{
public:
	
	void setPosition(int x, int y);
private:
	Timer UpdateCoolDown;
	int pos_x;
	int pos_y;
};

class AIHandle
{
public:
	AIHandle();
	~AIHandle();

	void update();
private:
	std::list<ObjectProperties> objects;
};

