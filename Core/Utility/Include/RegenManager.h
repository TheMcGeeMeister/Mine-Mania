#ifndef REGENMANAGER_H
#define REGENMANAGER_H
#include <list>
#include <Timer.h>
#include <Position.h>

class Display;

using namespace std;

class RegenManager
{
    public:
        RegenManager();
        virtual ~RegenManager();
        void update(Display&);
        void push_back(Position);
    protected:

    private:
        list<Position> Tiles;
        Timer RegenCoolDown;

};

#endif // REGENMANAGER_H
