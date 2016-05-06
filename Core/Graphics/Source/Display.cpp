#include <sstream>
#include <fstream>
#include "TileEnums.h"
#include "Player.h"
#include "Common.h"
#include "Core.h"
#include "Packet.h"
#include "PlayerHandler.h"
#include "LoadEnums.h"
#include "Display.h"
#include "Bullet.h"
#include "Turret.h"

namespace game
{
	extern System system;
	extern PlayerHandler pHandler;
}

extern void setFontInfo(int fontSize, int font);

Display::Display()
{
    h=GetStdHandle(STD_OUTPUT_HANDLE);
    size_x_=10;
    size_y_=10;
    nlength=1;
	isLoaded_ = false;
	isBorderUsed_ = false;
	isHidden_ = false;
	isMultiplayer_ = false;
	borderWidth_ = 0;
	offset_x_ = 0;
	offset_y_ = 0;
	int font = 0;
	int fontSize = 28;
	saveSuffix_ = "0";
}

Display::~Display()
{
    //dtor
}

void Display::update()
{
    for(auto &iter : tileChanges_)
    {
        m_map[iter.first]=iter.second;
        /*if(isSeen_[iter.first]==true) no vision yet
        {
            mapSeen_[iter.first]=iter.second;
            updatePos(iter.first);
        }*/
        updatePos(iter.first);
    }
    tileChanges_.clear();

	for (auto &iter : m_map)
	{
		iter.second.update();
	}
    if(reloadAll_)
    {
        for(auto &iter : m_map)
        {
            updatePos(iter.first);
            reloadAll_=false;
        }
    }
}

void Display::updatePos(Position pos_)
{
	if (!m_map.count(pos_))
		return;
    Position CurrentPosition;
    CurrentPosition=pos_;
    const char graphic=m_map[CurrentPosition].getGraphic();
    WORD color=m_map[CurrentPosition].getColor();
    if(m_map[CurrentPosition].getBackground()!=0)
    {
        color=color | m_map[CurrentPosition].getBackground();
    }
	if (isSelected_.count(pos_))
	{
		if (isSelected_[pos_] == true)
		{
			color = S_Box | C_White;
		}
	}
    pos.X=CurrentPosition.getX();
    pos.Y=CurrentPosition.getY();
    //ReadConsoleOutputCharacter(h, &readConsole_, nlength, pos, &output);
    //if (readConsole_ == graphic) return;
    WriteConsoleOutputCharacter(h, &graphic, nlength, pos, &output);
    WriteConsoleOutputAttribute(h, &color, nlength , pos, &output);
}

void Display::reloadAll()
{
    reloadAll_=true;
}

void Display::cleanOverlays()
{
	for (auto& iter : m_map)
	{
		iter.second.updateOverlay(false, ' ');
	}
}

void Display::setTileAt(Position _pos, Tile _tile)
{
    pair<Position, Tile> newTileChange;
    newTileChange.first=_pos;
    newTileChange.second=_tile;
    tileChanges_.push_back(newTileChange);
}
void Display::setTileAtS(Position _pos, Tile _tile)
{
	pair<Position, Tile> newTileChange;
	newTileChange.first = _pos;
	newTileChange.second = _tile;
	tileChanges_.push_back(newTileChange);
}
void Display::setTileAsSelected(Position newPos)
{
	WORD color;
	color = S_Box | C_White;
	setPos(newPos, color);
	isSelected_[newPos] = true;
}

void Display::setTileAsSelectedS(Position newPos)
{
	WORD color;
	color = S_Box | C_White;
	setPos(newPos, color);
	isSelected_[newPos] = true;
}

void Display::setSizeX(int x)
{
    size_x_=x;
}

void Display::setSizeY(int y)
{
    size_y_=y;
}

void Display::setFont(int font)
{
	font_ = font;
}

