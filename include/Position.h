#ifndef POSITION_H
#define POSITION_H
#include <utility>
#include <Direction.h>
#include <string>

class Position
{
    public:
        Position();
        Position(int _x, int _y);
        virtual ~Position();

        void setX(int x_);
        void setY(int y_);
        void isValid(bool);
        std::pair<int, int> getPair();

        int getX() const;
        int getY() const;
        int& getRefX();
        int& getRefY();
        bool isValid() const;
        void incrementX(int);
        void incrementY(int);
        void decrementX(int);
        void decrementY(int);
		void up();
		void down();
		void left();
		void right();

		bool go(DIRECTION direction);
		bool go(DIRECTION direction, int amount);

        bool operator<(const Position&) const;
		void operator()(int _x, int _y);

        void operator=(Position& pos);
		void operator=(const Position& pos);

		bool operator==(const Position& pos);

		std::string serializeR();

    protected:

    private:
        int x_;
        int y_;
        bool isValid_;
};

#endif // POSITION_H
