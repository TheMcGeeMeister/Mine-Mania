#include "..\include\HealthComponent.h"



HealthComponent::HealthComponent()
{
	health_ = 100;
	maxHealth_ = 100;
	healthRegen_ = 5;
	isRegenEnabled_ = true;
}


HealthComponent::~HealthComponent()
{
}

void HealthComponent::setHealth(double health)
{
	health_ = health;
}

void HealthComponent::setMaxHealth(double maxHealth)
{
	double healthPerc = health_ / maxHealth_;
	maxHealth_ = maxHealth;
	health_ = maxHealth * healthPerc;
}

void HealthComponent::setHealthRegen(double healthRegen)
{
	healthRegen_ = healthRegen;
}

void HealthComponent::isRegenEnabled(bool isRegenEnabled)
{
	isRegenEnabled_ = isRegenEnabled;
}

double HealthComponent::getHealth() const
{
	return health_;
}

double HealthComponent::getMaxHealth() const
{
	return maxHealth_;
}

double HealthComponent::getHealthRegen() const
{
	return healthRegen_;
}

bool HealthComponent::isRegenEnabled() const
{
	return isRegenEnabled_;
}

double & HealthComponent::getHealthRef()
{
	return health_;
}

double & HealthComponent::getMaxHealthRef()
{
	return maxHealth_;
}

double & HealthComponent::getHealthRegenRef()
{
	return healthRegen_;
}

void HealthComponent::damage(double amount)
{
	health_ -= amount;
}

void HealthComponent::heal(double amount)
{
	health_ += amount;
	if (health_ > maxHealth_)
	{
		health_ = maxHealth_;
	}
}

void HealthComponent::forceHealth(double amount)
{
	health_ = amount;
	if (health_ > maxHealth_) maxHealth_ = amount;
}

void HealthComponent::update()
{
	if (regenCoolDown.Update() == true)
	{
		if (health_ == maxHealth_) return;
		health_ += healthRegen_;
		if (health_ > maxHealth_)
		{
			health_ = maxHealth_;
		}
		regenCoolDown.StartNewTimer(2);
	}
}
