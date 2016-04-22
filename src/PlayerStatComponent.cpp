#include "..\include\PlayerStatComponent.h"
#include "..\include\PlayerStatComponent.h"



PlayerStatComponent::PlayerStatComponent()
{
	level_ = 1;
	maxLevel_ = 0;
	strength_ = 1;
	stamina_ = 100;
	maxStamina_ = 100;
	magicka_ = 100;
	maxMagicka_ = 100;
	speed_ = 1.0;
	exp_ = 0;
	expNeed_ = 100;
}


PlayerStatComponent::~PlayerStatComponent()
{

}

int PlayerStatComponent::getLevel()
{
	return level_;
}

int PlayerStatComponent::getMaxLevel()
{
	return maxLevel_;
}

double PlayerStatComponent::getExp()
{
	return exp_;
}

double PlayerStatComponent::getMaxExp()
{
	return expNeed_;
}

int PlayerStatComponent::getStength()
{
	return strength_;
}

int PlayerStatComponent::getStamina()
{
	return stamina_;
}

int PlayerStatComponent::getMaxStamina()
{
	return maxStamina_;
}

int PlayerStatComponent::getMagicka()
{
	return magicka_;
}

int PlayerStatComponent::getMaxMagicka()
{
	return maxMagicka_;
}

double PlayerStatComponent::getSpeedMultiplyer()
{
	return speed_;
}

void PlayerStatComponent::update()
{
	return;
}

bool PlayerStatComponent::addExp(double exp)
{
	exp_ += exp;
	if (exp_ >= expNeed_)
	{
		if (maxLevel_ == 0)
		{
			level_++;
			strength_ += 10;
			maxMagicka_ += 10;
			maxStamina_ += 10;
			speed_ -= 0.01;
			exp_ = exp_ - expNeed_;
			expNeed_ = expNeed_ * 2;
			return true;
		}
		else
		{
			if (level_ == maxLevel_)
			{
				exp_ = expNeed_;
			}
			else
			{
				level_++;
				strength_ += 10;
				maxMagicka_ += 10;
				maxStamina_ += 10;
				speed_ -= 0.01;
				exp_ = exp_ - expNeed_;
				expNeed_ = expNeed_ * 2;
				return true;
			}
		}
	}
	else
	{
		return false;
	}
}

void PlayerStatComponent::setLevel(int level)
{
	level_ = level;
}

void PlayerStatComponent::setLevelWithScale(int level)
{
	for (; level_ <= level; level_++)
	{
		strength_ += 10;
		maxMagicka_ += 10;
		maxStamina_ += 10;
		speed_ -= 0.01;
		exp_ = exp_ - expNeed_;
		expNeed_ = expNeed_ * 2;
	}
}

void PlayerStatComponent::setMaxLevel(int maxLevel)
{
	maxLevel_ = maxLevel;
}

void PlayerStatComponent::setStamina(int stamina)
{
	stamina_ = stamina;
}

void PlayerStatComponent::setMaxStamina(int stamina)
{
	maxStamina_ = stamina;
}

void PlayerStatComponent::setMagicka(int magicka)
{
	magicka_ = magicka;
}

void PlayerStatComponent::setMaxMagicka(int magicka)
{
	maxMagicka_ = magicka;
}

void PlayerStatComponent::setStrength(int strength)
{
	strength_ = strength;
}

void PlayerStatComponent::setSpeed(double speed)
{
	speed_ = speed;
}
