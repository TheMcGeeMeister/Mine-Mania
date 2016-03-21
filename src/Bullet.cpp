#include "..\include\Bullet.h"



Bullet::Bullet(Position sPos, double damage, DIRECTION direction)
{
	this->damage = damage;
	ai.setDirection(direction);
	ai.setPosition(sPos);
}

Bullet::~Bullet()
{
}

void Bullet::setDamage(double damage)
{
	this->damage = damage;
}

double Bullet::getDamage()
{
	return damage;
}

void Bullet::update()
{
	ai.update();
}

bool Bullet::hasComponent()
{
	return false;
}
