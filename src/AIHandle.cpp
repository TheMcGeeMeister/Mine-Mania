#include "..\include\AIHandle.h"
#include <Display.h>
#include <ObjectIds.h>
#include <fstream>

namespace game
{
	extern Display game;
}


AIHandle::AIHandle()
{

}


AIHandle::~AIHandle()
{

}

void AIHandle::addObject(int x, int y)
{
	ObjectProperties newObj;
	newObj.setPosition(x, y);
	objects.push_back(newObj);
	return;
}

void AIHandle::log(std::string txt)
{
	std::fstream file("Logs\\Log.txt", ios::app);
	file << "AIHANDLE:" << txt << endl;
	file.close();
	return;
}

void AIHandle::update()
{
}


/* Object Properties */
//////////////////////////////////////////

void ObjectProperties::setPosition(int x, int y)
{
	pos_(x, y);
}

void ObjectProperties::startTimer(int seconds)
{
	UpdateCoolDown.StartNewTimer(seconds);
}

Position ObjectProperties::getPos()
{
	return pos_;
}

bool ObjectProperties::isReady()
{
	return UpdateCoolDown.Update();
}

//////////////////////////////////////////
