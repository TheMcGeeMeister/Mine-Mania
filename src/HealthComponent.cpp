#include "..\include\HealthComponent.h"
#include <sstream>


HealthComponent::HealthComponent()
{
	health_ = 100;
	maxHealth_ = 100;
	healthRegen_ = 1;
	isRegenEnabled_ = true;
	isDead_ = false;
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

void HealthComponent::isDead(bool isDead)
{
	isDead_ = isDead;
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

bool HealthComponent::isDead() const
{
	return isDead_;
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
	if (health_ < 1)
	{
		health_ = 0;
		isDead_ = true;
	}
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

void HealthComponent::reset()
{
	health_ = maxHealth_;
	isDead_ = false;
}

void HealthComponent::serialize(std::fstream & file)
{
	file << health_ << std::endl
		<< maxHealth_ << std::endl
		<< healthRegen_ << std::endl
		<< isRegenEnabled_ << std::endl
		<< isDead_ << std::endl;
}

void HealthComponent::serialize(std::stringstream& stream)
{
	stream << health_ << std::endl
		<< maxHealth_ << std::endl
		<< healthRegen_ << std::endl
		<< isRegenEnabled_ << std::endl
		<< isDead_ << std::endl;
}

void HealthComponent::deserialize(std::fstream & file)
{
	file >> health_
		>> maxHealth_
		>> healthRegen_
		>> isRegenEnabled_
		>> isDead_;
}

void HealthComponent::deserialize(std::stringstream & stream)
{
	stream >> health_
		>> maxHealth_
		>> healthRegen_
		>> isRegenEnabled_
		>> isDead_;
}

void HealthComponent::update()
{
	if (regenCoolDown.Update() == true)
	{
		if (isDead_) return;
		if (health_ == maxHealth_) return;
		health_ += healthRegen_;
		if (health_ > maxHealth_)
		{
			health_ = maxHealth_;
		}
		regenCoolDown.StartNewTimer(0.250);
	}
}
