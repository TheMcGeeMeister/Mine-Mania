#pragma once
#include "Entity.h"

class PlayerStatComponent :
	public Component
{
public:
	PlayerStatComponent();
	~PlayerStatComponent();

	int getLevel();
	int getMaxLevel();
	double getExp();
	double getMaxExp();
	int getStength();
	int getStamina();
	int getMaxStamina();
	int getMagicka();
	int getMaxMagicka();
	double getSpeedMultiplyer();

	bool addExp(double exp); // Returns true if you leveled

	void setLevel(int level);
	void setLevelWithScale(int level);
	void setMaxLevel(int maxLevel);
	void setStamina(int stamina);
	void setMaxStamina(int stamina);
	void setMagicka(int magicka);
	void setMaxMagicka(int magicka);
	void setStrength(int stength);
	void setSpeed(double speed);

	virtual void update();
private:
	int level_;
	int maxLevel_;
	double exp_;
	double expNeed_;
	int strength_;
	int stamina_;
	int maxStamina_;
	int magicka_;
	int maxMagicka_;
	double speed_;
};

