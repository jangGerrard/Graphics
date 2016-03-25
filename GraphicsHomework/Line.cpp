#include "Line.h"

void Line::setStartPoint(Point start)
{
	this->start = start;
}
void Line::setEndPoint(Point end)
{
	this->end = end;
}

void Line::draw()
{
	glBegin(GL_LINES);
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
	glVertex2d(this->start.getX(), this->start.getY());
	glVertex2d(this->end.getX(), this->end.getY());
	glEnd();
}

void Line::setMotionPoint(Point point)
{
	this->end = point;
}

