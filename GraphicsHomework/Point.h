#pragma once
class Point
{
public:
	Point();
	Point(double x, double y);
	double getX();
	double getY();
	void setX(double x);
	void setY(double y);
private:
	double x, y;
};