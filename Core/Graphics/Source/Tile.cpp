#include "LoadEnums.h"
#include "Tile.h"
#include "game.h"
#include "Player.h"
#include "TileChangeManager.h"
#include "TileEnums.h"
#include "Position.h"
#include "SimpleNetClient.h"
#include "SoundManager.h"
#include "Common.h"

namespace game
{
	extern SimpleNetClient server;
	extern SoundManager m_sounds;
}

extern void updateTile(Position pos);

Tile::Tile()
{
	graphic_ = TG_StoneFloor;
	overlayGraphic_ = ' ';
	color_ = TGC_StoneFloor;
	gold_ = 0;
	claimedPercentage_ = 0;
	objectid_ = 0;
	isClaimable_ = true;
	overlayEnabled_ = false;
	claimedBy_ = "Neutral";
	curBeingClaimedBy_ = "None";
	isWalkable_ = true;
	isFortified_ = false;
	isDestructable_ = false;
	isWall_ = false;
	isClaimed_ = false;
	hasGold_ = false;
	pos_.setX(0);
	pos_.setY(0);
}

Tile::~Tile()
{
	//dtor
}

WORD Tile::getColor() const
{
	return color_;
}

WORD Tile::getBackground() const
{
	return background_;
}

WORD Tile::getAttribute() const
{
	return color_ | background_;
}

Position Tile::getPos() const
{
	return pos_;
}

char Tile::getGraphic() const
{
	if (overlayEnabled_ == true)
		return overlayGraphic_;
	else
		return graphic_;
}

bool Tile::isWall() const
{
	return isWall_;
}

bool Tile::isFortified() const
{
	return isFortified_;
}

bool Tile::isClaimed() const
{
	return isClaimed_;
}

bool Tile::isClaimedBy(string name) const
{
	if (claimedBy_ == name)
		return true;
	else
		return false;
}

bool Tile::isDestructable() const
{
	return isDestructable_;
}

bool Tile::isWalkable() const
{
	return isWalkable_;
}

bool Tile::hasGold() const
{
	return hasGold_;
}

bool Tile::hasOverlay() const
{
	return overlayEnabled_;
}

double Tile::getHealth() const
{
	return health.getHealth();
}

double Tile::getMaxHealth() const
{
	return health.getMaxHealth();
}

int Tile::getClaimedPercentage() const
{
	return claimedPercentage_;
}

int Tile::getGold() const
{
	return gold_;
}

int Tile::getObjectId() const
{
	return objectid_;
}

char Tile::getOverlayGraphic() const
{
	return overlayGraphic_;
}

string Tile::getClaimedBy() const
{
	return claimedBy_;
}

string Tile::getCurBeingClaimedBy() const
{
	return curBeingClaimedBy_;
}

void Tile::setHealth(double health)
{
	this->health.forceHealth(health);
}

void Tile::setMaxHealth(double maxHealth)
{
	health.setMaxHealth(maxHealth);
}

void Tile::setColor(WORD color)
{
	if (color_ == color) return;
	color_ = color;
	game::TileHandler.push_back(pos_);
}

void Tile::setIsWall(bool wall)
{
	isWall_ = wall;
}

void Tile::setIsClaimed(bool isClaimed)
{
	isClaimed_ = isClaimed;
	if (isClaimed == false)
	{
		claimedBy_ = "Neutral";
	}
}

void Tile::setIsClaimed(bool isClaimed, string claimedBy)
{
	isClaimed_ = isClaimed;
	claimedBy_ = claimedBy;
}

void Tile::setIsDestructable(bool isDestructable)
{
	isDestructable_ = isDestructable;
}

void Tile::setIsWalkable(bool isWalkable)
{
	isWalkable_ = isWalkable;
}

void Tile::setIsClaimable(bool canClaim)
{
	isClaimable_ = canClaim;
}

void Tile::setClaimedBy(string claimedBy)
{
	claimedBy_ = claimedBy;
}

void Tile::setObjectId(int id)
{
	objectid_ = id;
}

