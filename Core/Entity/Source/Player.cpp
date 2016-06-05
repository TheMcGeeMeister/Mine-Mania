#include <fstream>
#include <sstream>
#include "Core.h"
#include "LoadEnums.h"
#include "Player.h"
#include "Position.h"
#include "Display.h"
#include "SoundManager.h"
#include "TileEnums.h"
#include "Common.h"
#include "LoadParser.h"
#include "Message.h"

namespace game
{
	extern Display game;
	extern UserInterface SlideUI;
	extern UserInterface tileUI;
	extern System system;
	extern SoundManager m_sounds;
	extern LoadParser Parser;
}

enum PLAYER_MODE
{
	MODE_MINING, MODE_HAND, MODE_SHOOTING,
};

Player::Player() : UI(23, 5, 50, 30, 1)
{
    goldAmount_ = 100;
	maxGoldAmount_ = 10000;
    manaAmount_ = 500;
    maxManaAmount_ = 500;
	handMode_ = 0;
	ammo_ = 0;
	passiveGoldIncrease_ = 0;
	id_ = 0;
	baseDamage_ = 15.0;

	isGoldPassive_ = false;
	claimedColor_ = B_Blue;
	pos_(0, 0);
	spawnPos_(0, 0);
    name_= "None";
	moved_ = true;
	mined_ = false;
	isMining_ = false;
	isDead_ = false;
	isGoldPassive_ = false;

	turret_sound.SetSound("TurretPlayerHit");
	repair_sound.SetSound("Repair");
	mine_sound.SetSound("Mining");

	UI.push_back("Mining", false, true);
	UI.push_back("Health:", false, true);
	UI.push_back("Gold:", false, true);
	UI.getSectionRef(2).push_backVar(" ");
	UI.getSectionRef(3).push_backVar(" ");
	UI.isHidden(true);

	mineUIPos_.setX(0);
	mineUIPos_.setY(0);
}

Player::Player(Player & p)
{
	*this = p;
}

Player::~Player()
{
    //dtor
}

void Player::stopSounds()
{
	mine_sound.StopSound();
	turret_sound.StopSound();
	repair_sound.StopSound();
}


/* Getters */
////////////////////////////////////////////////
int Player::getGoldAmount()
{
    return goldAmount_;
}

int Player::getManaAmount()
{
    return manaAmount_;
}

int Player::getMaxGoldAmount()
{
    return maxGoldAmount_;
}

int Player::getMaxManaAmount()
{
    return maxManaAmount_;
}

int Player::getAmmoAmount()
{
	return ammo_;
}

int Player::getHealth()
{
	return health.getHealth();
}

int Player::getMaxHealth()
{
	return health.getMaxHealth();
}

int Player::getLevel()
{
	return stats.getLevel();
}

int Player::getMaxLevel()
{
	return stats.getMaxLevel();
}

int Player::getExp()
{
	return stats.getExp();
}

int Player::getMaxExp()
{
	return stats.getMaxExp();
}

int Player::getID()
{
	return id_;
}

string Player::getName()
{
    return name_;
}

Position Player::getSpawnPos()
{
	return spawnPos_;
}

UserInterface & Player::getUIRef()
{
	return UI;
}

HealthComponent & Player::getHealthComponentRef()
{
	return health;
}

PlayerStatComponent& Player::getStatComponentRef()
{
	return stats;
}

WORD Player::getClaimedColor()
{
	return claimedColor_;
}

void Player::setClaimedColor(WORD color)
{
	claimedColor_ = color;
}
////////////////////////////////////////////////


/* Setters */
////////////////////////////////////////////////
void Player::setManaAmount(int amount)
{
    manaAmount_=amount;
}

void Player::setMaxManaAmount(int amount)
{
    maxManaAmount_=amount;
    if(manaAmount_>maxManaAmount_)
    {
        manaAmount_=maxManaAmount_;
    }
}

void Player::setAmmoAmount(int amount)
{
	ammo_ = amount;
}

void Player::setName(string name)
{
	game::game.claimNameChange(name_, name);
	name_ = name;
}

void Player::setSpawnPos(Position pos)
{
	spawnPos_ = pos;
}

