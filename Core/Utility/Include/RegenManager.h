#pragma once
#include <list>
#include "Timer.h"
#include "Position.h"

class Display;

using namespace std;

class RegenManager
{
    public:
        RegenManager();
        virtual ~RegenManager();
        void update();
        void push_back(Position);
    protected:

    private:
        list<Position> Tiles;
        Timer RegenCoolDown;

};
