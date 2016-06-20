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
	extern TileChangeManager TileHandler;
}

extern void updateTile(Position pos);

Tile::Tile()
{
	graphic_ = TG_StoneFloor;
	overlayGraphic_ = ' ';
	overlayColor_ = 0;
	color_ = TGC_StoneFloor;
	claimColor_ = B_Black;
	background_ = TGB_StoneFloor;
	gold_ = 0;
	claimedPercentage_ = 0;
	fortifyAmount_ = 0;
	fortifiedByPlayerID_ = 0;
	objectid_ = 0;
	isClaimable_ = true;
	isOverlayEnabled_ = false;
	isOverlayColorSet_ = false;
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

Tile::Tile(Position pos)
{
	graphic_ = TG_StoneFloor;
	overlayGraphic_ = ' ';
	overlayColor_ = 0;
	color_ = TGC_StoneFloor;
	claimColor_ = B_Black;
	background_ = TGB_StoneFloor;
	gold_ = 0;
	claimedPercentage_ = 0;
	fortifyAmount_ = 0;
	fortifiedByPlayerID_ = 0;
	objectid_ = 0;
	isClaimable_ = true;
	isOverlayEnabled_ = false;
	isOverlayColorSet_ = false;
	claimedBy_ = "Neutral";
	curBeingClaimedBy_ = "None";
	isWalkable_ = true;
	isFortified_ = false;
	isDestructable_ = false;
	isWall_ = false;
	isClaimed_ = false;
	hasGold_ = false;
	pos_(pos.getX(), pos.getY());
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

WORD Tile::getAttribute()
{
    WORD attribute;
	if (isOverlayColorSet_ == false)
	{
		overlayColor_ = color_;
	}
	if (isOverlayEnabled_)
	{
		if (isClaimed_ == false)
		{
			if (background_ == 0)
			{
				attribute = overlayColor_;
			}
			else
			{
				attribute = overlayColor_ | background_;
			}
		}
		else
		{
			attribute = overlayColor_ | claimColor_;
		}
	}
	else
	{
		if (isClaimed_ == false)
		{
			if (background_ == 0)
			{
				attribute = color_;
			}
			else
			{
				attribute = color_ | background_;
			}
		}
		else
		{
			attribute = color_ | claimColor_;
		}
	}
	return attribute;
}

WORD Tile::getOverlayColor() const
{
	return overlayColor_;
}

WORD Tile::getClaimColor() const
{
	return claimColor_;
}

Position Tile::getPos() const
{
	return pos_;
}

char Tile::getGraphic() const
{
	if (isOverlayEnabled_ == true)
		return overlayGraphic_;
	else
		return graphic_;
}

NoteUI Tile::getNote()
{
	NoteUI note;
	note.AddLine("Tile");
	std::stringstream line;
	line << "Health: " << health.getHealth() << "/" << health.getMaxHealth();
	note.AddLine(line.str());
	line.str(string());
	if (isClaimed_)
	{
		line << "Owner:" << claimedBy_;
		note.AddLine(line.str());
		line.str(string());
	}
	return note;
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

bool Tile::isHealthFull() const
{
	return health.getHealth() == health.getMaxHealth();
}

bool Tile::hasGold() const
{
	return hasGold_;
}

bool Tile::hasOverlay() const
{
	return isOverlayEnabled_;
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

int Tile::getFortifiedByID() const
{
	return fortifiedByPlayerID_;
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

void Tile::setOverlayColor(WORD color)
{
	overlayColor_ = color;
	isOverlayColorSet_ = true;
}

void Tile::setClaimColor(WORD color)
{
	claimColor_ = color;
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
	isOverlayEnabled_ = is;
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

void Tile::setFortifiedByID(int id)
{
	fortifiedByPlayerID_ = id;
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

void Tile::claim(int amount, string claimer, WORD color)
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
				if (claimedPercentage_ >= 100)
				{
					claimedBy_ = claimer;
					claimedPercentage_ = 0;
					isClaimed_ = true;
					curBeingClaimedBy_ = "None";
					background_ = color;
					claimColor_ = color;
				}
			}
			else
			{
				claimedPercentage_ -= amount;
				if (claimedPercentage_ <= 0)
				{
					claimedPercentage_ = 0;
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
					background_ = B_Black;
					claimColor_ = B_Black;
				}
		}
	}
	updateServer();
}

void Tile::forceClaim(string claim)
{
	claimedBy_ = claim;
	isClaimable_ = true;
	isClaimed_ = true;
	claimedPercentage_ = 0;
	curBeingClaimedBy_ = "None";
}

bool Tile::fortify(int amount)
{
	if (isFortified_) return false;
	fortifyAmount_ += amount;
	if (fortifyAmount_  >= 25)
	{
		health.getMaxHealthRef() += 150;
		health.getHealthRef() += 150;
		WORD color = C_White;
		Position cPos = pos_;
		isFortified_ = true;
		if (cPos.go(DIRECTION_UP))
		{
			Tile* tile;
			Common::GetTileAt(cPos, &tile);
			if (tile->isFortified() == false)
			{
				color |= S_Top;
			}
			else
			{
				tile->updateFortify();
			}
			cPos = pos_;
		}
		else
		{
			color |= S_Top;
			cPos = pos_;
		}
		if (cPos.go(DIRECTION_DOWN))
		{
			Tile* tile;
			Common::GetTileAt(cPos, &tile);
			if (tile->isFortified() == false)
			{
				color |= S_Bottom;
			}
			else
			{
				tile->updateFortify();
			}
			cPos = pos_;
		}
		else
		{
			color |= S_Bottom;
			cPos = pos_;
		}
		if (cPos.go(DIRECTION_LEFT))
		{
			Tile* tile;
			Common::GetTileAt(cPos, &tile);
			if (tile->isFortified() == false)
			{
				color |= S_Left;
			}
			else
			{
				tile->updateFortify();
			}
			cPos = pos_;
		}
		else
		{
			color |= S_Left;
			cPos = pos_;
		}
		if (cPos.go(DIRECTION_RIGHT))
		{
			Tile* tile;
			Common::GetTileAt(cPos, &tile);
			if (tile->isFortified() == false)
			{
				color |= S_Right;
			}
			else
			{
				tile->updateFortify();
			}
			cPos = pos_;
		}
		else
		{
			color |= S_Right;
			cPos = pos_;
		}
		color_ = color;
		game::TileHandler.push_back(pos_);
		updateServer();
		return true;
	}
	return false;
}

void Tile::hasGold(bool hasGold)
{
	hasGold_ = hasGold;
}

void Tile::heal(int amount)
{
	health.heal(amount);
}

bool Tile::mine(int damage, Player& player)
{
	if (isWall_ == false)
		return false;

	if (isDestructable_ == false)
			return false;

	health.getHealthRef() -= damage;
	health.damage(damage);

	if (isFortified_ && fortifiedByPlayerID_ != player.getID())
	{
		player.damage(5);
	}

	if (health.isDead())
	{
		health.isDead(false);
		health.setMaxHealth(100);
		health.setHealth(100);
		isWall_ = false;
		graphic_ = TG_StoneFloor;
		color_ = TGC_StoneFloor;
		if (isClaimed_ == false)
		{
			setBackground(TGB_StoneFloor);
		}
		else
		{
			background_ = claimColor_;
		}
		isWalkable_ = true;
		isDestructable_ = false;
		isClaimable_ = true;

		if (hasGold_)
		{
			player.addGold(gold_);
			hasGold_ = false;
			gold_ = 0;
			game::m_sounds.PlaySoundR("MetalBreak");
		}
		else
		{
			game::m_sounds.PlaySoundR("Break");
		}
		/* Update Wall Fortifcations Around this block */
		////////////////////
		if (isFortified())
		{
			isFortified_ = false;
			Position cPos = pos_;
			if (cPos.go(DIRECTION_UP))
			{
				Tile* tile;
				Common::GetTileAt(cPos, &tile);
				tile->updateFortify();
			}
			cPos = pos_;
			if (cPos.go(DIRECTION_DOWN))
			{
				Tile* tile;
				Common::GetTileAt(cPos, &tile);
				tile->updateFortify();
			}
			cPos = pos_;
			if (cPos.go(DIRECTION_LEFT))
			{
				Tile* tile;
				Common::GetTileAt(cPos, &tile);
				tile->updateFortify();
			}
			cPos = pos_;
			if (cPos.go(DIRECTION_RIGHT))
			{
				Tile* tile;
				Common::GetTileAt(cPos, &tile);
				tile->updateFortify();
			}
		}
		////////////////////
		updateTile(pos_);
		game::TileHandler.push_back(pos_);
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
	overlayColor_ = 0;
	isOverlayEnabled_ = false;
	isOverlayColorSet_ = false;
	game::TileHandler.push_back(pos_);
}

void Tile::updateOverlay(bool enabled, char graphic)
{
	overlayGraphic_ = graphic;
	isOverlayEnabled_ = enabled;
	game::TileHandler.push_back(pos_);
}

void Tile::updateOverlayS(bool enabled, char graphic)
{
	overlayGraphic_ = graphic;
	isOverlayEnabled_ = enabled;
	game::TileHandler.push_back(pos_);
}

void Tile::updateServer()
{
	stringstream msg;
	msg << SendDefault << EndLine << UpdateTile << EndLine;
	serialize(msg);
	SendServerLiteral(msg.str());
}

void Tile::updateFortify()
{
	if (isFortified_ == false) return;

	WORD color = C_White;
	Position cPos = pos_;
	if (cPos.go(DIRECTION_UP))
	{
		Tile* tile;
		Common::GetTileAt(cPos, &tile);
		if (tile->isFortified() == false)
		{
			color |= S_Top;
		}
		cPos = pos_;
	}
	else
	{
		color |= S_Top;
		cPos = pos_;
	}
	if (cPos.go(DIRECTION_DOWN))
	{
		Tile* tile;
		Common::GetTileAt(cPos, &tile);
		if (tile->isFortified() == false)
		{
			color |= S_Bottom;
		}
		cPos = pos_;
	}
	else
	{
		color |= S_Bottom;
		cPos = pos_;
	}
	if (cPos.go(DIRECTION_LEFT))
	{
		Tile* tile;
		Common::GetTileAt(cPos, &tile);
		if (tile->isFortified() == false)
		{
			color |= S_Left;
		}
		cPos = pos_;
	}
	else
	{
		color |= S_Left;
		cPos = pos_;
	}
	if (cPos.go(DIRECTION_RIGHT))
	{
		Tile* tile;
		Common::GetTileAt(cPos, &tile);
		if (tile->isFortified() == false)
		{
			color |= S_Right;
		}
		cPos = pos_;
	}
	else
	{
		color |= S_Right;
		cPos = pos_;
	}
	color_ = color;
	game::TileHandler.push_back(pos_);
	updateServer();
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
	 << claimedBy_ << endl
	 << curBeingClaimedBy_ << endl
	 << fortifiedByPlayerID_ << endl
	 << color_ << endl
	 << claimColor_ << endl
	 << background_ << endl
	 << isClaimable_ << endl
	 << isWalkable_ << endl
	 << isDestructable_ << endl
	 << isWall_ << endl
	 << isFortified_ << endl
	 << isClaimed_ << EndLine
	 << hasGold_ << endl
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
		<< claimedBy_ << endl
		<< curBeingClaimedBy_ << endl
		<< fortifiedByPlayerID_ << endl
		<< color_ << endl
		<< claimColor_ << endl
		<< background_ << endl
		<< isClaimable_ << endl
		<< isWalkable_ << endl
		<< isDestructable_ << endl
		<< isWall_ << endl
		<< isFortified_ << endl
		<< isClaimed_ << EndLine
		<< hasGold_ << endl
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
		<< claimedBy_ << endl
		<< curBeingClaimedBy_ << endl
		<< fortifiedByPlayerID_ << endl
		<< color_ << endl
		<< claimColor_ << endl
		<< background_ << endl
		<< isClaimable_ << endl
		<< isWalkable_ << endl
		<< isDestructable_ << endl
		<< isWall_ << endl
		<< isFortified_ << endl
		<< isClaimed_ << EndLine
		<< hasGold_ << endl
		<< pos_x << endl
		<< pos_y << endl;
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
		>> claimedBy_
		>> curBeingClaimedBy_
		>> fortifiedByPlayerID_
		>> color_
		>> claimColor_
		>> background_
		>> isClaimable_
		>> isWalkable_
		>> isDestructable_
		>> isWall_
		>> isFortified_
		>> isClaimed_
		>> hasGold_
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
		>> claimedBy_
		>> curBeingClaimedBy_
		>> fortifiedByPlayerID_
		>> color_
		>> claimColor_
		>> background_
		>> isClaimable_
		>> isWalkable_
		>> isDestructable_
		>> isWall_
		>> isFortified_
		>> isClaimed_
		>> hasGold_
		>> pos_x
		>> pos_y;
	pos_.setX(pos_x);
	pos_.setY(pos_y);
	graphic_ = graphic;
	overlayGraphic_ = overlayGraphic;
}

void Tile::deserialize(std::stringstream& stream)
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
		>> claimedBy_
		>> curBeingClaimedBy_
		>> fortifiedByPlayerID_
		>> color_
		>> claimColor_
		>> background_
		>> isClaimable_
		>> isWalkable_
		>> isDestructable_
		>> isWall_
		>> isFortified_
		>> isClaimed_
		>> hasGold_
		>> pos_x
		>> pos_y;
	pos_.setX(pos_x);
	pos_.setY(pos_y);
	graphic_ = graphic;
	overlayGraphic_ = overlayGraphic;
}