void Player::setHandPos(Position pos)
{
	pos_ = pos;
}

void Player::setHandPosNoUpdate(Position pos)
{
	pos_ = pos;
}

void Player::setHealth(int amount)
{
	health.setHealth(amount);
}

void Player::setMaxHealth(int amount)
{
	health.setMaxHealth(amount);
}

void Player::setID(int id)
{
	id_ = id;
}

bool Player::damage(int amount, string name, bool isServer)
{
	if (isServer == false)
	{
		health.damage(amount);


		std::stringstream slide;
		slide << name_ << " -" << amount << " Health";
		game::SlideUI.addSlide(slide.str());

		if (health.isDead() == true)
		{
			game::SlideUI.addSlide(name_ + " Died");
			kill();
			return true;
		}
		else
		{
			stringstream msg;
			msg << SendDefault << EndLine
				<< PlayerUpdate << EndLine
				<< Health << EndLine
				<< id_ << EndLine
				<< health.getHealth() << EndLine;
			SendServerLiteral(msg.str());
		}
	}
	else
	{
		health.damage(amount);

		std::stringstream slide;
		slide << name_ << " -" << amount << " Health";
		game::SlideUI.addSlide(slide.str());

		if (health.isDead())
		{
			killS();
			return true;
		}
	}
	return false;
}

void Player::damageS(int amount)
{
	/* Not Needed */
}

void Player::heal(int amount)
{
	/* Not Needed */
}

void Player::healS(int amount)
{
	/* Not Needed */
}
////////////////////////////////////////////////


/* Hand*/
////////////////////////////////////////////////
void Player::moveHand(DIRECTION direction)
{
	if (movementTimer_.Update() == false) return;
	Position newPos = pos_; // The position that the player is moving to
	newPos.go(direction);

	/* Collision Checking */
	///////////////////////////////////
	if (game::game.isValidPosition(newPos, true) == false)
		return;
	Entity* entity;
	if (game::system.getEntityAt(newPos, &entity))
	{
		if (entity->hasKeyWord(KEYWORD_BULLET))
		{
			damage(Common::GetBulletDamageFromEntity(entity));
			forceHandPosition(newPos);
			knockbackTo((DIRECTION)Common::GetBulletDirectionFromEntity(entity), 1);
			entity->kill();
			game::m_sounds.PlaySoundR("Bullet");
			std::stringstream msg;
			msg << SendDefault << EndLine << Sound << EndLine << "Bullet" << EndLine;
			SendServerLiteral(msg.str());
		}
		return;
	}
	///////////////////////////////////

	game::game.removeSelectedAtTile(pos_);
	game::game.setTileAsSelected(newPos);
	pos_ = newPos;
	moved_ = true;
	mined_ = false;
	std::stringstream msg;
	msg << SendDefault << EndLine
		<< UpdatePlayerPosition << EndLine
		<< name_ << EndLine
		<< newPos.getX() << EndLine
		<< newPos.getY() << EndLine;
	movementTimer_.StartNewTimer(0.075);
	SendServerLiteral(msg.str());
	return;
}

void Player::moveHandUp(Display& game)
{
	moveHand(DIRECTION_UP);
	return;
}

void Player::moveHandDown(Display& game)
{
	moveHand(DIRECTION_DOWN);
	return;
}

void Player::moveHandLeft(Display& game)
{
	moveHand(DIRECTION_LEFT);
	return;
}

void Player::moveHandRight(Display& game)
{
	moveHand(DIRECTION_RIGHT);
	return;
}

void Player::mineUp(Display& game)
{
	mine(DIRECTION_UP);
}

void Player::mineDown(Display& game)
{
	mine(DIRECTION_DOWN);
}

void Player::mineLeft(Display& game)
{
	mine(DIRECTION_LEFT);
}

void Player::mineRight(Display& game)
{
	mine(DIRECTION_RIGHT);
}

