#pragma once
class Point {

public:
	Point();
	Point(double x, double y, double z);
	Point abs();

	friend Point operator- (Point a, Point b);
	friend Point operator+ (Point a, Point b);
	friend Point operator / (Point a, Point b);

	double x;
	double y;
	double z;

};
