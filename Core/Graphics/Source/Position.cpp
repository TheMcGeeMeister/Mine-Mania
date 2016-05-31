#include <sstream>
#include <fstream>
#include "Position.h"
#include "Common.h"

#define EndLine "\n"

using namespace std;

Position::Position()
{
	x_ = 1;
	y_ = 1;
    isValid_ = true;
}

Position::Position(int _x, int _y)
{
    x_=_x;
    y_=_y;
    isValid_ = true;
    return;
}

Position::~Position()
{
    //dtor
}

void Position::setX(int _x)
{
    x_ = _x;
}

void Position::setY(int _y)
{
    y_ = _y;
}

void Position::isValid(bool isValid)
{
    isValid_ = isValid;
}

int Position::getX() const
{
    return x_;
}

int Position::getY() const
{
    return y_;
}

int& Position::getRefX()
{
    return x_;
}

int& Position::getRefY()
{
    return y_;
}

bool Position::isValid() const
{
    return isValid_;
}

void Position::incrementX(int i)
{
    x_++;
}

void Position::incrementY(int i)
{
    y_++;
}

void Position::decrementX(int i)
{
    x_--;
}

void Position::decrementY(int i)
{
    y_--;
}

void Position::up()
{
	if (y_ == 0)
	{
		isValid_ = false;
		return;
	}
	else
	{
		y_--;
		return;
	}
}

void Position::down()
{
	y_++;
	if (y_ == Common::GetDisplayMaxHeight())
	{
		y_--;
		isValid_ = false;
	}
	else
	{
		return;
	}
}

void Position::left()
{
	if (x_ == 0)
	{
		isValid_ = false;
		return;
	}
	else
		x_--;
}

void Position::right()
{
	x_++;
	if (x_ == Common::GetDisplayMaxWidth())
	{
		x_--;
		isValid_ = false;
	}
	else
	{
		return;
	}
}

bool Position::go(DIRECTION direction)
{
	switch (direction)
	{
	case DIRECTION_UP: up(); break;
	case DIRECTION_DOWN: down(); break;
	case DIRECTION_LEFT: left(); break;
	case DIRECTION_RIGHT: right(); break;
	}
	return isValid_;
}

bool Position::go(DIRECTION direction, int amount)
{
	for (int x = 0; x < amount; x++)
	{
		go(direction);
		if (isValid_ == false)
		{
			return false;
		}
	}
	return isValid_;
}

bool Position::operator<(const Position& r) const
{
    if (getX() < r.getX())  return true;
    if (getX() > r.getX())  return false;
    if (getY() < r.getY())  return true;
    if (getY() > r.getY())  return false;
	
    return false;
}

void Position::operator()(int _x, int _y)
{
	_x = x_;
	_y = y_;
}

void Position::operator=(Position& pos)
{
	x_ = pos.getX();
	y_ = pos.getY();
	isValid_ = pos.isValid();
}

void Position::operator=(const Position& pos)
{
	x_ = pos.x_;
	y_ = pos.y_;
	isValid_ = pos.isValid_;
}

bool Position::operator==(const Position & pos)
{
	if (x_ == pos.x_ & y_ == pos.y_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string Position::serializeR()
{
	std::stringstream msg;
	msg << x_ << EndLine << y_ << EndLine;
	return msg.str();
}

void Position::serialize(std::stringstream & stream)
{
	stream << x_ << EndLine << y_ << EndLine;
}

void Position::serialize(std::fstream & stream)
{
	stream << x_ << EndLine << y_ << EndLine;
}

void Position::deserialize(std::stringstream & file)
{
	file >> x_
		>> y_;
}

void Position::deserialize(std::fstream & file)
{
	file >> x_
		>> y_;
}

std::pair<int, int> Position::getPair()
{
    pair<int, int> newPair;
    newPair.first=x_;
    newPair.second=y_;
    return newPair;
}
