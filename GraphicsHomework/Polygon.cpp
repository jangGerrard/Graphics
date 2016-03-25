#include "Polygon.h"

#pragma once
#include "Point.h"

void Polygon::setModeAndColor(int mode, int color)
{
	this->mode = mode;
	this->color = color;
}
int Polygon::getMode()
{
	return this->mode;
}
int Polygon::getColor()
{
	return this->color;
}