void Display::setFontSize(int fontSize)
{
	if (fontSize < 5 || fontSize > 72)
	{
		return;
	}
	else
	{
		fontSize_ = fontSize;
	}
}

void Display::setVolume(int volume)
{
	volume_ = volume;
}

void Display::isHidden(bool hidden)
{
	isHidden_ = hidden;
	if (hidden)
	{
		extern void clearScreenCertain(int x, int y, int start_y);
		clearScreenCertain(size_x_, size_y_, offset_y_);
	}
	else
	{
		reloadAll();
	}
}

void Display::isFullScreen(bool is)
{
	isFullscreen_ = is;
}

void Display::setPos(Position ipos, char graphic)
{
    pos.X=ipos.getX();
    pos.Y=ipos.getY();
    //ReadConsoleOutputCharacter(h, &readConsole_, nlength, pos, &output);
    //if(readConsole_ == graphic) return;
    WriteConsoleOutputCharacter(h, &graphic, nlength, pos, &output);
}

void Display::setPos(Position ipos, WORD color)
{
    pos.X=ipos.getX();
    pos.Y=ipos.getY();

    //ReadConsoleOutputAttribute(h, &readAttribute_, nlength, pos, &output);
    //if(readAttribute_ != color)
        WriteConsoleOutputAttribute(h, &color, nlength, pos, &output);
}

void Display::setPos(Position ipos, char graphic, WORD color)
{
    pos.X=ipos.getX();
    pos.Y=ipos.getY();

    //ReadConsoleOutputCharacter(h, &readConsole_, nlength, pos, &output);
    //if(readConsole_ != graphic)
        WriteConsoleOutputCharacter(h, &graphic, nlength, pos, &output);

    //ReadConsoleOutputAttribute(h, &readAttribute_, nlength, pos, &output);
    //if(readAttribute_ != color)
        WriteConsoleOutputAttribute(h, &color, nlength, pos, &output);
}

bool Display::isTileNear(Tile& Seek, Position iPos)
{
    Position pos=iPos;

    pos=getPosUp(iPos);
    if(pos.isValid())
    {
        if(&getTileRefAt(pos)==&Seek)
        {
            return true;
        }
    }
    pos=getPosDown(iPos);
    if(pos.isValid())
    {
        if(&getTileRefAt(pos)==&Seek)
        {
            return true;
        }
    }
    pos=getPosLeft(iPos);
    if(pos.isValid())
    {
        if(&getTileRefAt(pos)==&Seek)
        {
            return true;
        }
    }
    pos=getPosRight(iPos);
    if(pos.isValid())
    {
        if(&getTileRefAt(pos)==&Seek)
        {
            return true;
        }
    }
    return false;
}

bool Display::isWalkableTileNear(Position iPos)
{
    Position pos=iPos;

    pos=getPosUp(iPos);
    if(pos.isValid())
    {
        if(getTileRefAt(pos).isWalkable())
        {
            return true;
        }
    }
    pos=getPosDown(iPos);
    if(pos.isValid())
    {
        if(getTileRefAt(pos).isWalkable())
        {
            return true;
        }
    }
    pos=getPosLeft(iPos);
    if(pos.isValid())
    {
        if(getTileRefAt(pos).isWalkable())
        {
            return true;
        }
    }
    pos=getPosRight(iPos);
    if(pos.isValid())
    {
        if(getTileRefAt(pos).isWalkable())
        {
            return true;
        }
    }
    return false;
}

bool Display::isClaimedTileNear(Position iPos, string underlord)
{
	Position pos = iPos;
	pos = getPosUp(iPos);
	if (pos.isValid())
	{
		if (getTileRefAt(pos).isClaimedBy(underlord))
		{
			return true;
		}
	}
	pos = getPosDown(iPos);
	if (pos.isValid())
	{
		if (getTileRefAt(pos).isClaimedBy(underlord))
		{
			return true;
		}
	}
	pos = getPosLeft(iPos);
	if (pos.isValid())
	{
		if (getTileRefAt(pos).isClaimedBy(underlord))
		{
			return true;
		}
	}
	pos = getPosRight(iPos);
	if (pos.isValid())
	{
		if (getTileRefAt(pos).isClaimedBy(underlord))
		{
			return true;
		}
	}
	return false;
}

