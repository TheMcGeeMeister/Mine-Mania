#pragma once
#include <Entity.h>
#include <Timer.h>

class HealthComponent : public Component
{
public:
	HealthComponent();
	~HealthComponent();

	void setHealth(double health);
	void setMaxHealth(double maxHealth);
	void setHealthRegen(double healthRegen);
	void isRegenEnabled(bool isRegenEnabled);

	double getHealth() const;
	double getMaxHealth() const;
	double getHealthRegen() const;
	bool isRegenEnabled() const;

	double& getHealthRef();
	double& getMaxHealthRef();
	double& getHealthRegenRef();

	void damage(double amount);
	void heal(double amount);
	void forceHealth(double amount);

	virtual void update();
private:
	double health_;
	double maxHealth_;
	double healthRegen_;
	Timer regenCoolDown;
	bool isRegenEnabled_;
};

