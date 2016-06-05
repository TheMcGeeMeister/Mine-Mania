#include <list>
#include <fstream>
#include <sstream>
#include "Entity.h"
#include "LoadEnums.h"
#include "Packet.h"
#include "Common.h"


Entity::Entity()
{
	kill_ = false;
	isObjectHosted_ = true;
}


Entity::~Entity()
{

}

void Entity::setToUpdate()
{
	isObjectHosted_ = true;
}

void Entity::setToNoUpdate()
{
	isObjectHosted_ = false;
}

void Entity::setID(int id)
{
	id_ = id;
}

void Entity::addKeyWord(KEYWORD key)
{
	keywords_.push_back(key);
}

bool Entity::isSetToUpdate()
{
	return isObjectHosted_;
}

bool Entity::hasKeyWord(KEYWORD key)
{
	for (auto& iter : keywords_)
	{
		if (iter == key)
		{
			return true;
		}
	}
	return false;
}

void Entity::kill()
{
	kill_ = true;
}

int Entity::getID()
{
	return id_;
}

int & Entity::getIDRef()
{
	return id_;
}

bool Entity::isKilled()
{
	return kill_;
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
	if (m_system.empty() == false)
	{
		for (auto& iter : m_system)
		{
			if (iter.second->isSetToUpdate())
			{
				if (iter.second->isKilled() == true)
				{
					d_queue.push_back(iter.first);

					std::stringstream msg;
					msg << SendDefault << EndLine << EntityKill << EndLine << iter.first << EndLine;
					SendServerLiteral(msg.str());
					msg.str(std::string());

					continue;
				}
				else
				{
					iter.second->update();
				}
			}
		}
	}
	for (auto& iter : d_queue)
	{
		m_system.erase(iter);
	}
}

void System::cleanAndUpdateOverlays() // Disables All Overlays, then calls on the entity's to update them
{
	Common::CleanGameOverlay();
	for (auto& iter : m_system)
	{
		iter.second->updateOverlay();
	}
}

bool System::entityAt(Position pos)
{
	for (auto& iter : m_system)
	{
		if (iter.second->getPos() == pos)
		{
			return true;
		}
	}
	return false;
}

int System::addEntity(std::shared_ptr<Entity> entity, bool send)
{
	entity->setID(id_index);
	m_system[id_index] = entity;

	id_index++;
	if (send)
	{
		if (Common::isConnected() == true)
		{
			entity->send();
		}
	}
	entity->render();
	return (id_index - 1);
}

int System::addEntityServer(std::shared_ptr<Entity> entity)
{
	m_server[entity->getID()] = id_index;
	entity->setID(id_index);
	m_system[id_index] = entity;


	id_index++;
	return (id_index - 1);
}

int System::addEntity(std::shared_ptr<Entity> entity, std::string txt, bool send)
{
	entity->setID(id_index);
	m_system[id_index] = entity;

	id_index++;
	if (send)
	{
		if (Common::isConnected() == true)
		{
			entity->send();
		}
	}
	return (id_index - 1);
}

bool System::getEntityAt(Position pos, Entity ** entity)
{
	for (auto& iter : m_system)
	{
		if (iter.second->getPos() == pos)
		{
			*entity = iter.second.get();
			return true;
		}
	}
	entity = nullptr;
	return false;
}

bool System::getEntityServer(int i_id, Entity ** entity)
{
	int id = m_server[i_id];
	if (m_system.count(id))
	{
		*entity = m_system[id].get();
		return true;
	}
	return false;
}

bool System::getEntity(int id, Entity ** entity)
{
	if (m_system.count(id))
	{
		*entity = m_system[id].get();
		return true;
	}
	return false;
}

bool System::deleteEntity(int id)
{
	if (m_system.count(id))
	{
		m_system[id]->kill();
		m_system.erase(id);
		return true;
	}
	else
	{
		return false;
	}
}

bool System::deleteEntityServer(int id)
{
	if (m_system.count(m_server[id]))
	{
		m_system[m_server[id]]->kill();
		m_system.erase(m_server[id]);
		m_server.erase(id);
		return true;
	}
	return false;
}

void System::serialize(std::fstream & file)
{
	for (auto& iter : m_system)
	{
		iter.second->serialize(file);
	}
}

void System::sendAll()
{
	for (auto& iter : m_system)
	{
		iter.second->send();
	}
}

void System::clear()
{
	m_system.clear();
	id_index = 0;
}
