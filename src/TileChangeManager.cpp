#include "TileChangeManager.h"
#include <Display.h>

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

void TileChangeManager::update(Display& game)
{
    for(auto &i : Changes)
    {
        game.updatePos(i);
    }
    Changes.clear();
}