void Player::mine(DIRECTION direction)
{
	if (mineTimer_.Update() == false) return;
	if (handMode_ == MODE_MINING)
	{
		Position newPos = pos_;
		newPos.go(direction);
		if (game::system.entityAt(newPos))
		{
			Entity* entity;
			if (game::system.getEntityAt(newPos, &entity))
			{
				double tDamage = baseDamage_ + (baseDamage_ / (5.0 / stats.getStrength()));
				if (entity->damage((int)tDamage, name_))
				{
					turret_sound.SetTimer(0.200);
				}
			}
		}
		else
		{
			if (game::game.isValidPosition(newPos) == false)
				return;
			if (game::game.getTileRefAt(newPos).isWall() == true)
			{
				double tDamage = baseDamage_ + (baseDamage_ / (5.0 / stats.getStrength()));
				if (game::game.getTileRefAt(newPos).mine((int)tDamage, *this))
				{
					if (stats.addExp(10))
					{
						health.setMaxHealth(health.getMaxHealth() + 10);
						std::stringstream lvlmsg;
						lvlmsg << "You Leveled Up! Level:" << stats.getLevel();
						game::SlideUI.addSlide(lvlmsg.str());

						/////////* Sync *//////////
						Message msg(true);
						msg << PlayerUpdate << Stats << id_;
						stats.serialize(msg.get());
						msg.Send();
						/////////* Sync *//////////
					}
					else
					{
						/////////* Sync *//////////
						Message msg(true);
						msg << PlayerUpdate << Exp << id_ << stats.getExp();
						msg.Send();
						/////////* Sync *//////////
					}
					mine_sound.StopSound();
				}
			}
			else
				return;
			mineUIPos_ = newPos;
			mined_ = true;
			moved_ = false;
			if (game::game.getTileRefAt(newPos).isWall() == true)
			{
				mine_sound.SetTimer(0.200);
			}
			mineTimer_.StartNewTimer(0.075);
		}
	}
	else if(handMode_ == MODE_HAND)
	{
		if (moved_ == true)
		{
			moved_ = false;
			mineProgress_ = 0;
		}
		Position newPos = pos_;
		newPos.go(direction);
		Entity* entity;
		if(game::system.getEntityAt(newPos, &entity) == true)
		{
			entity->activate(this);
		}
		else
		{
			Tile& tile = game::game.getTileRefAt(newPos);
			if (tile.isWall() == false)
			{
				mineProgress_ += 25;
				if (mineProgress_ >= 100)
				{
					tile.setGraphic(TG_Stone);
					tile.isWall(true);
					tile.setColor(TGC_Stone);
					tile.setBackground(TGB_Stone);
					tile.isWalkable(false);
					tile.isDestructable(true);
					game::m_sounds.PlaySoundR("Place");
					tile.updateServer();
					mineProgress_ = 0;
				}
			}
			else
			{
				if (tile.isHealthFull() == false)
				{
					tile.heal(1);
					repair_sound.SetTimer(0.2);
				}
				else
				{
					if (tile.fortify(1))
					{
						tile.setFortifiedByID(id_);
					}
				}
			}
		}

	}
	else if (handMode_ = MODE_SHOOTING)
	{
		if (ammo_ > 0)
		{
			if (shootTimer_.Update() == true)
			{
				if (Common::ShootFrom(pos_, direction, 50))
				{
					shootTimer_.StartNewTimer(0.35);
					game::m_sounds.PlaySoundR("TurretShoot");
					ammo_--;
				}
			}
			
		}
		else
		{
			game::m_sounds.PlaySoundR("NoAmmo");
		}
	}
}

void Player::forceHandPosition(Position newPos, Display& game)
{
	game.removeSelectedAtTile(pos_);
	pos_ = newPos;
	game.setTileAsSelected(newPos);
}

void Player::forceHandPosition(Position newPos)
{
	game::game.removeSelectedAtTile(pos_);
	pos_ = newPos;
	game::game.setTileAsSelected(newPos);

}

void Player::claimOnHand()
{
	if (game::game.isClaimedTileNear(pos_, name_))
	{
		game::game.getTileRefAt(pos_).claim(10, name_, claimedColor_);
	}
}

