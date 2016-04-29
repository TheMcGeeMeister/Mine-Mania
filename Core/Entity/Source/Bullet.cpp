#include <fstream>
#include "Bullet.h"
#include "LoadEnums.h"
#include "Common.h"

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

void Bullet::setBulletRange(int range)
{
	ai.setRange(range);
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

void Bullet::serialize(std::stringstream & file)
{
	file << L_Bullet << std::endl
		<< damage_ << std::endl;
	ai.serialize(file);
}

void Bullet::deserialize(std::stringstream & file)
{
	file >> damage_;
	ai.deserialize(file);
}

void Bullet::serialize(fstream & file)
{
	file << L_Bullet << std::endl
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

void Bullet::updateID()
{
	ai.setID(getID());
}

void Bullet::send()
{
	std::stringstream msg;
	msg << SendDefault << EndLine << EntityAdd << EndLine << EBullet << EndLine; serialize(msg);
	SendServerLiteral(msg.str());
}

Position Bullet::getPos()
{
	return ai.getPosition();
}
