#pragma once
#include "Entity.h"
#include <Position.h>
#include <Timer.h>

class TurretAIComponent : public Component
{
public:
	TurretAIComponent();
	~TurretAIComponent();

	void setPosition(Position Position);
	void setShootCoolDownTime(int Seconds);

	void search();
	void shoot(DIRECTION Direction, Position StartPosition);

	virtual void update();
private:
	int visionRange_;
	int shootCoolDownTime_;
	Timer shootCoolDown_;
	Timer targetSearchCoolDown_;
	bool targetFound_;
	Position targetPosition_;
	Position curPosition_;
};

