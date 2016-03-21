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
	void setRange(int range);
	void isDestroyed(bool);
	void setGraphic(char g);

	void clean();

	bool isDestroyed();

	DIRECTION getDirection();
	Position getPosition();

	virtual void update();
private:
	Timer movementCoolDown;
	double movementCoolDownTime;
	DIRECTION direction;
	Position position;
	int range;
	int rangeIndex;
	bool isDestroyed_;
	bool isCleaned_;
	char graphic_;
};

