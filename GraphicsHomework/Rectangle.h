#pragma once
#include "Polygon.h"
#include "Point.h"
#include <gl/glut.h>

class Rectangle : public Polygon
{
public:
	void draw();
	void setMotionPoint(Point point);

	void setLeftTopPoint(Point leftTop);
	void setRightBottomPoint(Point rightBottom);
private :
	Point leftTop;
	Point rightBottom;
};
