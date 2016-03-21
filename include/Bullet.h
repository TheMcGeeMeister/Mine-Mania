#pragma once
#include <BulletAIComponent.h>
#include <Position.h>

class Bullet
{
public:
	Bullet(Position sPos, double damage, DIRECTION direction);
	~Bullet();

	void setDamage(double damage);
	
	double getDamage();

	virtual void update();
	virtual bool hasComponent();

private:
	double damage;

	BulletAIComponent ai;
};

