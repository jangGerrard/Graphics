#include "PolyLine.h"

void PolyLine::draw()
{
	glBegin(GL_LINE_STRIP);
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
	for (int i = 0; i < this->points.size();i++)
	{
		glVertex2d(points.at(i).getX(), points.at(i).getY());
	}
	glEnd();
}
void PolyLine::setMotionPoint(Point point)
{
	points.push_back(point);
}
void PolyLine::setPoint(Point point)
{
	points.push_back(point);
}
