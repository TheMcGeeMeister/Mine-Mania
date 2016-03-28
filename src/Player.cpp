#include <Player.h>
#include <fstream>
#include <Position.h>
#include <Display.h>
#include <sstream>

namespace game
{
	extern Display game;
	extern UserInterface SlideUI;
	extern System system;
}

Player::Player() : UI(25, 5, 50, 30)
{
    goldAmount_ = 100;
	maxGoldAmount_ = 10000;
    manaAmount_ = 500;
    maxManaAmount_ = 500;
	handPos.setX(0);
	handPos.setY(0);
    name_="None";
	moved_ = false;
	mined_ = false;

	UI.push_back("Health:", false);
	UI.push_back("Gold:", false);
	UI.getSectionRef(1).push_backVar(" ");
	UI.getSectionRef(2).push_backVar(" ");
	UI.isHidden(true);
	mineUIPos.setX(0);
	mineUIPos.setY(0);
	spawnPos(0, 0);
}

Player::Player(Player & p)
{
	*this = p;
}

Player::~Player()
{
    //dtor
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

int Player::getHealth()
{
	return health.getHealth();
}

int Player::getMaxHealth()
{
	return health.getMaxHealth();
}

string Player::getName()
{
    return name_;
}

Position Player::getSpawnPos()
{
	return spawnPos;
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

void Player::setName(string name)
{
	game::game.claimNameChange(name_, name);
	name_ = name;
}

void Player::setSpawnPos(Position pos)
{
	spawnPos = pos;
}

void Player::damage(int amount)
{
	health.damage(amount);
	std::stringstream slide;
	slide << " -" << amount << " Health";
	game::SlideUI.addSlide(slide.str());
	if (health.isDead() == true)
	{
		game::SlideUI.addSlide(name_ + " Died");
		health.getHealthRef() = health.getMaxHealth();
		forceHandPosition(spawnPos, game::game);
		health.isDead(false);
	}

	stringstream msg;
	msg << DamagePlayer << End
		<< name_ << End
		<< amount << End;
	game::game.addPacket(msg.str());
}

void Player::damageS(int amount)
{
	health.damage(amount);
	std::stringstream slide;
	slide << " -" << amount << " Health";
	game::SlideUI.addSlide(slide.str());
	if (health.isDead() == true)
	{
		game::SlideUI.addSlide(name_ + " Died");
		health.getHealthRef() = health.getMaxHealth();
		forceHandPosition(spawnPos, game::game);
		health.isDead(false);
	}
}

void Player::heal(int amount)
{
	health.heal(amount);
	std::stringstream healthSlide;
	healthSlide << name_ << " +" << amount << " Health";
	game::SlideUI.addSlide(healthSlide.str());

	stringstream msg;
	msg << HealPlayer << End
		<< name_ << End
		<< amount << End;
	game::game.addPacket(msg.str());
}
void Player::healS(int amount)
{
	health.heal(amount);
	std::stringstream healthSlide;
	healthSlide << name_ << " +" << amount << " Health";
	game::SlideUI.addSlide(healthSlide.str());
}
////////////////////////////////////////////////


/* Hand*/
////////////////////////////////////////////////
void Player::moveHandUp(Display& game)
{
	Position newPos = handPos;
	newPos.getRefY()--;
	if (game.isValidPosition(newPos, true) == false)
		return;
	game.removeSelectedAtTile(handPos);
	game.setTileAsSelected(newPos);
	handPos = newPos;
	moved_ = true;
	mined_ = false;
	std::stringstream msg;
	msg << UpdatePlayerPosition << endl
		<< name_ << endl
		<< newPos.getX() << endl
		<< newPos.getY() << endl;
	game::game.addPacket(msg.str());
	return;
}

void Player::moveHandDown(Display& game)
{
	Position newPos = handPos;
	newPos.getRefY()++;
	if (game.isValidPosition(newPos, true) == false)
		return;
	game.removeSelectedAtTile(handPos);
	game.setTileAsSelected(newPos);
	handPos = newPos;
	moved_ = true;
	mined_ = false;
	std::stringstream msg;
	msg << UpdatePlayerPosition << endl
		<< name_ << endl
		<< newPos.getX() << endl
		<< newPos.getY() << endl;
	game::game.addPacket(msg.str());
	return;
}

void Player::moveHandLeft(Display& game)
{
	Position newPos = handPos;
	newPos.getRefX()--;
	if (game.isValidPosition(newPos, true) == false)
		return;
	game.removeSelectedAtTile(handPos);
	game.setTileAsSelected(newPos);
	handPos = newPos;
	moved_ = true;
	mined_ = false;
	std::stringstream msg;
	msg << UpdatePlayerPosition << endl
		<< name_ << endl
		<< newPos.getX() << endl
		<< newPos.getY() << endl;
	game::game.addPacket(msg.str());
	return;
}

void Player::moveHandRight(Display& game)
{
	Position newPos = handPos;
	newPos.getRefX()++;
	if (game.isValidPosition(newPos, true) == false)
		return;
	game.removeSelectedAtTile(handPos);
	game.setTileAsSelected(newPos);
	handPos = newPos;
	moved_ = true;
	mined_ = false;
	std::stringstream msg;
	msg << UpdatePlayerPosition << endl
		<< name_ << endl
		<< newPos.getX() << endl
		<< newPos.getY() << endl;
	game::game.addPacket(msg.str());
	return;
}

void Player::mineUp(Display& game)
{
	Position newPos = handPos;
	newPos.getRefY()--;
	if (game.isValidPosition(newPos) == false)
		return;
	if (game.getTileRefAt(newPos).isWall() == true)
		game.getTileRefAt(newPos).mine(25, *this);
	mineUIPos = newPos;
	mined_ = true;
	moved_ = false;
}

void Player::mineDown(Display& game)
{
	Position newPos = handPos;
	newPos.getRefY()++;
	if (game.isValidPosition(newPos) == false)
		return;
	if (game.getTileRefAt(newPos).isWall() == true)
		game.getTileRefAt(newPos).mine(25, *this);
	mineUIPos = newPos;
	mined_ = true;
	moved_ = false;
}

void Player::mineLeft(Display& game)
{
	Position newPos = handPos;
	newPos.getRefX()--;
	if (game.isValidPosition(newPos) == false)
		return;
	if (game.getTileRefAt(newPos).isWall() == true)
		game.getTileRefAt(newPos).mine(25, *this);
	mineUIPos = newPos;
	mined_ = true;
	moved_ = false;
}

void Player::mineRight(Display& game)
{
	Position newPos = handPos;
	newPos.getRefX()++;
	if (game.isValidPosition(newPos) == false)
		return;
	if (game.getTileRefAt(newPos).isWall() == true)
		game.getTileRefAt(newPos).mine(25, *this);
	mineUIPos = newPos;
	mined_ = true;
	moved_ = false;
}

void Player::forceHandPosition(Position newPos, Display& game)
{
	game.removeSelectedAtTile(handPos);
	handPos = newPos;
	game.setTileAsSelected(newPos);
}

void Player::claimOnHand()
{
	if (game::game.isClaimedTileNear(handPos, name_))
	{
		game::game.getTileRefAt(handPos).claim(10, name_);
	}
}

Position Player::getHandPosition()
{
	return handPos;
}
////////////////////////////////////////////////



void Player::placeObject(uint16_t objectID)
{

}

void Player::spawnTurret(Position pos)
{
	shared_ptr<Turret> turret = make_shared<Turret>();
	turret->setPosition(pos);
	turret->setGraphic('+');
	turret->setRange(5);
	turret->setOwner(name_);
	game::system.addEntity(turret, "Turret");
}

void Player::purchaseTurret()
{
	if (goldAmount_ >= 1000)
	{
		shared_ptr<Turret> turret = make_shared<Turret>();
		turret->setPosition(handPos);
		turret->setGraphic('+');
		turret->setRange(5);
		turret->setOwner("None");
		game::system.addEntity(turret, "Turret");
		goldAmount_ -= 1000;
	}
}

void Player::reset()
{
	goldAmount_ = 100;
	maxGoldAmount_ = 10000;
	manaAmount_ = 500;
	maxManaAmount_ = 500;
	handPos(0, 0);
	mineUIPos(0, 0);

	name_ = "None";
}

void Player::updateMiningUI()
{
	if (mined_ && moved_==false)
	{
		stringstream health;
		if (UI.isHidden())
		{
			UI.isHidden(false);
		}
		Tile& tile = game::game.getTileRefAt(mineUIPos);

		health << tile.getHealth() << "/" << tile.getMaxHealth();
		UI.getSectionRef(1).setVar(1, health.str());


		if (tile.hasGold())
		{
			stringstream gold;
			gold << tile.getGold();
			UI.getSectionRef(2).isHidden(false);
			UI.getSectionRef(2).setVar(1, gold.str());
		}
		else
			UI.getSectionRef(2).isHidden(true);
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

void Player::operator=(Player & player)
{
	goldAmount_ = player.goldAmount_;
	maxGoldAmount_ = player.maxGoldAmount_;
	manaAmount_ = player.manaAmount_;
	maxManaAmount_ = player.maxManaAmount_;
	health.setHealth(player.health.getHealth());
	health.setHealthRegen(player.health.getHealthRegen());
	health.setMaxHealth(player.health.getMaxHealth());
	health.isRegenEnabled(player.health.isRegenEnabled());
	moved_ = player.moved_;
	mined_ = player.mined_;
	isDead_ = player.isDead_;
	isLocal_ = player.isLocal_;
	handPos = player.handPos;
	mineUIPos = player.mineUIPos;
	spawnPos = player.spawnPos;
	name_ = player.name_;
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
void Player::serialize(fstream& file)
{
	file << "Player" << endl;
	file << goldAmount_ << endl;
	file << maxGoldAmount_ << endl;
	file << manaAmount_ << endl;
	file << maxManaAmount_ << endl;
	file << name_ << endl;
	file << handPos.getX() << endl;
	file << handPos.getY() << endl;
	file << spawnPos.getX() << endl;
	file << spawnPos.getY() << endl;
}

void Player::serialize(ofstream& file)
{
	file << "Player" << endl;
	file << goldAmount_ << endl;
	file << maxGoldAmount_ << endl;
	file << manaAmount_ << endl;
	file << maxManaAmount_ << endl;
	file << name_ << endl;
	file << handPos.getX() << endl;
	file << handPos.getY() << endl;
	file << spawnPos.getX() << endl;
	file << spawnPos.getY() << endl;
}

void Player::serialize(stringstream & file)
{
	file << "Player" << endl;
	file << goldAmount_ << endl;
	file << maxGoldAmount_ << endl;
	file << manaAmount_ << endl;
	file << maxManaAmount_ << endl;
	file << name_ << endl;
	file << handPos.getX() << endl;
	file << handPos.getY() << endl;
	file << spawnPos.getX() << endl;
	file << spawnPos.getY() << endl;
}

void Player::deserialize(fstream& file)
{
	int pos_x;
	int pos_y;
	int spos_x;
	int spos_y;
	file >> goldAmount_;
	file >> maxGoldAmount_;
	file >> manaAmount_;
	file >> maxManaAmount_;
	file >> name_;
	file >> pos_x;
	file >> pos_y;
	file >> spos_x;
	file >> spos_y;
	handPos.setX(pos_x);
	handPos.setY(pos_y);
	spawnPos.setX(spos_x);
	spawnPos.setY(spos_y);
}

void Player::deserialize(ifstream& file)
{
	int pos_x;
	int pos_y;
	int spos_x;
	int spos_y;
	file >> goldAmount_;
	file >> maxGoldAmount_;
	file >> manaAmount_;
	file >> maxManaAmount_;
	file >> name_;
	file >> pos_x;
	file >> pos_y;
	file >> spos_y;
	file >> spos_x;
	handPos.setX(pos_x);
	handPos.setY(pos_y);
	spawnPos.setX(spos_x);
	spawnPos.setY(spos_y);
}

void Player::deserialize(stringstream& file)
{
	file.clear();
	int pos_x;
	int pos_y;
	int spos_x;
	int spos_y;
	file >> goldAmount_;
	file >> maxGoldAmount_;
	file >> manaAmount_;
	file >> maxManaAmount_;
	file >> name_;
	file >> pos_x;
	file >> pos_y;
	file >> spos_y;
	file >> spos_x;
	handPos.setX(pos_x);
	handPos.setY(pos_y);
	spawnPos.setX(spos_x);
	spawnPos.setY(spos_y);
}
void Player::update()
{
	health.update();
	if (isDead_)
	{
		forceHandPosition(spawnPos, game::game);
		isDead_ = false;
		goldAmount_ = 0;
	}
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
	isDead_ = true;
	clean();

	forceHandPosition(spawnPos, game::game);

	stringstream msg;
	msg << KillPlayer << End
		<< name_ << End;
	game::game.addPacket(msg.str());
}

void Player::killS()
{
	isDead_ = true;
	clean();

	forceHandPosition(spawnPos, game::game);
}

void Player::Log(std::string txt)
{
	std::fstream file("Logs//Log.txt", std::ios::app);

	file << txt << std::endl;
}

void Player::clean()
{
	game::game.removeSelectedAtTile(handPos);
}

////////////////////////////////////////////////

