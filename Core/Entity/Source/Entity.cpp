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

bool Entity::isKilled()
{
	return kill_;
}

System::System()
{
	id_index = 0;
	updateServer_ = false;
}

System::~System()
{
	m_system.clear();
	return;
}

void System::update()
{
	std::list<int> d_queue;
	if (updateServer_ == false)
	{
		if (m_system.empty() == false)
		{
			for (auto& iter : m_system)
			{
				if (iter.second->isSetToUpdate())
				{
					if (iter.second->isKilled() == true)
					{
						d_queue.push_back(iter.first);
						/* Debug */
						/////////////////////////////////
						std::fstream file("Logs\\Log.txt", std::ios::app);
						file << "System: Entity Deleted ID:" << iter.first << std::endl;
						/////////////////////////////////
						continue;
					}
					else
					{
						iter.second->update();
					}
				}
			}
		}
	}
	else
	{
		if (m_system.empty() == false)
		{
			std::stringstream msg;
			for (auto& iter : m_system)
			{
				if (iter.second->isSetToUpdate())
				{
					if (iter.second->isKilled() == true)
					{
						d_queue.push_back(iter.first);
						/* Debug */
						/////////////////////////////////
						std::fstream file("Logs\\Log.txt", std::ios::app);
						file << "System: Entity Deleted ID:" << iter.first << std::endl;
						/////////////////////////////////
						msg << SendDefault << EndLine  << EntityKill << EndLine  << iter.second->getPos().serializeR();
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
	}

	for (auto& iter : d_queue)
	{
		m_system.erase(iter);
	}
}

void System::setUpdateServer(bool serverUpdate)
{
	updateServer_ = serverUpdate;
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

int System::addEntity(std::shared_ptr<Entity> entity)
{
	m_system[id_index] = entity;
	/* Debug */
	/////////////////////////////////
	std::fstream file("Logs\\Log.txt", std::ios::app);
	file << "System: Entity Created ID:" << id_index << std::endl;
	/////////////////////////////////
	id_index++;
	return (id_index - 1);
}

int System::addEntity(std::shared_ptr<Entity> entity, std::string txt)
{
	m_system[id_index] = entity;
	/* Debug */
	/////////////////////////////////
	std::fstream file("Logs\\Log.txt", std::ios::app);
	file << "System: Entity Created ID:" << id_index << " <" << txt.c_str() << ">" << std::endl;
	/////////////////////////////////
	id_index++;
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

bool System::getEntity(int id, Entity ** entity)
{
	if (m_system.count(id))
	{
		*entity = m_system[id].get();
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

void System::clear()
{
	m_system.clear();
	id_index = 0;
}
/*int System::addPlayer(std::shared_ptr<Player> player)
{
	m_players[id_player_index] = player;
	id_player_index;
	
	/* Debug 
	/////////////////////////////////
	std::fstream file("Logs\\Log.txt", std::ios::app);
	file << "System: Player Created ID:" << id_index << std::endl;
	/////////////////////////////////

	return (id_player_index - 1);
}

bool System::getPlayerAt(Position pos, Player & player)
{
	for (auto& iter : m_players)
	{
		if (iter.second->getHandPosition() == pos)
		{
			player = iter.second.get;
			return true;
		}
	}
	return false;
}

bool System::playerAt(Position pos)
{
	for (auto& iter : m_players)
	{
		if (pos == iter.second->getHandPosition())
		{
			return true;
		}
	}
	return false;
}*/
