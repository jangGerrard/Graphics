#pragma once
#include "Point.h"
class Math {
public:
	Point crossProduct(Point a, Point b);
	Point normalVector(Point a, Point b, Point c);
	Point normalVector(Point p);
};