void Player::switchMode()
{
	if (handMode_ == 0)
	{
		handMode_ = 1;
		game::tileUI.getSectionRef(6).setVar(1, "Hand");
	}
	else if(handMode_ == 1)
	{
		handMode_ = 2;
		game::tileUI.getSectionRef(6).setVar(1, "Weapon");
	}
	else
	{
		handMode_ = 0;
		game::tileUI.getSectionRef(6).setVar(1, "Mining");
	}
	game::m_sounds.PlaySoundR("Swap");
}

void Player::switchModeTo(int mode)
{
	handMode_ = mode;
	switch (mode)
	{
	case 0:game::tileUI.getSectionRef(6).setVar(1, "Mining"); break;
	case 1:game::tileUI.getSectionRef(6).setVar(1, "Hand"); break;
	case 2:game::tileUI.getSectionRef(6).setVar(1, "Weapon"); break;
	}
	game::m_sounds.PlaySoundR("Swap");
}

void Player::updateHandPos()
{
	game::game.setTileAsSelectedS(pos_);
}

void Player::disableMovementFor(int time)
{
	movementTimer_.StartNewTimer(time);
}

void Player::knockbackTo(DIRECTION direction, int amount)
{
	Position pPos = pos_; // Previous position
	Position cPos = pos_;
	std::stringstream msg;
	msg << SendDefault << EndLine << PlayerUpdate << EndLine << Knockback << EndLine << name_ << EndLine << 1.5 << EndLine;
	for (int x = 0; x < amount; x++)
	{
		pPos = cPos;
		cPos.go(direction);
		if (cPos.isValid() == false)
		{
			forceHandPosition(pPos);
			msg << pPos.serializeR();
			SendServerLiteral(msg.str());
			return;
		}
		if (game::game.getTileRefAt(cPos).isWalkable() == false)
		{
			if (x > 0)
			{
				forceHandPosition(pPos);
				msg << pPos.serializeR();
				SendServerLiteral(msg.str());
				return;
			}
			return;
		}
		if (game::system.entityAt(cPos) == true)
		{
			if (x > 0)
			{
				forceHandPosition(pPos);
				msg << pPos.serializeR();
				SendServerLiteral(msg.str());
				return;
			}
			return;
		}
	}
	forceHandPosition(cPos);
	msg << cPos.serializeR();
	SendServerLiteral(msg.str());
	updatePosition();
}

Position Player::getHandPosition()
{
	return pos_;
}
////////////////////////////////////////////////
void Player::purchaseTurret()
{
	if (goldAmount_ >= 1000)
	{
		if (game::system.entityAt(pos_) == false && game::game.getTileRefAt(pos_).isClaimedBy(name_))
		{
			shared_ptr<Turret> turret = make_shared<Turret>();
			turret->setPosition(pos_);
			turret->setGraphic('+');
			turret->setRange(8);
			turret->setOwner(name_);
			game::system.addEntity(turret, "Turret");
			goldAmount_ -= 1000;
			game::m_sounds.PlaySoundR("Money");
		}
	}
}

void Player::purchaseBullet()
{
	if (goldAmount_ >= 100)
	{
		ammo_++;
		goldAmount_ -= 100;
		game::m_sounds.PlaySoundR("Money");
	}
}

void Player::updatePosition()
{
	std::stringstream msg;
	msg << SendDefault << EndLine << UpdatePlayerPosition << EndLine << name_ << EndLine << pos_.serializeR();
	SendServerLiteral(msg.str());
}

void Player::isGoldPassive(bool is)
{
	isGoldPassive_ = is;
}

bool Player::isGoldPassive()
{
	return isGoldPassive_;
}

void Player::addPassiveGold(int amount)
{
	passiveGoldIncrease_ += amount;
	if (passiveGoldIncrease_ > 0)
	{
		isGoldPassive_ = true;
	}
}

void Player::removePassiveGold(int amount)
{
	passiveGoldIncrease_ -= amount;
	if (passiveGoldIncrease_ = 0)
	{
		isGoldPassive_ = false;
	}
}

void Player::reset()
{
	*this = Player();
}

