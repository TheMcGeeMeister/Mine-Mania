#pragma once
#include "Entity.h"
#include <Timer.h>
#include <Direction.h>
#include <Display.h>

class BulletAIComponent : public Component
{
public:
	BulletAIComponent();
	~BulletAIComponent();

	void setDirection(DIRECTION direction);
	void setPosition(Position position);

	DIRECTION getDirection();
	Position getPosition();

	virtual void update();
private:
	Timer movementCoolDown;
	double movementCoolDownTime;
	DIRECTION direction;
	Position position;
};