bool Display::isLoaded()
{
	return isLoaded_;
}

bool Display::isHidden()
{
	return isHidden_;
}

bool Display::isValidPosition(Position pos, bool isPlayer)
{
	if (pos.getX() <=offset_x_+borderWidth_ - 1)
		return false;
	if (pos.getX() >= offset_x_ + size_x_ + (borderWidth_ * 2))
		return false;
	if (pos.getY() <= offset_y_ + borderWidth_ - 1)
		return false;
	if (pos.getY() >= offset_y_ + size_y_ + (borderWidth_ * 2))
		return false;
	if (isPlayer)
	{
		if (getTileRefAt(pos).isWall() == true)
			return false;
		if (getTileRefAt(pos).isWalkable() == false)
			return false;
		if (game::pHandler.playerAt(pos) == true)
			return false;
	}
	return true;
}

bool Display::isValidPosition(Position pos)
{
	if (pos.getX() <= offset_x_ + borderWidth_ - 1)
		return false;
	if (pos.getX() >= offset_x_ + size_x_ + (borderWidth_ * 2))
		return false;
	if (pos.getY() <= offset_y_ + borderWidth_ - 1)
		return false;
	if (pos.getY() >= offset_y_ + size_y_ + (borderWidth_ * 2))
		return false;

	return true;
}

bool Display::isFullScreen()
{
	return isFullscreen_;
}

bool Display::isLoadedMultiplayer()
{
	return isMultiplayer_;
}

void Display::updateTileServer(Position pos)
{
	std::stringstream msg;
	msg << SendDefault << EndLine << UpdateTile << EndLine << m_map[pos].serialize(true);
	SendServerLiteral(msg.str());
}

Position Display::searchLine(Position sPos, DIRECTION direction, int amount, char target)
{
	Position rPos; // Return Position
	Position cPos; // Current Position

	cPos = sPos;
	
	for (int x = 0; x < amount; x++)
	{
		if (m_map[cPos].getGraphic() == target)
		{
			rPos = cPos;
			return rPos;
		}
		else
		{
			cPos.go(direction);
		}
	}
	cPos.isValid(false);
	return cPos;
}

Position Display::getPosUp(Position pos)
{
    if(isBorderUsed_)
    {
        if(pos.getY()==offset_y_+borderWidth_-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.setY(pos.getY()-1);
            pos.isValid(true);
            return pos;
        }
    }else
    {
        if(pos.getY()==offset_y_-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.isValid(true);
            pos.setY(pos.getY()-1);
            return pos;
        }
    }
}

Position Display::getPosDown(Position pos)
{
    if(isBorderUsed_)
    {
        if(pos.getY()==size_y_+offset_y_+(borderWidth_*2)-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.setY(pos.getY()+1);
            pos.isValid(true);
            return pos;
        }
    }else
    {
        if(pos.getY()==size_y_+offset_y_-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.setY(pos.getY()+1);
            pos.isValid(true);
            return pos;
        }
    }
}

void Display::Log(std::string txt)
{
	std::fstream file("Logs\\Log.txt", std::ios::app);
	file << txt << std::endl;
}

Position Display::getPosLeft(Position pos)
{
    if(isBorderUsed_)
    {
        if(pos.getX()==offset_x_+borderWidth_-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.setX(pos.getX()-1);
            pos.isValid(true);
            return pos;
        }
    }else
    {
        if(pos.getX()==offset_x_-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.setX(pos.getX()-1);
            pos.isValid(true);
            return pos;
        }
    }
}

