#pragma once
#include <iostream>
#include <string>
#include <list>
#include <Timer.h>
#include <Position.h>

class ObjectProperties
{
public:
	
	void setPosition(int x, int y);
	void startTimer(int seconds);
	Position getPos();
	bool isReady();
private:
	Timer UpdateCoolDown;
	Position pos_;
};

class AIHandle
{
public:
	AIHandle();
	~AIHandle();

	void addObject(int x, int y);

	void log(std::string txt);

	void update();
private:
	std::list<ObjectProperties> objects;
};

