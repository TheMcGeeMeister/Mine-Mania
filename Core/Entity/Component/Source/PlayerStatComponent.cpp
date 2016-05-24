#include <sstream>
#include <fstream>
#include "PlayerStatComponent.h"
#include "SoundManager.h"

namespace game
{
	extern SoundManager m_sounds;
}

#define EndLine "\n"

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

int PlayerStatComponent::getStrength()
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

void PlayerStatComponent::serialize(std::stringstream & file)
{
	file << level_ << EndLine
		<< maxLevel_ << EndLine
		<< exp_ << EndLine
		<< expNeed_ << EndLine
		<< strength_ << EndLine
		<< stamina_ << EndLine
		<< maxStamina_ << EndLine
		<< magicka_ << EndLine
		<< maxMagicka_ << EndLine
		<< speed_ << EndLine;
}

void PlayerStatComponent::serialize(std::fstream & file)
{
	file << level_ << EndLine // int
		<< maxLevel_ << EndLine // int
		<< exp_ << EndLine // double
		<< expNeed_ << EndLine // double
		<< strength_ << EndLine // int
		<< stamina_ << EndLine // int
		<< maxStamina_ << EndLine // int 
		<< magicka_ << EndLine // int 
		<< maxMagicka_ << EndLine // int 
		<< speed_ << EndLine; // double
}

void PlayerStatComponent::deserialize(std::stringstream & file)
{
	file >> level_
		>> maxLevel_
		>> exp_
		>> expNeed_
		>> strength_
		>> stamina_
		>> maxStamina_
		>> magicka_
		>> maxMagicka_
		>> speed_;
}

void PlayerStatComponent::deserialize(std::fstream & file)
{
	file >> level_
		>> maxLevel_
		>> exp_
		>> expNeed_
		>> strength_
		>> stamina_
		>> maxStamina_
		>> magicka_
		>> maxMagicka_
		>> speed_;
}

bool PlayerStatComponent::addExp(double exp)
{
	exp_ += exp;
	if (exp_ >= expNeed_)
	{
		if (maxLevel_ == 0)
		{
			level_++;
			strength_ += 1;
			maxMagicka_ += 10;
			maxStamina_ += 10;
			speed_ -= 0.01;
			exp_ = exp_ - expNeed_;
			expNeed_ += expNeed_ * 0.3;
			game::m_sounds.PlaySoundR("LevelUp");
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
				strength_ += 1;
				maxMagicka_ += 10;
				maxStamina_ += 10;
				speed_ -= 0.01;
				exp_ = exp_ - expNeed_;
				expNeed_ += expNeed_ * 0.3;
				game::m_sounds.PlaySoundR("LevelUp");
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
		strength_ += 1;
		maxMagicka_ += 10;
		maxStamina_ += 10;
		speed_ -= 0.01;
		exp_ = exp_ - expNeed_;
		expNeed_ += expNeed_ * 0.3;
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
