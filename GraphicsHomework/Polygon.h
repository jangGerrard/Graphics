#pragma once
#include "Point.h"

class Polygon
{
public:
	void setModeAndColor(int mode, int color);
	int getMode();
	int getColor();
	virtual void draw() =0;
	virtual void setMotionPoint(Point point) = 0;
protected:
	int mode;
	int color;
};