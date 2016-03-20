#include "..\include\Entity.h"
#include <list>



Entity::Entity()
{
	kill_ = false;
}


Entity::~Entity()
{

}

void Entity::setID(int id)
{
	id_ = id;
}

void Entity::setPos(Position pos)
{
	position_ = pos;
}

void Entity::kill()
{
	kill_ = true;
}

int Entity::getID()
{
	return id_;
}

bool Entity::isKilled()
{
	return kill_;
}

Position Entity::getPos()
{
	return position_;
}

System::System()
{
	id_index = 0;
}

System::~System()
{
	m_system.clear();
	return;
}

void System::update()
{
	std::list<int> d_queue;
	for (auto& iter : m_system)
	{
		if (iter.second->isKilled() == true)
		{
			d_queue.push_back(iter.first);
			continue;
		}
		else
		{
			iter.second->update();
		}
	}

	for (auto& iter : d_queue)
	{
		m_system.erase(iter);
	}
}

int System::addEntity(std::shared_ptr<Entity> entity)
{
	m_system[id_index] = entity;
	id_index;
	return (id_index - 1);
}
