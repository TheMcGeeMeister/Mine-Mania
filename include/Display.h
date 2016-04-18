#pragma once

#include <Position.h>
#include <Tile.h>
#include <map>
#include <windows.h>
#include <list>
#include <iostream>
#include <sstream>
#include "..\Mine-Mania\include\Packet.h"
#include <Direction.h>

using namespace std;

class Display
{
public:
    Display();
    virtual ~Display();
    void setTileAt(Position, Tile);
	void setTileAtS(Position, Tile);
	void setTileAsSelected(Position);
	void setTileAsSelectedS(Position);
    void setSizeX(int x);
    void setSizeY(int y);
	void setFont(int font);
	void setFontSize(int fontSize);
	void isHidden(bool hidden);
	void isFullScreen(bool is);

    bool isTileNear(Tile& Seek, Position pos);
    bool isWalkableTileNear(Position pos);
	bool isClaimedTileNear(Position pos, string underlord);
	bool isLoaded();
	bool isHidden();
	bool isValidPosition(Position pos, bool player);
	bool isValidPosition(Position pos);
	bool isFullScreen();

	void updateTileServer(Position pos);

	/* Search Method */
	Position searchLine(Position sPos, DIRECTION direction, int amount, char target);

    /* Loading/Saving */
    ///////////////////////////////////
	void loadWorld();
    void loadWorld(string filename);
	void loadWorldServer(string data);
	void loadWorldServer(stringstream& data);
    void saveWorld();
	void saveWorld(string filename);
	string getWorld();
    void newWorld();
	void newWorldMulti();
	void newWorldMulti(int pAmount, std::string names[]);
	void getSaveSuffix();
	int getSaveAmount();

	void saveSettings();
	bool loadSettings();
    ///////////////////////////////////

    Tile& getTileRefAt(Position);
	Tile& getTileRefAt(int x, int y);

	bool isPacketsAvailable();
	list<Packet>& getPackets();
	void addPacket(std::string);
	void clearPackets();

	int getFont();
	int getFontSize();
	int getMaxWidth();
	int getMaxHeight();
	int getWidth();
	int getHeight();

	void claimNameChange(string currentName, string newName);

	void removeSelectedAtTile(Position);
	void removeSelectedAtTileS(Position);

    void update();
    void updatePos(Position);
    void reloadAll();
	void cleanOverlays(); // Sets all tile overlaysEnabled to false, and sets overlay graphic to ' '
protected:

private:
    void setPos(Position, char);
    void setPos(Position, char, WORD);
    void setPos(Position, WORD);

    Position getPosLeft(Position pos);
    Position getPosRight(Position pos);
    Position getPosUp(Position pos);
    Position getPosDown(Position pos);


    map<Position, Tile> m_map;
    map<Position, bool> isSeen_;
	map<Position, bool> isSelected_;
    list<pair<Position, Tile> > tileChanges_;
	list<Packet> packetsBuffer_;
    int size_x_, size_y_, offset_x_, offset_y_, borderWidth_;

	void Log(std::string);



    //Windows Console Output Stuff//
    HANDLE h;
    DWORD nlength;
    DWORD output;
    char readConsole_;
    WORD readAttribute_;
    COORD pos;
    bool isBorderUsed_;
	bool isHidden_;
    bool reloadAll_;
	bool isLoaded_;
	string saveSuffix_;

	int font_;
	int fontSize_;
	bool isFullscreen_;
    //End//


};
