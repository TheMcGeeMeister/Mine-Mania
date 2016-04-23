#include "..\include\PositionVariables.h"



PositionVariables::PositionVariables()
{
	size_x = 0;
	size_y = 0;
	offset_x = 0;
	offset_y = 0;
	border_width = 0;
}

PositionVariables::PositionVariables(unsigned int size_x, unsigned int size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	offset_x = 0;
	offset_y = 0;
	border_width = 0;
}


PositionVariables::PositionVariables(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	this->offset_x = offset_x;
	this->offset_y = offset_y;
	border_width = 0;
}

PositionVariables::PositionVariables(unsigned int size_x, unsigned int size_y, unsigned int offset_x, unsigned int offset_y, unsigned int border_width)
{
	this->size_x = size_x;
	this->size_y = size_y;
	this->offset_x = offset_x;
	this->offset_y = offset_y;
	this->border_width = border_width;
}

void PositionVariables::operator=(PositionVariables & pv)
{
	size_x = pv.size_x;
	size_y = pv.size_y;
	offset_x = pv.offset_x;
	offset_y = pv.offset_y;
	border_width = pv.border_width;
	return;
}

bool PositionVariables::isInside(Position& pos)
{
	return true;
}