Position Display::getPosRight(Position pos)
{
    if(isBorderUsed_)
    {
        if(pos.getX()==size_x_+offset_x_+(borderWidth_*2)-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.setX(pos.getX()+1);
            pos.isValid(true);
            return pos;
        }
    }else
    {
        if(pos.getX()==size_x_+offset_x_-1)
        {
            pos.isValid(false);
            return pos;
        }else
        {
            pos.setX(pos.getX()+1);
            pos.isValid(true);
            return pos;
        }
    }
}

Tile& Display::getTileRefAt(Position pos)
{
    return m_map[pos];
}

Tile& Display::getTileRefAt(int x, int y)
{
	return m_map[Position(x, y)];
}

int Display::getFont()
{
	return font_;
}

int Display::getFontSize()
{
	return fontSize_;
}

int Display::getWidth()
{
	return size_x_;
}

int Display::getHeight()
{
	return size_y_;
}

int Display::getVolume()
{
	return volume_;
}

int Display::getMaxWidth()
{
	return size_x_ + offset_x_;
}

int Display::getMaxHeight()
{
	return size_y_ + offset_y_;
}

void Display::claimNameChange(string currentName, string newName)
{
	for (auto& iter : m_map)
	{
		if (iter.second.isClaimedBy(currentName))
		{
			iter.second.forceClaim(newName);
		}
	}
	return;
}

void Display::removeSelectedAtTile(Position pos)
{
	if (isSelected_.count(pos))
		isSelected_.erase(pos);
	updatePos(pos);
}

void Display::removeSelectedAtTileS(Position pos)
{
	if (isSelected_.count(pos))
		isSelected_.erase(pos);
	updatePos(pos);
}


/* Loading/Saving */
///////////////////////////////////

void Display::saveWorld()
{
	if (isLoaded_ == false)
		return;
	stringstream filename;
	if (saveSuffix_ == "0")
		getSaveSuffix();
	filename <<"Saves\\" << "World" << saveSuffix_ << ".dat";
    fstream file(filename.str(), fstream::trunc | fstream::out);

	file.seekp(0, ios_base::beg);

    if(file.is_open() == false)
    {
        file.clear();
        file.open(filename.str(), fstream::app);
    }
    Position pos;

	if (isMultiplayer_)
		file << L_Multi << EndLine;
	else
		file << L_Solo << EndLine;

    for(int x=0;x<size_x_;x++)
    {
        for(int y=0;y<size_y_;y++)
        {
            pos.setX(x);
            pos.setY(y);
            if(!m_map.count(pos))
                continue;
            Tile& tile=m_map[pos];
			tile.serialize(file);
        }
    }
	game::pHandler.serializeAll(file);
	game::system.serialize(file);
	file << LOAD::END;
    file.close();
}

void Display::saveWorld(string filename)
{
	fstream file(filename, fstream::trunc | fstream::out);

	file.seekp(0, ios_base::beg);

	if (file.is_open() == false)
	{
		file.clear();
		file.open(filename, fstream::app);
	}
	Position pos;

	if (isMultiplayer_)
		file << L_Multi << EndLine;
	else
		file << L_Solo << EndLine;

	file << playerAmount_ << EndLine;

	for (int x = 0; x<size_x_; x++)
	{
		for (int y = 0; y<size_y_; y++)
		{
			pos.setX(x);
			pos.setY(y);
			if (!m_map.count(pos))
				continue;
			Tile& tile = m_map[pos];
			tile.serialize(file);
		}
	}
	game::pHandler.serializeAll(file);
	game::system.serialize(file);
	file << LOAD::END;
	file.close();
}

string Display::getWorld()
{
	if (isLoaded_ == false)
	{
		return string((char*)LOAD::END);
	}

	stringstream world;
	for (auto& iter : m_map)
	{
		world << iter.second.serialize(true);
	}
	world << LOAD::END << EndLine;
	return world.str();
}