void Tile::setOverlayEnabled(bool is) // Deprecated Use updateOverlay(bool, char)
{
	overlayEnabled_ = is;
	game::TileHandler.push_back(pos_);
}

void Tile::setOverlayGraphic(char g) // Deprecated Use updateOverlay(bool, char)
{
	overlayGraphic_ = g;
	game::TileHandler.push_back(pos_);
}

void Tile::isWall(bool isWall)
{
	isWall_ = isWall;
}

void Tile::isWalkable(bool isWalkable)
{
	isWalkable_ = isWalkable;
}

void Tile::isFortified(bool isFortified)
{
	if (isFortified == true && isFortified_ == false)
	{
		health.getHealthRef() += 50;
		health.getMaxHealthRef() += 50;
		color_ = S_Right;
		game::TileHandler.push_back(pos_);
	}
	else if (isFortified_ == true && isFortified == false)
	{
		health.getHealthRef() -= 50;
		health.getMaxHealthRef() -= 50;
	}
	isFortified_ = isFortified;
}

void Tile::isDestructable(bool isDestructable)
{
	isDestructable_ = isDestructable;
}

void Tile::isClaimed(bool isClaimed)
{
	isClaimed_ = isClaimed;
	if (isClaimed == false)
	{
		claimedBy_ = "Neutral";
	}
}

void Tile::isClaimed(bool isClaimed, string claimedBy)
{
	isClaimed_ = isClaimed;
	claimedBy_ = claimedBy;
}

void Tile::isClaimable(bool isClaimable)
{
	isClaimable_ = isClaimable;
}

void Tile::setGraphic(char graphic)
{
	if (graphic_ == graphic) return;
	graphic_ = graphic;
	game::TileHandler.push_back(pos_);
}

void Tile::setGoldAmount(int amount)
{
	gold_ = amount;
	if (amount > 0)
		hasGold_ = true;
	else
		hasGold_ = false;
}

void Tile::setPos(Position pos)
{
	pos_ = pos;
}

void Tile::setBackground(WORD background)
{
	if (background_ == background) return;
	background_ = background;
	game::TileHandler.push_back(pos_);
}

void Tile::decrementHealth(int amount)
{
	if (isDestructable_ == false)
		if (amount > 0)
			return;

	health.damage(amount);

	if (health.getHealthRef() <= 0)
	{
		if (isWall_)
		{
			isWall_ = false;
			setGraphic(TG_StoneFloor);
			setColor(TGC_StoneFloor);
			setBackground(TGB_StoneFloor);
			isWalkable_ = true;
			isDestructable_ = true;
			isClaimable(true);
		}
		health.getHealthRef() = health.getMaxHealthRef();
	}
	else if (health.getHealthRef() > health.getMaxHealthRef())
	{
		health.getHealthRef() = health.getMaxHealthRef();
	}
	game::server.UpdateTile(pos_.getX(), pos_.getY());
}

void Tile::incrementHealth(int amount)
{
	if (isDestructable_ == false)
		if (amount < 0)
			return;

	health.heal(amount);

	if (health.getHealthRef() <= 0)
	{
		if (isWall_)
		{
			isWall_ = false;
			graphic_ = TG_StoneFloor;
			color_ = TGC_StoneFloor;
			background_ = TGB_StoneFloor;
			isWalkable_ = true;
			isDestructable_ = true;
			isClaimable_ = true;
		}
		health.getHealthRef() = health.getMaxHealthRef();
	}
	else if (health.getHealthRef() > health.getMaxHealthRef())
	{
		health.getHealthRef() = health.getMaxHealthRef();
	}
	game::server.UpdateTile(pos_.getX(), pos_.getY());
}

