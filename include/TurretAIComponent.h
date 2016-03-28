#pragma once
#include "Entity.h"
#include <Position.h>
#include <Timer.h>
#include <fstream>

class TurretAIComponent : public Component
{
public:
	TurretAIComponent();
	~TurretAIComponent();

	void setPosition(Position Position);
	void setShootCoolDownTime(int Seconds);
	void setVisionRange(int range);
	void setOwner(std::string owner);

	void search();
	void searchLine(Position sPos, DIRECTION direction, int amount, Position& fPos, bool& isFind);
	void shoot(DIRECTION direction);

	Position getPosition();
	std::string getOwner();

	virtual void serialize(std::fstream& file);
	virtual void deserialize(std::fstream& file);

	virtual void update();
private:
	int visionRange_;
	int shootCoolDownTime_;
	Timer shootCoolDown_;
	Timer targetSearchCoolDown_;
	bool targetFound_;
	Position targetPosition_;
	Position curPosition_;
	std::string owner_;
};

