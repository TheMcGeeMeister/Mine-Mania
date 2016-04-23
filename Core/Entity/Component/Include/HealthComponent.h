#pragma once
#include <Entity.h>
#include <Timer.h>
#include <fstream>

class HealthComponent : public Component
{
public:
	HealthComponent();
	~HealthComponent();

	void setHealth(double health);
	void setMaxHealth(double maxHealth);
	void setHealthRegen(double healthRegen);
	void isRegenEnabled(bool isRegenEnabled);
	void isDead(bool isDead);

	double getHealth() const;
	double getMaxHealth() const;
	double getHealthRegen() const;
	bool isRegenEnabled() const;
	bool isDead() const;

	double& getHealthRef();
	double& getMaxHealthRef();
	double& getHealthRegenRef();

	void damage(double amount);
	void heal(double amount);
	void forceHealth(double amount);

	void reset();

	void serialize(std::fstream&);
	void serialize(std::stringstream&);
	void deserialize(std::fstream&);
	void deserialize(std::stringstream&);

	virtual void update();
private:
	double health_;
	double maxHealth_;
	double healthRegen_;
	Timer regenCoolDown;
	bool isRegenEnabled_;
	bool isDead_;
};