void Player::updateMiningUI()
{
	if (mined_ == true && moved_ == false)
	{
		stringstream health;
		if (UI.isHidden())
		{
			UI.isHidden(false);
		}
		Tile& tile = game::game.getTileRefAt(mineUIPos_);

		health << tile.getHealth() << "/" << tile.getMaxHealth();
		UI.getSectionRef(2).setVar(1, health.str());


		if (tile.hasGold())
		{
			stringstream gold;
			gold << tile.getGold();
			UI.getSectionRef(3).isHidden(false);
			UI.getSectionRef(3).setVar(1, gold.str());
		}
		else
			UI.getSectionRef(3).isHidden(true);
	}
	else
	{
		if (UI.isHidden() == false)
		{
			UI.isHidden(true);
		}
	}
	UI.update();
}



/* Gold Related*/
////////////////////////////////////////////////
void Player::addGold(int amount)
{
    goldAmount_+=amount;
	stringstream goldAdd;
	goldAdd << name_ << ":+" << amount << " Gold";
	game::SlideUI.addSlide(goldAdd.str());
    if(goldAmount_>maxGoldAmount_)
    {
        goldAmount_=maxGoldAmount_;
		game::SlideUI.addSlide("Max Gold");
    }
}

void Player::removeGold(int amount)
{
    goldAmount_-=amount;
    if(goldAmount_<0)
        goldAmount_=0;
}

void Player::setGoldAmount(int amount)
{
	goldAmount_ = amount;
}

void Player::setMaxGoldAmount(int amount)
{
	maxGoldAmount_ = amount;
	if (goldAmount_>maxGoldAmount_)
	{
		goldAmount_ = maxGoldAmount_;
	}
}
////////////////////////////////////////////////

/* Serialize - Deserialize */
////////////////////////////////////////////////
void Player::serialize(fstream& stream)
{
	using namespace game;
	stream << LOAD::L_Player << endl
		<< id_ << endl
		<< goldAmount_ << endl
		<< maxGoldAmount_ << endl
		<< manaAmount_ << endl
		<< maxManaAmount_ << endl
		<< ammo_ << endl
		<< name_ << endl
		<< passiveGoldIncrease_ << endl
		<< isGoldPassive_ << endl
		<< (int)claimedColor_ << endl
		<< pos_.getX() << endl
		<< pos_.getY() << endl
		<< spawnPos_.getX() << endl
		<< spawnPos_.getY() << endl;
	health.serialize(stream);
	stats.serialize(stream);
}

void Player::serialize(stringstream & stream)
{
	stream << LOAD::L_Player << endl
		<< id_ << endl
		<< goldAmount_ << endl
		<< maxGoldAmount_ << endl
		<< manaAmount_ << endl
		<< maxManaAmount_ << endl
		<< ammo_ << endl
		<< name_ << endl
		<< passiveGoldIncrease_ << endl
		<< isGoldPassive_ << endl
		<< (int)claimedColor_ << endl
		<< pos_.getX() << endl
		<< pos_.getY() << endl
		<< spawnPos_.getX() << endl
		<< spawnPos_.getY() << endl;
	health.serialize(stream);
	stats.serialize(stream);
}

void Player::deserialize(fstream& stream)
{
	int pos_x;
	int pos_y;
	int spos_x;
	int spos_y;
	int claimedColor;
	stream >> id_
		>> goldAmount_
		>> maxGoldAmount_
		>> manaAmount_
		>> maxManaAmount_
		>> ammo_
		>> name_
		>> passiveGoldIncrease_
	    >> isGoldPassive_
		>> claimedColor
		>> pos_x
		>> pos_y
		>> spos_x
		>> spos_y;
	pos_.setX(pos_x);
	pos_.setY(pos_y);
	spawnPos_.setX(spos_x);
	spawnPos_.setY(spos_y);
	health.deserialize(stream);
	stats.deserialize(stream);
	claimedColor_ = claimedColor;
}

void Player::deserialize(stringstream& stream)
{
	int pos_x;
	int pos_y;
	int spos_x;
	int spos_y;
	int claimedColor;
	stream >> id_
		>> goldAmount_
		>> maxGoldAmount_
		>> manaAmount_
		>> maxManaAmount_
		>> ammo_
		>> name_
		>> passiveGoldIncrease_
		>> isGoldPassive_
		>> claimedColor
		>> pos_x
		>> pos_y
		>> spos_x
		>> spos_y;
	pos_.setX(pos_x);
	pos_.setY(pos_y);
	spawnPos_.setX(spos_x);
	spawnPos_.setY(spos_y);
	health.deserialize(stream);
	stats.deserialize(stream);
	claimedColor_ = claimedColor;
}

