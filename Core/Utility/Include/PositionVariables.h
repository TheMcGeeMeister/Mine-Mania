#pragma once

#include <Position.h>

class PositionVariables
{
public:
	PositionVariables();
	PositionVariables(unsigned int size_x, unsigned int size_y);
	PositionVariables(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y);
	PositionVariables(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y, unsigned int border_width);

	void operator=(PositionVariables& pv);

	bool isInside(Position& pos);

	unsigned int size_x;
	unsigned int size_y;
	unsigned int offset_x;
	unsigned int offset_y;
	unsigned int border_width;
};

