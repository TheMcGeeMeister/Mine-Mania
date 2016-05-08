#include <fstream>
#include "Bullet.h"
#include "LoadEnums.h"
#include "Common.h"

Bullet::Bullet()
{
	ai.setDamage(5);
	ai.setGraphicNoUpdate('@');
	ai.setPositionNoUpdate(Position(0, 0));
	addKeyWord(KEYWORD_BULLET);
}

Bullet::Bullet(Position sPos, double damage, DIRECTION direction)
{
	ai.setDamage(damage);
	ai.setDirection(direction);
	ai.setPositionNoUpdate(sPos);
}

Bullet::~Bullet()
{

}

void Bullet::setDamage(double damage)
{
	ai.setDamage(damage);
}

void Bullet::setBulletRange(int range)
{
	ai.setRange(range);
}

double Bullet::getDamage()
{
	return ai.getDamage();
}

void Bullet::setDirection(DIRECTION direction)
{
	ai.setDirection(direction);
}

void Bullet::setPosition(Position pos)
{
	ai.setPosition(pos);
}

void Bullet::setPositionNoUpdate(Position pos)
{
	ai.setPositionNoUpdate(pos);
}

void Bullet::setID(int id)
{
	ai.setID(id);
	getIDRef() = id;
}

void Bullet::setGraphic(char g)
{
	ai.setGraphic(g);
}

void Bullet::setGraphicNoUpdate(char g)
{
	ai.setGraphicNoUpdate(g);
}

DIRECTION Bullet::getDirection()
{
	return ai.getDirection();
}

void Bullet::serialize(std::stringstream & file)
{
	file << L_Bullet << std::endl
		<< getID() << std::endl;
	ai.serialize(file);
}

void Bullet::deserialize(std::stringstream & file)
{
	int id;
	file >> id;
	setID(id);
	ai.deserialize(file);
}

void Bullet::serialize(fstream & file)
{
	file << L_Bullet << std::endl
		<< getID() << std::endl;
	ai.serialize(file);
}

void Bullet::deserialize(fstream & file)
{
	int id;
	file >> id;
	setID(id);
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

bool Bullet::damage(int damage, std::string name, bool server)
{
	return false;
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
