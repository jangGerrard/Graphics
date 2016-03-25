#include "Circle.h"

void Circle::draw()
{

	glBegin(GL_TRIANGLE_FAN);
	switch (this->color) {
	case 0:
		glColor3d(1, 0, 0);
		break;
	case 1:
		glColor3d(0, 1, 0);
		break;
	case 2:
		glColor3d(0, 0, 1);
		break;
	case 3:
		glColor3d(0, 0, 0);
		break;
	}

	const double PI = 3.141592;
	int triangleCount = 200;
	float twicePI = 2.0f * PI;

	double radius;///피타고라스를 이용해서 거리를 구했습니다.
	radius = sqrt(pow((this->end.getX() - this->start.getX()), 2.0) 
												+ pow((this->end.getY() - this->start.getY()), 2.0));

	glVertex2d(this->start.getX(), this->start.getY());

	for (int i = 0;i <= triangleCount; i++)
	{
		glVertex2f(
			this->start.getX() + (radius * cos(i * twicePI / triangleCount)),
			this->start.getY() + (radius * sin(i * twicePI / triangleCount))
			);
	}
	glEnd();
}
void Circle::setMotionPoint(Point point)
{
	this->end = point;
}
void Circle::setStartPoint(Point start)
{
	this->start = start;
}
