#include "..\include\Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{

}

void Entity::setID(int id)
{
	id_ = id;
}

int Entity::getID()
{
	return id_;
}