void Display::loadWorld(string filename)
{
    fstream file(filename);
	if (file.is_open() == false)
	{
		newWorld();
		saveWorld();
		isLoaded_ = true;
		return;
	}

	saveSuffix_=filename.substr(filename.find("d")+1, 2);

	file.seekp(0, ios_base::beg);

	game::system.clear();

	int text;
	file >> text;

	if (text == L_Solo)
	{
		isMultiplayer_ = false;
	}
	else
	{
		isMultiplayer_ = true;
	}

	file >> text;

	bool localLoaded_ = false;

	while (text != LOAD::END )
	{
		if (text == LOAD::L_Tile)
		{
			file.clear();
			Tile tile;
			tile.deserialize(file);
			m_map[tile.getPos()] = tile;
			file.clear();
		}
		if (text == LOAD::L_Player)
		{
			if (localLoaded_ == false)
			{
				game::pHandler.getLocalPlayer().deserialize(file);
				localLoaded_ = true;
			}else
			{
				game::pHandler.addPlayerDeserialize(file);
			}
			playerAmount_++;
		}
		if (text == LOAD::L_Bullet)
		{
			std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
			bullet->deserialize(file);
			game::system.addEntity(bullet);
		}
		if (text == LOAD::L_Turret)
		{
			std::shared_ptr<Turret> turret = std::make_shared<Turret>();
			turret->deserialize(file);
			game::system.addEntity(turret);
		}
		file.clear();
		file >> text;
	}

	reloadAll_ = true;
	isLoaded_ = true;
}

void Display::loadWorldServer(string data)
{
	stringstream msg;
	msg << data;

	int text;
	msg >> text;

	while (text != LOAD::END)
	{
		if (text == L_Tile)
		{
			msg.clear();
			Tile tile;
			tile.deserialize(msg);
			m_map[tile.getPos()] = tile;
			msg.clear();
		}
		if (text == L_Player)
		{
			Player player;
			player.deserialize(msg);
			game::pHandler.addPlayer(player);
		}
		msg.clear();
		msg >> text;
	}

	reloadAll_ = true;
	isLoaded_ = true;
}

void Display::loadWorldServer(stringstream& msg)
{
	int text;
	msg >> text;

	while (text != LOAD::END)
	{
		if (text == L_Tile)
		{
			msg.clear();
			Tile tile;
			tile.deserialize(msg);
			m_map[tile.getPos()] = tile;
			msg.clear();
		}
		if (text == L_Player)
		{
			Player player;
			player.deserialize(msg);
			game::pHandler.addPlayer(player);
		}
		msg.clear();
		msg >> text;
	}

	reloadAll_ = true;
	isLoaded_ = true;
}

void Display::loadWorld()
{
	stringstream filename;
	getSaveSuffix();
	filename <<"Save\\" << "World" << saveSuffix_ << ".dat";
	fstream file(filename.str());
	if (file.is_open() == false)
	{
		newWorld();
		saveWorld();
		isLoaded_ = true;
		return;
	}

	file.seekp(0, ios_base::beg);

	int text;
	file >> text;

	if (text == L_Solo)
	{
		isMultiplayer_ = false;
	}
	else
	{
		isMultiplayer_ = true;
	}

	file >> text;

	bool localLoaded_ = false;

	game::system.clear();

	while (text!=LOAD::END)
	{
		if (text == LOAD::L_Tile)
		{
			file.clear();
			Tile tile;
			tile.deserialize(file);
			m_map[tile.getPos()] = tile;
			file.clear();
		}
		if (text == LOAD::L_Player)
		{
			if (localLoaded_ == false)
			{
				game::pHandler.getLocalPlayer().deserialize(file);
				localLoaded_ = true;
			}
			else
			{
				game::pHandler.addPlayerDeserialize(file);
			}
		}
		if (text == LOAD::L_Bullet)
		{
			std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
			bullet->deserialize(file);	
			game::system.addEntity(bullet);
		}
		if (text == LOAD::L_Turret)
		{
			std::shared_ptr<Turret> turret = std::make_shared<Turret>();
			turret->deserialize(file);
			game::system.addEntity(turret);
		}
		file.clear();
		file >> text;
	}

	reloadAll_ = true;
	isLoaded_ = true;
}

