#include "TileChangeManager.h"
#include "Display.h"

namespace game
{
	extern Display game;
}

TileChangeManager::TileChangeManager()
{
    //ctor
}

TileChangeManager::~TileChangeManager()
{
    //dtor
}

void TileChangeManager::push_back(Position pos)
{
    Changes.push_back(pos);
}

void TileChangeManager::update()
{
    for(auto &i : Changes)
    {
        game::game.updatePos(i);
    }
    Changes.clear();
}
