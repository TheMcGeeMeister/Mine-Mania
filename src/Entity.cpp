#include "..\include\Entity.h"
#include <list>
#include <fstream>



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
	id_player_index = 0;
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

	for (auto& iter : d_queue)
	{
		m_system.erase(iter);
	}
}

int System::addEntity(std::shared_ptr<Entity> entity)
{
	m_system[id_index] = entity;
	id_index++;
	/* Debug */
	/////////////////////////////////
	std::fstream file("Logs\\Log.txt", std::ios::app);
	file << "System: Entity Created ID:" << id_index << std::endl;
	/////////////////////////////////
	return (id_index - 1);
}

int System::addEntity(std::shared_ptr<Entity> entity, std::string txt)
{
	m_system[id_index] = entity;
	id_index++;
	/* Debug */
	/////////////////////////////////
	std::fstream file("Logs\\Log.txt", std::ios::app);
	file << "System: Entity Created ID:" << id_index << " <" << txt.c_str() << ">" << std::endl;
	/////////////////////////////////
	return (id_index - 1);
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
