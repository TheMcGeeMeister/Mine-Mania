#include "RegenManager.h"
#include "Tile.h"
#include "Display.h"

namespace game
{
	extern Display game;
}

RegenManager::RegenManager()
{
    //ctor
}

RegenManager::~RegenManager()
{
    //dtor
}

void RegenManager::update()
{
    if(RegenCoolDown.Update()==true)
    {
        list<list<Position>::iterator> DeleteQueue;
        auto iter=Tiles.begin();
        auto iEnd=Tiles.end();
        for(;iter!=iEnd;++iter)
        {
            Tile& tile=game::game.getTileRefAt(*iter);
            tile.incrementHealth(2);
            if(tile.getHealth()==tile.getMaxHealth())
            {
                DeleteQueue.push_back(iter);
            }
        }
        for(auto iter : DeleteQueue)
        {
            Tiles.erase(iter);
        }
        RegenCoolDown.StartNewTimer(1);
    }
}

void RegenManager::push_back(Position pos)
{
   Tiles.push_back(pos);
}
