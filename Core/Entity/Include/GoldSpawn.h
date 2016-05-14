#pragma once
#include <windows.h>
#include "Entity.h"

class GoldSpawn : public Entity
{
public:
	GoldSpawn();
	~GoldSpawn();

	std::string getOwner();
	void setOwner(std::string owner);

	void serialize(std::stringstream& stream);
	void deserialize(std::stringstream& stream);

	virtual void update();

	virtual bool hasComponent(int component);
	virtual bool isKilled();
	virtual void kill();
	virtual void clean();
	virtual void serialize(std::fstream&);
	virtual void deserialize(std::fstream&);
	virtual bool damage(int damage, std::string name, bool server = false);
	virtual void setPos(Position);
	virtual void updateOverlay();
	virtual void updateID();
	virtual void send();
	virtual void render();
	virtual void activate(Player* player);
	virtual Position getPos();
private:
	Position pos_;
	int claimedPercentage_;
	std::string currentClaimer_;
	std::string owner_;
	WORD color_;
	bool isClaimed_;
	bool isKilled_;
};