void Player::update()
{
	int pHealth = health.getHealth();
	health.update();
	if (health.getHealth() != pHealth)
	{
		Message msg(true);
		msg << PlayerUpdate
			<< Health
			<< id_
			<< health.getHealth();
		msg.Send();
	}
	if (expCooldown_.Update() == true)
	{
		if (stats.addExp(1) == false)
		{
			Message msg(true);
			msg << PlayerUpdate << Exp << id_ << stats.getExp();
			msg.Send();
		}else
		{
			Message msg(true);
			msg << PlayerUpdate << Stats << id_;
			stats.serialize(msg.get());
			msg.Send();
		}
		expCooldown_.StartNewTimer(1);
	}
	if (isGoldPassive_ == true)
	{
		if (goldCooldown_.Update() == true)
		{
			goldAmount_ += passiveGoldIncrease_;
			goldCooldown_.StartNewTimer(2);
		}
	}
	turret_sound.Update();
	repair_sound.Update();
	mine_sound.Update();
}

bool Player::hasComponent(int id)
{
	switch (id)
	{
	case COMPONENT_HEALTH: return true;
	default: return false;
	}
}

bool Player::isKilled()
{
	return isDead_;
}

void Player::kill()
{
	forceHandPosition(spawnPos_, game::game);
	health.reset();
	stats = PlayerStatComponent();

	stringstream msg;
	msg << SendDefault << EndLine << PlayerUpdate << EndLine << Kill << EndLine << id_ << EndLine;
	SendServerLiteral(msg.str());
}

void Player::killS()
{
	health.reset();
	stats = PlayerStatComponent();
	forceHandPosition(spawnPos_, game::game);
}

void Player::Log(std::string txt)
{
	std::fstream file("Logs//Log.txt", std::ios::app);

	file << txt << std::endl;
}

void Player::clean()
{
	game::game.removeSelectedAtTile(pos_);
}

void Player::setPos(Position pos)
{
	forceHandPosition(pos);
}

void Player::updateOverlay()
{
	return;
}

void Player::updateID()
{
	return;
}

void Player::send()
{
	return;
}

void Player::render()
{
	game::game.setTileAsSelected(pos_);
}

void Player::activate(Player* player)
{
	return;
}

Position Player::getPos()
{
	return pos_;
}

////////////////////////////////////////////////

Player CreatePlayerAt(Position pos, std::string name) // Creates player, but doesn't add to map
{
	Player NewPlayer;
	NewPlayer.setHandPos(pos);
	NewPlayer.setSpawnPos(pos);
	NewPlayer.setName(name);
	return NewPlayer;
}

Player CreatePlayerAndSendAt(Position pos, std::string name, list<int> toSend) // Creates player, sends to other, but doesn't add to map
{
	Player NewPlayer;
	NewPlayer.setHandPos(pos);
	NewPlayer.setSpawnPos(pos);
	NewPlayer.setName(name);
	std::stringstream msg;
	for (auto& iter : toSend)
	{
		msg << iter << EndLine << AddPlayer << EndLine;
		NewPlayer.serialize(msg);
		SendServerLiteral(msg.str());
		msg.str(string());
	}
	return NewPlayer;
}

Player Common::CreatePlayer(Position pos, std::string name, WORD claim_color, int id, bool isLocal)
{
	Player NewPlayer;
	NewPlayer.setHandPosNoUpdate(pos);
	NewPlayer.setSpawnPos(pos);
	NewPlayer.setName(name);
	NewPlayer.setClaimedColor(claim_color);
	NewPlayer.setID(id);
	if (isLocal)
	{
		Common::AddLocalPlayer(&NewPlayer);
	}
	else
	{
		Common::AddPlayer(&NewPlayer);

	}
	return NewPlayer;
}
