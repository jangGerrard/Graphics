#pragma once
#include "Point.h"
#include "Polygon.h"
#include <gl/glut.h>

#include <math.h>

class Circle : public Polygon
{
public:
	void draw();
	void setMotionPoint(Point point);
	void setStartPoint(Point start);
private:
	Point start;
	Point end;
};