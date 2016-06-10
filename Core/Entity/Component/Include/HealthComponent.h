#pragma once
#include <fstream>
#include "Entity.h"
#include "Timer.h"

class HealthComponent : public Component
{
public:
	HealthComponent();
	HealthComponent(double health, double maxHealth, double healthRegen, bool isRegenEnabled);
	~HealthComponent();

	void setHealth(double health);
	void setMaxHealth(double maxHealth);
	void setHealthRegen(double healthRegen);
	void setIDPtr(int* id_ptr);
	void isRegenEnabled(bool isRegenEnabled);
	void isRegenDelayed(bool isRegenDelayed);
	void isDead(bool isDead);

	double getHealth() const;
	double getMaxHealth() const;
	double getHealthRegen() const;
	bool isRegenEnabled() const;
	bool isRegenDelayed() const;
	bool isDead() const;
	bool isFull() const;

	double& getHealthRef();
	double& getMaxHealthRef();
	double& getHealthRegenRef();

	void damage(double amount);
	void heal(double amount);
	void forceHealth(double amount);
	void delayHealing(double timeInSeconds);

	void reset();

	void serialize(std::fstream&);
	void serialize(std::stringstream&);
	void deserialize(std::fstream&);
	void deserialize(std::stringstream&);

	void operator()(double health, double maxHealth, double healthRegen, bool isRegenEnabled);

	virtual void update();
private:
	double health_;
	double maxHealth_;
	double healthRegen_;
	Timer regenCoolDown;
	bool isRegenEnabled_;
	bool isRegenDelayed_;
	bool isDead_;
	int* id_;
};

