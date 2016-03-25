#pragma once
#include <math.h>
#include "Point.h"
#include "Math.h"
Point Math::crossProduct(Point a, Point b)
{
	Point result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;

	return result;
}
Point Math::normalVector(Point a, Point b, Point c)
{
	Point N = crossProduct(b - a, c - a);
	double length = sqrt(N.x*N.x + N.y*N.y + N.z*N.z);
	Point n;
	n.x = N.x / length;
	n.y = N.y / length;
	n.z = N.z / length;
	return n;
}
Point Math::normalVector(Point p)
{
	double length = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
	Point n;

	n.x = p.x / length;
	n.y = p.y / length;
	n.z = p.z / length;

	return n;
}
