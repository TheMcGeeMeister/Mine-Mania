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

Player::Player() : UI(0, 0, 50, 30)
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

string Player::getName()
{
    return name_;
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
	return;
}

void Player::mineUp(Display& game)
{
	Position newPos = handPos;
	newPos.getRefY()--;
	if (game.isValidPosition(newPos) == false)
		return;
	if (game.getTileRefAt(newPos).isWall() == true)
		game.getTileRefAt(newPos).mine(10, *this);
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
		game.getTileRefAt(newPos).mine(10, *this);
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
		game.getTileRefAt(newPos).mine(10, *this);
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
		game.getTileRefAt(newPos).mine(10, *this);
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
	turret->setGraphic('*');
	turret->setRange(5);
	game::system.addEntity(turret, "Turret");
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
}

void Player::deserialize(fstream& file)
{
	int pos_x;
	int pos_y;
	file >> goldAmount_;
	file >> maxGoldAmount_;
	file >> manaAmount_;
	file >> maxManaAmount_;
	file >> name_;
	file >> pos_x;
	file >> pos_y;
	handPos.setX(pos_x);
	handPos.setY(pos_y);
}

void Player::deserialize(ifstream& file)
{
	file >> goldAmount_;
	file >> maxGoldAmount_;
	file >> manaAmount_;
	file >> maxManaAmount_;
	file >> name_;
}

void Player::deserialize(stringstream& file)
{
	int pos_x;
	int pos_y;
	file >> goldAmount_;
	file >> maxGoldAmount_;
	file >> manaAmount_;
	file >> maxManaAmount_;
	file >> name_;
	file >> pos_x;
	file >> pos_y;
	handPos.setX(pos_x);
	handPos.setY(pos_y);
}
void Player::update()
{
	health.update();
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
}

void Player::clean()
{
	game::game.removeSelectedAtTile(handPos);
}

////////////////////////////////////////////////

