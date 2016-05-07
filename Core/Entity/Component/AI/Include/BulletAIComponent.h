#pragma once
#include "Entity.h"
#include "Timer.h"
#include "Direction.h"
#include "Display.h"

class BulletAIComponent : public Component
{
public:
	BulletAIComponent();
	~BulletAIComponent();

	void setDirection(DIRECTION direction);
	void setPosition(Position position);
	void setPositionNoUpdate(Position position);
	void setRange(int range);
	void setDamage(int damage);
	void isDestroyed(bool);
	void setGraphic(char g);
	void setGraphicNoUpdate(char g);
	void updateOverlay();
	void setID(int id);

	int getDamage();

	void clean();

	bool isDestroyed();

	DIRECTION getDirection();
	Position getPosition();

	void serialize(fstream& file);
	void serialize(std::stringstream& file);
	void deserialize(fstream& file);
	void deserialize(std::stringstream& file);

	virtual void update();
private:
	Timer movementCoolDown;
	double movementCoolDownTime;
	double realMovementCoolDownTime;
	DIRECTION direction;
	Position position;
	int range;
	int rangeIndex;
	int damage;
	int id;
	bool isDestroyed_;
	bool isCleaned_;
	char graphic_;
};