void Display::newWorld()
{
	game::system.clear();

    Position newPos(1,1);

	getSaveSuffix();

	game::pHandler.getLocalPlayer().reset();

    Tile gold;
    gold.setGraphic(TG_Gold);
    gold.setColor(TGC_Gold);
    gold.setBackground(TGB_Gold);
    gold.isDestructable(true);
    gold.isWall(true);
    gold.isWalkable(false);
    gold.isClaimable(false);
    gold.setHealth(150);
    gold.setMaxHealth(150);

    Tile stoneFloor;
    stoneFloor.setColor(TGC_StoneFloor);
    stoneFloor.setGraphic(TG_StoneFloor);
    stoneFloor.setBackground(TGB_StoneFloor);
    stoneFloor.isDestructable(false);
    stoneFloor.isWall(false);
    stoneFloor.isWalkable(true);
    stoneFloor.forceClaim(game::pHandler.getLocalPlayer().getName());
    stoneFloor.isClaimable(true);
	stoneFloor.setHealth(100);
	stoneFloor.setMaxHealth(100);

    Tile core;
    core.setGraphic('C');
    core.setColor(TC_Gray);
	core.setBackground(B_DarkGray);
    core.isWall(false);
    core.isWalkable(false);
    core.isDestructable(true);
    core.isClaimable(false);
    core.setMaxHealth(2500);
    core.setHealth(2500);
    core.setClaimedBy(game::pHandler.getLocalPlayer().getName());

	Tile stone;
    stone.setGraphic(TG_Stone);
    stone.setColor(TGC_Stone);
    stone.setBackground(TGB_Stone);
    stone.isDestructable(true);
    stone.setMaxHealth(100);
    stone.setHealth(100);
    stone.isWall(true);

    Position startPos(rand() % 50, rand() % 20);
    Position corePos(rand() % 50, rand() % 20);
    int key = rand() % 15;
    for(int x=0;x<size_x_;x++)
    {
        for(int y=0;y<size_y_;y++)
        {
            newPos.setX(x);
            newPos.setY(y);
            if((rand() % 15)==key)
            {
                gold.setGoldAmount((rand() % 500)+101);
                gold.setPos(newPos);
                m_map[newPos]=gold;
            }
            else
            {
                stone.setPos(newPos);
                m_map[newPos]=stone;
            }
        }
    }
	core.setPos(corePos);
	stoneFloor.setPos(startPos);
    m_map[startPos]=stoneFloor;
    m_map[corePos]=core;
	isLoaded_ = true;
	std::stringstream txt;
	game::pHandler.getLocalPlayer().forceHandPosition(startPos, *this);
	game::pHandler.getLocalPlayer().setSpawnPos(startPos);
	reloadAll_ = true;
	isMultiplayer_ = false;
}

