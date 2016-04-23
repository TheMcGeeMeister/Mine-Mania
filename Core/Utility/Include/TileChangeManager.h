#ifndef TILECHANGEMANAGER_H
#define TILECHANGEMANAGER_H
#include <List>
#include <Position.h>

class Display;

class TileChangeManager
{
    public:
        TileChangeManager();
        virtual ~TileChangeManager();
        void update(Display&);
        void push_back(Position pos);
    protected:

    private:
        std::list<Position> Changes;
};

#endif // TILECHANGEMANAGER_H