void Tile::claim(int amount, string claimer)
{
	if (isClaimable_)
	{
		if (claimer == claimedBy_)
			return;
		if (isClaimed_ == false)
		{
			if (curBeingClaimedBy_ == claimer)
			{
				claimedPercentage_ += amount;
				if (claimedPercentage_ > 100)
				{
					claimedBy_ = claimer;
					claimedPercentage_ = 0;
					isClaimed_ = true;
					curBeingClaimedBy_ = "None";
				}
			}
			else
			{
				int newClaim;
				newClaim = claimedPercentage_ - amount;
				if (newClaim <= 0)
				{
					claimedPercentage_ += amount + newClaim;
					curBeingClaimedBy_ = claimer;
				}
			}
		}
		else
		{
			claimedPercentage_ += amount;
			if (claimedPercentage_ >= 100)
			{
				claimedBy_ = "Neutral";
				claimedPercentage_ = 0;
				isClaimed_ = false;
				curBeingClaimedBy_ = claimer;
			}
		}
	}
}

void Tile::forceClaim(string claim)
{
	claimedBy_ = claim;
	isClaimable_ = true;
	claimedPercentage_ = 0;
	curBeingClaimedBy_ = "None";
}

void Tile::hasGold(bool hasGold)
{
	hasGold_ = hasGold;
}

bool Tile::mine(int damage, Player& underlord)
{
	if (isWall_ == false)
		return false;

	if (isDestructable_ == false)
		if (damage > 0)
			return false;

	health.getHealthRef() -= damage;

	if (health.getHealthRef() <= 0)
	{
		isWall_ = false;
		setGraphic(TG_StoneFloor);
		setColor(TGC_StoneFloor);
		setBackground(TGB_StoneFloor);
		isWalkable_ = true;
		isDestructable_ = false;
		isClaimable_ = true;;
		health.getMaxHealthRef() = 100;

		if (hasGold_)
		{
			underlord.addGold(gold_);
			hasGold_ = false;
			gold_ = 0;
			game::m_sounds.ResetSound("MetalBreak");
			game::m_sounds.PlaySound("MetalBreak");
		}
		else
		{
			game::m_sounds.ResetSound("Break");
			game::m_sounds.PlaySound("Break");
		}

		game::m_sounds.StopSound("Mining");

		health.getHealthRef() = health.getMaxHealthRef();
		updateTile(pos_);
		return true;
	}
	else if (health.getHealthRef() > health.getMaxHealthRef())
	{
		health.getHealthRef() = health.getMaxHealthRef();
	}
	updateTile(pos_);
	return false;
}

void Tile::removeOverlay()
{
	overlayGraphic_ = ' ';
	overlayEnabled_ = false;
	game::TileHandler.push_back(pos_);
}

void Tile::updateOverlay(bool enabled, char graphic)
{
	overlayGraphic_ = graphic;
	overlayEnabled_ = enabled;
	game::TileHandler.push_back(pos_);
}

void Tile::updateOverlayS(bool enabled, char graphic)
{
	overlayGraphic_ = graphic;
	overlayEnabled_ = enabled;
	game::TileHandler.push_back(pos_);
}

void Tile::updateServer()
{
	stringstream msg;
	msg << SendDefault << EndLine << UpdateTile << EndLine;
	serialize(msg);
	SendServerLiteral(msg.str());
}

void Tile::update()
{
	health.update();
}

void Tile::serialize(fstream& stream)
{
	int pos_x = pos_.getX();
	int pos_y = pos_.getY();
	stream << LOAD::L_Tile << endl
	 << health.getHealth() << endl
	 << health.getMaxHealth() << endl
	 << claimedPercentage_ << endl
	 << gold_ << endl
	 << (int)graphic_ << endl
	 << (int)overlayGraphic_ << endl
	 << claimedBy_ << endl
	 << curBeingClaimedBy_ << endl
	 << color_ << endl
	 << background_ << endl
	 << isClaimable_ << endl
	 << isWalkable_ << endl
	 << isDestructable_ << endl
	 << isWall_ << endl
	 << isFortified_ << endl
	 << isClaimable_ << endl
	 << hasGold_ << endl
	 << overlayEnabled_ << endl
	 << pos_x << endl
	 << pos_y << endl;
}