void Display::newWorldMulti()
{
	game::system.clear();

	Position newPos(0, 0);

	getSaveSuffix();

	game::pHandler.getLocalPlayer().reset();

	Tile gold;
	gold.setGraphic(TG_Gold);
	gold.setColor(TGC_Gold);
	gold.setBackground(TGB_Gold);
	gold.isDestructable(true);
	gold.isWall(true);
	gold.isWalkable(false);
	gold.isClaimable(false);
	gold.setHealth(150);
	gold.setMaxHealth(150);

	Tile stoneFloor;
	stoneFloor.setColor(TGC_StoneFloor);
	stoneFloor.setGraphic(TG_StoneFloor);
	stoneFloor.setBackground(TGB_StoneFloor);
	stoneFloor.isDestructable(false);
	stoneFloor.isWall(false);
	stoneFloor.isWalkable(true);
	stoneFloor.forceClaim(game::pHandler.getLocalPlayer().getName());
	stoneFloor.isClaimable(true);
	stoneFloor.setHealth(100);
	stoneFloor.setMaxHealth(100);

	Tile core;
	core.setGraphic('C');
	core.setColor(TC_Gray);
	core.setBackground(B_DarkGray);
	core.isWall(false);
	core.isWalkable(false);
	core.isDestructable(true);
	core.isClaimable(false);
	core.setMaxHealth(2500);
	core.setHealth(2500);
	core.setClaimedBy(game::pHandler.getLocalPlayer().getName());

	Tile stone;
	stone.setGraphic(TG_Stone);
	stone.setColor(TGC_Stone);
	stone.setBackground(TGB_Stone);
	stone.isDestructable(true);
	stone.setMaxHealth(100);
	stone.setHealth(100);
	stone.isWall(true);

	//Position startPos(rand() % 50, rand() % 20);
	//Position corePos(rand() % 50, rand() % 20);
	Position startPos(0, 1);
	Position corePos(0, 0);
	int key = rand() % 15;
	for (int x = 0; x<size_x_; x++)
	{
		for (int y = 0; y<size_y_; y++)
		{
			newPos.setX(x);
			newPos.setY(y);
			if ((rand() % 15) == key)
			{
				gold.setGoldAmount((rand() % 500) + 101);
				gold.setPos(newPos);
				m_map[newPos] = gold;
			}
			else
			{
				stone.setPos(newPos);
				m_map[newPos] = stone;
			}
		}
	}


	isLoaded_ = true;
	reloadAll_ = true;
	isMultiplayer_ = true;
}

void Display::newWorldMulti(int pAmount, std::string names[])
{
	map<int, Position> p_pos;
	map<int, Position> p_cpos;
	p_pos[1] = Position(74, 28);
	p_cpos[1] = Position(74, 29);
	p_pos[2] = Position(74, 1);
	p_cpos[2] = Position(74, 0);
	p_pos[3] = Position(1, 29);
	p_cpos[3] = Position(0, 29);
	

	game::system.clear();

	Position newPos(0, 0);

	getSaveSuffix();

	game::pHandler.getLocalPlayer().reset();

	Tile gold;
	gold.setGraphic(TG_Gold);
	gold.setColor(TGC_Gold);
	gold.setBackground(TGB_Gold);
	gold.isDestructable(true);
	gold.isWall(true);
	gold.isWalkable(false);
	gold.isClaimable(false);
	gold.setHealth(150);
	gold.setMaxHealth(150);

	Tile core;
	core.setGraphic('C');
	core.setColor(TC_Gray);
	core.setBackground(B_DarkGray);
	core.isWall(false);
	core.isWalkable(false);
	core.isDestructable(true);
	core.isClaimable(false);
	core.setMaxHealth(2500);
	core.setHealth(2500);
	core.setClaimedBy(game::pHandler.getLocalPlayer().getName());

	Tile stone;
	stone.setGraphic(TG_Stone);
	stone.setColor(TGC_Stone);
	stone.setBackground(TGB_Stone);
	stone.isDestructable(true);
	stone.setMaxHealth(100);
	stone.setHealth(100);
	stone.isWall(true);

	int key = rand() % 15;
	for (int x = 0; x < size_x_; x++)
	{
		for (int y = 0; y < size_y_; y++)
		{
			newPos.setX(x);
			newPos.setY(y);
			if ((rand() % 15) == key)
			{
				gold.setGoldAmount((rand() % 500) + 101);
				gold.setPos(newPos);
				m_map[newPos] = gold;
			}
			else
			{
				stone.setPos(newPos);
				m_map[newPos] = stone;
			}
		}
	}

	/* Local */
	////////////////////
	Common::SendPlayer(&Common::CreatePlayer(Position(0,1), names[0], true), pAmount, 0);
	Common::CreatePlayerCore(names[0], Position(0, 0));
	Common::SetStoneFloorAt(Position(0,1), names[0]);
	////////////////////

	for (int x = 1; x < pAmount; x++)
	{
		Common::SendPlayer(&Common::CreatePlayer(p_pos[x], names[x]), pAmount, x);
		Common::SetStoneFloorAt(p_pos[x], names[x]);
		Common::CreatePlayerCore(names[x], p_cpos[x]);
	}

	isLoaded_ = true;
	reloadAll_ = true;
	isMultiplayer_ = true;
}

