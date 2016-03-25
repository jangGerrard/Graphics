#pragma once
#include "Polygon.h"
#include "Point.h"
#include <gl/glut.h>

class Line : public Polygon
{
public:

	void setStartPoint(Point start);
	void setEndPoint(Point end);
	void draw();
	void setMotionPoint(Point point);

private:
	Point start;
	Point end;
};