#include "Rectangle.h"

void Rectangle::draw()
{
	glBegin(GL_QUADS);
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
	glVertex2d(leftTop.getX(), rightBottom.getY());
	glVertex2d(rightBottom.getX(), rightBottom.getY());
	glVertex2d(rightBottom.getX(), leftTop.getY());
	glVertex2d(leftTop.getX(), leftTop.getY());
	glEnd();
}
void Rectangle::setMotionPoint(Point point)
{
	this->rightBottom = point;
}

void Rectangle::setLeftTopPoint(Point leftTop)
{
	this->leftTop = leftTop;
}
void Rectangle::setRightBottomPoint(Point rightBottom)
{
	this->rightBottom = rightBottom;
}
