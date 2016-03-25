#pragma once
#include "Point.h"
Point::Point() {}
Point::Point(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }
Point Point::abs()
{
	Point result;
	if (this->x < 0) {
		result.x = -(this->x);
	}
	if (this->y < 0) {
		result.y = -(this->y);
	}
	if (this->z < 0) {
		result.z = -(this->z);
	}

	return result;
}

Point operator- (Point a, Point b)
{
	Point result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}

Point operator+ (Point a, Point b)
{
	Point result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}


Point operator / (Point a, Point b)
{
	Point result;
	result.x = (double)a.x / b.x;
	result.y = (double)a.y / b.y;
	result.z = (double)a.z / b.z;
	return result;
}
