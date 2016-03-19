#pragma once
#include "Entity.h"
#include <Timer.h>
#include <Direction.h>
#include <Display.h>

class BulletAIComponent : public Component
{
public:
	BulletAIComponent();
	~BulletAIComponent();

	virtual void update();
private:

};

