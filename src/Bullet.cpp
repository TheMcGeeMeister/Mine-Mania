#include "..\include\Bullet.h"
#include <fstream>

Bullet::Bullet()
{
	damage_ = 5;
	ai.setGraphic('@');
}

Bullet::Bullet(Position sPos, double damage, DIRECTION direction)
{
	this->damage_ = damage;
	ai.setDirection(direction);
	ai.setPosition(sPos);
}

Bullet::~Bullet()
{
}

void Bullet::setDamage(double damage)
{
	this->damage_ = damage;
}

double Bullet::getDamage()
{
	return damage_;
}

void Bullet::setDirection(DIRECTION direction)
{
	ai.setDirection(direction);
}

void Bullet::setPosition(Position pos)
{
	ai.setPosition(pos);
}

void Bullet::setGraphic(char g)
{
	ai.setGraphic(g);
}

DIRECTION Bullet::getDirection()
{
	return ai.getDirection();
}

void Bullet::serialize(fstream & file)
{
	file << "Bullet" << std::endl
		<< damage_ << std::endl;
	ai.serialize(file);
}

void Bullet::deserialize(fstream & file)
{
	file >> damage_;
	ai.deserialize(file);
}

void Bullet::update()
{
	if (ai.isDestroyed() == true)
	{
		ai.clean();
		return;
	}
	ai.update();
}

bool Bullet::hasComponent(int id)
{
	return false;
}

bool Bullet::isKilled()
{
	return ai.isDestroyed();
}

void Bullet::kill()
{
	ai.clean();
	ai.isDestroyed(true);
}

void Bullet::clean()
{
	ai.clean();
}

void Bullet::damage(int damage, std::string name)
{
	return;
}

void Bullet::setPos(Position pos)
{
	ai.setPosition(pos);
}

void Bullet::updateOverlay()
{
	ai.updateOverlay();
}

Position Bullet::getPos()
{
	return ai.getPosition();
}
