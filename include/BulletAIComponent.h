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
	void setDamage(int damage);
	void isDestroyed(bool);
	void setGraphic(char g);

	int getDamage();

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
	int damage;
	bool isDestroyed_;
	bool isCleaned_;
	char graphic_;
};

