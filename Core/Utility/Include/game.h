#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <TileChangeManager.h>
#include <RegenManager.h>

class Tile;

namespace game
{
    extern TileChangeManager TileHandler;
    extern RegenManager RegenHandler;
}

namespace gameTiles
{
	extern Tile stone_wall;
	extern Tile stone_wall_gold;
	extern Tile stone_floor;
}

extern void updateTileInfo();

//void connectMenu(thread & sThread, bool & threadStarted);

#endif // GAME_H_INCLUDED
