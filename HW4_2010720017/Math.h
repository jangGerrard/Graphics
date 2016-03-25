#pragma once
#include <math.h>
struct Point
{
	float p[3];
};

struct Vector
{
	float p[3];
};

struct Matrix
{
	float m[9];
};

void SetPoint(Point * p, float a, float b, float c)
{
	p->p[0] = a;
	p->p[1] = b;
	p->p[2] = c;
}

void SetVector(Vector * v, float a, float b, float c)
{
	v->p[0] = a;
	v->p[1] = b;
	v->p[2] = c;
}

void SetMatrix(Matrix * m, float _00, float _01, float _02,
	float _10, float _11, float _12,
	float _20, float _21, float _22)
{
	m->m[0] = _00; m->m[1] = _01;m->m[2] = _02;
	m->m[3] = _10; m->m[4] = _11;m->m[5] = _12;
	m->m[6] = _20; m->m[7] = _21;m->m[8] = _22;
}

float DotProduct(Vector * v1, Vector * v2)
{
	float s = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		s += v1->p[i] * v2->p[i];
	}
	return s;
}

Vector CrossProduct(Vector * v1, Vector * v2)
{
	Vector v3;

	v3.p[0] = (v1->p[1] * v2->p[2]) - (v1->p[2] * v2->p[1]);
	v3.p[1] = (v1->p[2] * v2->p[0]) - (v1->p[0] * v2->p[2]);
	v3.p[2] = (v1->p[0] * v2->p[1]) - (v1->p[1] * v2->p[0]);

	return v3;
}

float VecLength(Vector * v)
{
	return sqrt((v->p[0] * v->p[0]) + (v->p[1] * v->p[1]) + (v->p[2] * v->p[2]));
}

Vector Normalize(Vector * v)
{
	Vector v1;
	float length = VecLength(v);
	v1.p[0] = v->p[0] / length;
	v1.p[1] = v->p[1] / length;
	v1.p[2] = v->p[2] / length;

	return v1;
}

Vector InverseVector(Vector * v)
{
	Vector v1;
	for (int i = 0; i < 3; i++)
		v1.p[i] = -(v->p[i]);

	return v1;
}

Matrix MatrixTranspose(Matrix * m)
{
	Matrix c;

	c.m[0] = m->m[0];
	c.m[1] = m->m[3];
	c.m[2] = m->m[6];

	c.m[3] = m->m[1];
	c.m[4] = m->m[4];
	c.m[5] = m->m[7];

	c.m[6] = m->m[2];
	c.m[7] = m->m[5];
	c.m[8] = m->m[8];

	return c;
}

Point AddPointVec(Point * p, Vector * v)
{
	Point p1;
	for (int i = 0; i < 3; i++)
		p1.p[i] = p->p[i] + v->p[i];

	return p1;
}

Vector SubPoint(Point * p1, Point *p2)
{
	Vector v;
	v.p[0] = p1->p[0] - p2->p[0];
	v.p[1] = p1->p[1] - p2->p[1];
	v.p[2] = p1->p[2] - p2->p[2];

	return v;
}

Vector MultMatVec(Matrix * m, Vector * v)
{
	Vector v1;
	v1.p[0] = m->m[0] * v->p[0] + m->m[3] * v->p[1] + m->m[6] * v->p[2];
	v1.p[1] = m->m[1] * v->p[0] + m->m[4] * v->p[1] + m->m[7] * v->p[2];
	v1.p[2] = m->m[2] * v->p[0] + m->m[5] * v->p[1] + m->m[8] * v->p[2];

	return v1;
}

Point MultMatPoint(Matrix * m, Point * p)
{
	Point p1;

	p1.p[0] = m->m[0] * p->p[0] + m->m[3] * p->p[1] + m->m[6] * p->p[2];
	p1.p[1] = m->m[1] * p->p[0] + m->m[4] * p->p[1] + m->m[7] * p->p[2];
	p1.p[2] = m->m[2] * p->p[0] + m->m[5] * p->p[1] + m->m[8] * p->p[2];

	return p1;
}

