#include <Underlord.h>
#include <fstream>
#include <Position.h>
#include <Display.h>
#include <sstream>

namespace game
{
	extern Display game;
	extern UserInterface SlideUI;
}

Underlord::Underlord() : UI(0, 0, 50, 30)
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

Underlord::~Underlord()
{
    //dtor
}


/* Getters */
////////////////////////////////////////////////
int Underlord::getGoldAmount()
{
    return goldAmount_;
}

int Underlord::getManaAmount()
{
    return manaAmount_;
}

int Underlord::getMaxGoldAmount()
{
    return maxGoldAmount_;
}

int Underlord::getMaxManaAmount()
{
    return maxManaAmount_;
}

string Underlord::getName()
{
    return name_;
}
////////////////////////////////////////////////


/* Setters */
////////////////////////////////////////////////
void Underlord::setManaAmount(int amount)
{
    manaAmount_=amount;
}

void Underlord::setMaxManaAmount(int amount)
{
    maxManaAmount_=amount;
    if(manaAmount_>maxManaAmount_)
    {
        manaAmount_=maxManaAmount_;
    }
}

void Underlord::setName(string name)
{
	game::game.claimNameChange(name_, name);
	name_ = name;
}
////////////////////////////////////////////////


/* Hand*/
////////////////////////////////////////////////
void Underlord::moveHandUp(Display& game)
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

void Underlord::moveHandDown(Display& game)
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

void Underlord::moveHandLeft(Display& game)
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

void Underlord::moveHandRight(Display& game)
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

void Underlord::mineUp(Display& game)
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

void Underlord::mineDown(Display& game)
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

void Underlord::mineLeft(Display& game)
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

void Underlord::mineRight(Display& game)
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

void Underlord::forceHandPosition(Position newPos, Display& game)
{
	game.removeSelectedAtTile(handPos);
	handPos = newPos;
	game.setTileAsSelected(newPos);
}

void Underlord::claimOnHand()
{
	if (game::game.isClaimedTileNear(handPos, name_))
	{
		game::game.getTileRefAt(handPos).claim(10, name_);
	}
}

Position Underlord::getHandPosition()
{
	return handPos;
}
////////////////////////////////////////////////



void Underlord::reset()
{
	goldAmount_ = 100;
	maxGoldAmount_ = 10000;
	manaAmount_ = 500;
	maxManaAmount_ = 500;
	handPos(0, 0);
	mineUIPos(0, 0);

	name_ = "None";
}

void Underlord::updateMiningUI()
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
void Underlord::addGold(int amount)
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

void Underlord::removeGold(int amount)
{
    goldAmount_-=amount;
    if(goldAmount_<0)
        goldAmount_=0;
}

void Underlord::setGoldAmount(int amount)
{
	goldAmount_ = amount;
}

void Underlord::setMaxGoldAmount(int amount)
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
void Underlord::serialize(fstream& file)
{
	file << "Underlord" << endl;
	file << goldAmount_ << endl;
	file << maxGoldAmount_ << endl;
	file << manaAmount_ << endl;
	file << maxManaAmount_ << endl;
	file << name_ << endl;
	file << handPos.getX() << endl;
	file << handPos.getY() << endl;
}

void Underlord::serialize(ofstream& file)
{
	file << "Underlord" << endl;
	file << goldAmount_ << endl;
	file << maxGoldAmount_ << endl;
	file << manaAmount_ << endl;
	file << maxManaAmount_ << endl;
	file << name_ << endl;
	file << handPos.getX() << endl;
	file << handPos.getY() << endl;
}

void Underlord::deserialize(fstream& file)
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

void Underlord::deserialize(ifstream& file)
{
	file >> goldAmount_;
	file >> maxGoldAmount_;
	file >> manaAmount_;
	file >> maxManaAmount_;
	file >> name_;
}

void Underlord::deserialize(stringstream& file)
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
////////////////////////////////////////////////

