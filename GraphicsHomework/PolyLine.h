#pragma once
#include "Polygon.h"
#include "Point.h"
#include <vector>
#include <gl/glut.h>


class PolyLine : public Polygon
{
public:
	void draw();
	void setMotionPoint(Point point);
	void setPoint(Point point);
private:
	std::vector<Point> points;
};