void Display::getSaveSuffix()
{
	int worldAmount = 1;
	bool exitFlag = false;
	while (exitFlag == false)
	{
		if (worldAmount < 10)
		{
			stringstream filename;
			filename << "Saves\\" << "World" << "0" << worldAmount << ".dat";
			fstream file(filename.str());
			if (file.is_open() == false)
			{
				stringstream name;
				name << "0" << worldAmount;
				saveSuffix_ = name.str();
				exitFlag = true;
			}
			file.close();
		}
		else
		{
			stringstream filename;
			filename << "Saves\\" << "World" << worldAmount << ".dat";
			fstream file(filename.str());
			if (file.is_open() == false)
			{
				stringstream suffix;
				suffix << worldAmount;
				saveSuffix_ = suffix.str();
				exitFlag = true;
			}
			file.close();
		}
		worldAmount++;
	}
}

int Display::getSaveAmount()
{
	int worldAmount = 1;
	int realWorldAmount = 0;
	bool exitFlag = false;
	while (exitFlag == false)
	{
		if (worldAmount < 10)
		{
			stringstream filename;
			filename << "Saves\\" << "World" << "0" << worldAmount << ".dat";
			fstream file(filename.str());
			if (file.is_open() == false)
			{
				exitFlag = true;
				file.close();
				continue;
			}
			realWorldAmount++;
			file.close();
		}
		else
		{
			stringstream filename;
			filename << "Saves\\" << "World" << worldAmount << ".dat";
			fstream file(filename.str());
			if (file.is_open() == false)
			{
				exitFlag = true;
				file.close();
				continue;
			}
			realWorldAmount++;
			file.close();
		}
		worldAmount++;
	}
	return realWorldAmount;
}

void Display::saveSettings()
{
	fstream file("Settings.txt");
	if (!file)
	{
		file.open("Settings.txt", ios::app);
	}
	file << "Settings" << endl
	     << "Font: " << font_ << endl
	     << "FontSize: " << fontSize_ << endl
	     << "FullScreen: " << isFullscreen_ << endl
	     << "Name: " << game::pHandler.getLocalPlayer().getName() << endl
		 << "Volume: " << volume_ << endl;
}

bool Display::loadSettings()
{
	fstream file("Settings.txt");
	if (!file)
	{
		file.open("Settings.txt", ios::app);
		file << "Settings" << endl;
		file << "Font: " << 1 << endl;
		file << "FontSize: " << 28 << endl;
		file << "FullScreen: " << 0 << endl;
		file << "Name: Player" << endl;
		file << "Volume: " << 50 << endl;
		return false;
	}
	else
	{
		string none;
		string name;
		file >> none;
		file >> none;
		file >> font_;
		file >> none;
		file >> fontSize_;
		file >> none;
		file >> isFullscreen_;
		file >> none;
		file >> name;
		file >> none;
		file >> volume_;
		setFontInfo(fontSize_, font_);
		game::pHandler.getLocalPlayer().setName(name);
		game::pHandler.addLocalPlayer(game::pHandler.getLocalPlayer());
		if (isFullscreen_)
		{
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD flags = CONSOLE_FULLSCREEN_MODE;
			COORD pos;
			SetConsoleDisplayMode(h, flags, &pos);
		}
		return true;
	}
}
///////////////////////////////////