void Tile::serialize(ofstream& stream)
{
	int pos_x = pos_.getX();
	int pos_y = pos_.getY();
	stream << LOAD::L_Tile << endl
		<< health.getHealth() << endl
		<< health.getMaxHealth() << endl
		<< claimedPercentage_ << endl
		<< gold_ << endl
		<< (int)graphic_ << endl
		<< (int)overlayGraphic_ << endl
		<< claimedBy_ << endl
		<< curBeingClaimedBy_ << endl
		<< color_ << endl
		<< background_ << endl
		<< isClaimable_ << endl
		<< isWalkable_ << endl
		<< isDestructable_ << endl
		<< isWall_ << endl
		<< isFortified_ << endl
		<< isClaimable_ << endl
		<< hasGold_ << endl
		<< overlayEnabled_ << endl
		<< pos_x << endl
		<< pos_y << endl;
}

void Tile::serialize(stringstream& stream)
{
	int pos_x = pos_.getX();
	int pos_y = pos_.getY();
	stream << LOAD::L_Tile << endl
		<< health.getHealth() << endl
		<< health.getMaxHealth() << endl
		<< claimedPercentage_ << endl
		<< gold_ << endl
		<< (int)graphic_ << endl
		<< (int)overlayGraphic_ << endl
		<< claimedBy_ << endl
		<< curBeingClaimedBy_ << endl
		<< color_ << endl
		<< background_ << endl
		<< isClaimable_ << endl
		<< isWalkable_ << endl
		<< isDestructable_ << endl
		<< isWall_ << endl
		<< isFortified_ << endl
		<< isClaimable_ << endl
		<< hasGold_ << endl
		<< overlayEnabled_ << endl
		<< pos_x << endl
		<< pos_y << endl;
}

void Tile::deserialize(stringstream& stream)
{
	int pos_x = 0;
	int pos_y = 0;
	int graphic = 0;
	int overlayGraphic = 0;
	stream >> health.getHealthRef()
	 >> health.getMaxHealthRef()
	 >> claimedPercentage_
	 >> gold_
	 >> graphic
	 >> overlayGraphic
	 >> claimedBy_
	 >> curBeingClaimedBy_
	 >> color_
	 >> background_
	 >> isClaimable_
	 >> isWalkable_
	 >> isDestructable_
	 >> isWall_
	 >> isFortified_
	 >> isClaimable_
	 >> hasGold_
	 >> overlayEnabled_
	 >> pos_x
	 >> pos_y;
	pos_.setX(pos_x);
	pos_.setY(pos_y);
	graphic_ = graphic;
	overlayGraphic_ = overlayGraphic;
}

void Tile::deserialize(fstream& stream)
{
	int pos_x = 0;
	int pos_y = 0;
	int graphic = 0;
	int overlayGraphic = 0;
	stream >> health.getHealthRef()
		>> health.getMaxHealthRef()
		>> claimedPercentage_
		>> gold_
		>> graphic
		>> overlayGraphic
		>> claimedBy_
		>> curBeingClaimedBy_
		>> color_
		>> background_
		>> isClaimable_
		>> isWalkable_
		>> isDestructable_
		>> isWall_
		>> isFortified_
		>> isClaimable_
		>> hasGold_
		>> overlayEnabled_
		>> pos_x
		>> pos_y;
	pos_.setX(pos_x);
	pos_.setY(pos_y);
	graphic_ = graphic;
	overlayGraphic_ = overlayGraphic;
}

void Tile::deserialize(ifstream& stream)
{
	int pos_x = 0;
	int pos_y = 0;
	int graphic = 0;
	int overlayGraphic = 0;
	stream >> health.getHealthRef()
		>> health.getMaxHealthRef()
		>> claimedPercentage_
		>> gold_
		>> graphic
		>> overlayGraphic
		>> claimedBy_
		>> curBeingClaimedBy_
		>> color_
		>> background_
		>> isClaimable_
		>> isWalkable_
		>> isDestructable_
		>> isWall_
		>> isFortified_
		>> isClaimable_
		>> hasGold_
		>> overlayEnabled_
		>> pos_x
		>> pos_y;
	pos_.setX(pos_x);
	pos_.setY(pos_y);
	graphic_ = graphic;
	overlayGraphic_ = overlayGraphic;
}