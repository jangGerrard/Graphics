#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <gl/glaux.h>

#include <stdio.h>
#include <math.h>
#include "Math.h"

/*
 * 조교님!! release모드에서는 실행이 안되는데,
 * debug모드에서는 실행이 됩니다.
*/

#define B0(t) ( (1-t) * (1-t) * (1-t) )
#define B1(t) ( 3 * (1-t) * (1-t) * t )
#define B2(t) ( 3 * t * t * (1-t) )
#define B3(t) ( t * t * t )

int win_width = 600;
int win_height = 600;
float fovy = 45.0f;
float zNear = 0.1f;
float zFar = 1000.0f;
double win_aspect_ratio = ((double)win_width / (double)win_height);

struct Object
{
	Point position;
	float radius;
};

Object television;
Object movingObj;
bool isTvOn = false;
bool isLeftMouseDown;

Point controlPoint[4];
float segments = 0.02f;
float bazT = 0.0f;
Point bazP[2];

const double PI = 3.141593;
double cam_dist = 20.0;
double cam_theta = 0.0;
double cam_phi = PI / 2;

double doortheta = 0.0;
double closetDoortheta = 0.0;
static float theta;

//font
static int listID;
static int base;
HDC hDC;
HWND hWnd;

//texture
AUX_RGBImageRec * texRec[20];
GLuint texID[20];

void setupProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, win_aspect_ratio, zNear, zFar);
}
void setupViewing()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double cam_x = cam_dist * sin(cam_phi) * sin(cam_theta);
	double cam_y = cam_dist * cos(cam_phi);
	double cam_z = cam_dist * sin(cam_phi) * cos(cam_theta);
	gluLookAt(cam_x, cam_y, cam_z, 0, 0, 0, 0, 1, 0);
}
void setupLighting()
{
	//distant light
	float ambient0[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	float specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	float position0[] = { -5.0f, 2.0, -5.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);

	//평소 생활 불빛
	float ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
	float specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	float position1[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);

	//자기전에 불빛
	float ambient2[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse2[] = { 0.5f, 0.5f, 0.0, 1.0 };
	float specular2[] = { 1.0, 1.0, 1.0, 1.0 };
	float position2[] = { 3.0f, 3.0, 3.0f, 1.0f };

	glLightfv(GL_LIGHT2, GL_POSITION, position2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
}


void bazierCurve()
{
	bazP[0] = bazP[1];
	bazP[1].p[0] = (controlPoint[0].p[0] * B0(bazT)) + (controlPoint[1].p[0] * B1(bazT)) 
						+ (controlPoint[2].p[0] * B2(bazT)) + (controlPoint[3].p[0] * B3(bazT));
	bazP[1].p[1] = -0.5f;
	bazP[1].p[2] = (controlPoint[0].p[2] * B0(bazT)) + (controlPoint[1].p[2] * B1(bazT)) 
						+ (controlPoint[2].p[2] * B2(bazT)) + (controlPoint[3].p[2] * B3(bazT));


	movingObj.position.p[0] = bazP[1].p[0];
	movingObj.position.p[1] = bazP[1].p[1];
	movingObj.position.p[2] = bazP[1].p[2];

	if (bazT >= 1.0f)
	{
		controlPoint[3].p[0] = controlPoint[0].p[0];
		controlPoint[3].p[1] = controlPoint[0].p[1];
		controlPoint[3].p[2] = controlPoint[0].p[2];

		controlPoint[0].p[0] = bazP[1].p[0];
		controlPoint[0].p[1] = bazP[1].p[1];
		controlPoint[0].p[2] = bazP[1].p[2];

		Point temp;
		temp.p[0] = controlPoint[2].p[0];
		temp.p[1] = controlPoint[2].p[1];
		temp.p[2] = controlPoint[2].p[2];

		controlPoint[2].p[0] = controlPoint[1].p[0];
		controlPoint[2].p[1] = controlPoint[1].p[1];
		controlPoint[2].p[2] = controlPoint[1].p[2];

		controlPoint[1].p[0] = temp.p[0];
		controlPoint[1].p[1] = temp.p[1];
		controlPoint[1].p[2] = temp.p[2];

		bazT = 0.0f;
	}
}


void BuildFont(void)
{
	HFONT font;
	HFONT oldfont;
	base = glGenLists(96);
	font = CreateFontA(-24,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		"Courier New");

	oldfont = (HFONT)SelectObject(hDC, font);
	wglUseFontBitmaps(hDC, 32, 96, base);
	SelectObject(hDC, oldfont);
	DeleteObject(font);
}
void KillFont(GLvoid)
{
	glDeleteLists(base, 96);
}
void glPrint(const char* fmt, ...) {
	char text[256];
	va_list ap;
	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}


void calcRay(int _x, int _y, Point * _rayOri, Vector * _rayDir)
{
	float aspectRatio = win_width / win_height;
	float fovyRadius = fovy * 3.141592f / 180.0f;
	float rayDx = tanf(fovyRadius * 0.5f) * aspectRatio;
	float rayDy = tanf(fovyRadius * 0.5f);

	float windowHalfWidth = (float)win_width * 0.5f;
	float windowHalfHeight = (float)win_height * 0.5f;

	float dx = rayDx * ((float)_x / windowHalfWidth - 1.0f);
	float dy = rayDy * (1.0f - (float)_y / windowHalfHeight);

	Point ori;
	SetPoint(&ori, dx * zNear, dy*zNear, -zNear);
	Point end;
	SetPoint(&end, dx * zFar, dy*zFar, -zFar);
	Vector dir = SubPoint(&end, &ori);
	dir = Normalize(&dir);

	double mv[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);

	Matrix mat;
	SetMatrix(&mat, mv[0], mv[4], mv[8], mv[1], mv[5], mv[9], mv[2], mv[6], mv[10]);

	Vector translVec;
	SetVector(&translVec, mv[12], mv[13], mv[14]);

	Matrix invMat = MatrixTranspose(&mat);
	Vector invTranslVec = InverseVector(&translVec);

	ori = AddPointVec(&ori, &invTranslVec);
	ori = MultMatPoint(&invMat, &ori);

	end = AddPointVec(&end, &invTranslVec);
	end = MultMatPoint(&invMat, &end);

	dir = MultMatVec(&invMat, &dir);

	*(_rayOri) = ori;
	*(_rayDir) = dir;

}
bool interSection(Point & _rayOri, Vector & _rayDir, Point & _objPos, float _radius, float * t)
{
	Vector vo = SubPoint(&_rayOri, &_objPos);

	float a = (float)DotProduct(&_rayDir, &_rayDir);
	float b = (float)(2.0f * DotProduct(&_rayDir, &vo));
	float c = (float)DotProduct(&vo, &vo) - (float)(_radius *_radius);
	float disc = b * b - 4.0f * a * c;

	if (disc < 0)
		return false;

	float distSqrt = sqrtf(disc);
	float q;

	if (b < 0)
		q = (-b - distSqrt) / 2.0f;
	else
		q = (-b + distSqrt) / 2.0f;

	float t0 = q / a;
	float t1 = c / q;


	std::cout << "vector obj : " << vo.p[0] << ", " << vo.p[1] << ", " << vo.p[2] << std::endl;


	if (t0 < t1)
	{
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	if (t1 < 0)
	{
		return false;
	}

	if (t0 < 0)
	{
		*t = (float)t1;
		return true;
	}
	else {
		*t = (float)t0;
		return true;
	}
}
void pickingObject(int _x, int _y)
{
	Point rayOri;
	Vector rayDir;

	calcRay(_x, _y, &rayOri, &rayDir);

	float t;
	bool isCollision = interSection(rayOri, rayDir, television.position, television.radius, &t);

	if (isCollision)
	{
		std::cout << "telePicked" << std::endl;
		if (isTvOn) isTvOn = false;
		else isTvOn = true;
	}
}



void DrawQuad(float a[], float  b[], float  c[], float  d[], float norm[], int n)
{
	glBegin(GL_QUADS);
	{
		glNormal3fv(norm);
		glTexCoord2f(0, 0);
		glVertex3fv(a);

		glTexCoord2f(n, 0);
		glVertex3fv(b);

		glTexCoord2f(n, n);
		glVertex3fv(c);

		glTexCoord2f(0, n);
		glVertex3fv(d);
	}
	glEnd();
}
void drawFloor()
{
	double width = 10, dx = 0.1, x = 0, u = 0, du = 0.01 * 1;
	double height = 10, dz = 0.1, z = 0, v = 0, dv = 0.01 * 1;
	glBindTexture(GL_TEXTURE_2D, texID[2]);
	for (int i = 100; i != 0; i--)
	{
		x = 0;
		u = 0;
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0, 1, 0);

		for (int j = 100; j != 0; j--)
		{
			glTexCoord2d(u, v);
			glVertex3f(x - width / 2, 0, z - height / 2);

			glTexCoord2d(u, v + dv);
			glVertex3f(x - width / 2, 0, (z + dx) - height / 2);

			x += dx;
			u += du;
		}

		glEnd();
		z += dz;
		v += dv;
	}
}
void drawDoor()
{
	float a[3] = { -0.1f, 1.0f, 1.0f };//
	float b[3] = { 0.1f, 1.0f, 1.0f };
	float c[3] = { -0.1f, -1.0f, 1.0f };//
	float d[3] = { 0.1f, -1.0f, 1.0f };
	float e[3] = { -0.1f, 1.0f, -1.0f };//
	float f[3] = { 0.1f, 1.0f, -1.0f };
	float g[3] = { 0.1f, -1.0f, -1.0f };//
	float h[3] = { -0.1f, -1.0f, -1.0f };

	float norm[3];

	glTranslatef(0.1f, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, texID[4]);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f;
	DrawQuad(a, c, d, b, norm, 1);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = -1.0f;
	DrawQuad(e, f, g, h, norm, 1);
	norm[0] = 0.0f; norm[1] = 1.0f; norm[2] = 0.0f;
	DrawQuad(a, b, f, e, norm, 1);
	norm[0] = 0.0f; norm[1] = -1.0f; norm[2] = 0.0f;
	DrawQuad(d, c, h, g, norm, 1);

	glBindTexture(GL_TEXTURE_2D, texID[3]);
	norm[0] = -1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(a, e, h, c, norm, 1);
	norm[0] = 1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(b, f, g, d, norm, 1);
}
void drawCylinder()
{
	float a[3] = { -0.25f, 0.5f, 0.25f };
	float b[3] = { 0.25f, 0.5f, 0.25f };
	float c[3] = { -0.25f, -0.5f, 0.25f };
	float d[3] = { 0.25f, -0.5f, 0.25f };
	float e[3] = { -0.25f, 0.5f, -0.25f };
	float f[3] = { 0.25f, 0.5f, -0.25f };
	float g[3] = { 0.25f, -0.5f, -0.25f };
	float h[3] = { -0.25f, -0.5f, -0.25f };

	float norm[3];

	glBindTexture(GL_TEXTURE_2D, texID[4]);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f;
	DrawQuad(a, c, d, b, norm, 1);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = -1.0f;
	DrawQuad(e, f, g, h, norm, 1);
	norm[0] = 0.0f; norm[1] = 1.0f; norm[2] = 0.0f;
	DrawQuad(a, b, f, e, norm, 1);
	norm[0] = 0.0f; norm[1] = -1.0f; norm[2] = 0.0f;
	DrawQuad(d, c, h, g, norm, 1);
	norm[0] = -1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(a, e, h, c, norm, 1);
	norm[0] = 1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(b, f, g, d, norm, 1);
}
void drawTable()
{
	float a[3] = { -1.5f, 0.25f, 1.0f };//
	float b[3] = { 1.5f, 0.25f, 1.0f };
	float c[3] = { -1.5f, -0.25f, 1.0f };//
	float d[3] = { 1.5f, -0.25f, 1.0f };
	float e[3] = { -1.5f, 0.25f, -1.0f };//
	float f[3] = { 1.5f, 0.25f, -1.0f };
	float g[3] = { 1.5f, -0.25f, -1.0f };//
	float h[3] = { -1.5f, -0.25f, -1.0f };

	float norm[3];

	glBindTexture(GL_TEXTURE_2D, texID[4]);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f;
	DrawQuad(a, c, d, b, norm, 1);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = -1.0f;
	DrawQuad(e, f, g, h, norm, 1);
	norm[0] = 0.0f; norm[1] = 1.0f; norm[2] = 0.0f;
	DrawQuad(a, b, f, e, norm, 1);
	norm[0] = 0.0f; norm[1] = -1.0f; norm[2] = 0.0f;
	DrawQuad(d, c, h, g, norm, 1);
	norm[0] = -1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(a, e, h, c, norm, 1);
	norm[0] = 1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(b, f, g, d, norm, 1);


	//아랫부분 받침대 4개의 막대기
	glPushMatrix();
	glTranslatef(-1.25f, -0.5f, 0.75f);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.25f, -0.5f, 0.75f);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.25f, -0.5f, -0.75f);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.25f, -0.5f, -0.75f);
	drawCylinder();
	glPopMatrix();
}
void drawTelevision()
{
	float a[3] = { -0.75f, 0.5f, 0.1f };//
	float b[3] = { 0.75f, 0.5f, 0.1f };
	float c[3] = { -0.75f, -0.5f, 0.1f };//
	float d[3] = { 0.75f, -0.5f, 0.1f };
	float e[3] = { -0.75f, 0.5f, -0.1f };//
	float f[3] = { 0.75f, 0.5f, -0.1f };
	float g[3] = { 0.75f, -0.5f, -0.1f };//
	float h[3] = { -0.75f, -0.5f, -0.1f };

	float norm[3];

	glBindTexture(GL_TEXTURE_2D, texID[5]);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = -1.0f;
	DrawQuad(e, f, g, h, norm, 1);
	norm[0] = 0.0f; norm[1] = 1.0f; norm[2] = 0.0f;
	DrawQuad(a, b, f, e, norm, 1);
	norm[0] = 0.0f; norm[1] = -1.0f; norm[2] = 0.0f;
	DrawQuad(d, c, h, g, norm, 1);
	norm[0] = -1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(a, e, h, c, norm, 1);
	norm[0] = 1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(b, f, g, d, norm, 1);
	//tv picked
	if (isTvOn) {
		glBindTexture(GL_TEXTURE_2D, texID[6]);
	}
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f;
	DrawQuad(c, d, b, a, norm, 1);
}
void drawCloset()
{
	float a[3] = { -0.75f, 1.0f, 1.0f };//
	float b[3] = { 0.75f, 1.0f, 1.0f };
	float c[3] = { -0.75f, -1.0f, 1.0f };//
	float d[3] = { 0.75f, -1.0f, 1.0f };
	float e[3] = { -0.75f, 1.0f, -1.0f };//
	float f[3] = { 0.75f, 1.0f, -1.0f };
	float g[3] = { 0.75f, -1.0f, -1.0f };//
	float h[3] = { -0.75f, -1.0f, -1.0f };

	float norm[3];

	glBindTexture(GL_TEXTURE_2D, texID[4]);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = -1.0f;
	DrawQuad(e, f, g, h, norm, 1);
	norm[0] = 0.0f; norm[1] = 1.0f; norm[2] = 0.0f;
	DrawQuad(a, b, f, e, norm, 1);
	norm[0] = 0.0f; norm[1] = -1.0f; norm[2] = 0.0f;
	DrawQuad(d, c, h, g, norm, 1);
	norm[0] = -1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(a, e, h, c, norm, 1);
	norm[0] = 1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(b, f, g, d, norm, 1);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f;
	DrawQuad(c, d, b, a, norm, 1);
}
void drawClosetDoor()
{
	float a[3] = { -0.1f, 1.0f, 1.0f };//
	float b[3] = { 0.1f, 1.0f, 1.0f };
	float c[3] = { -0.1f, -1.0f, 1.0f };//
	float d[3] = { 0.1f, -1.0f, 1.0f };
	float e[3] = { -0.1f, 1.0f, -1.0f };//
	float f[3] = { 0.1f, 1.0f, -1.0f };
	float g[3] = { 0.1f, -1.0f, -1.0f };//
	float h[3] = { -0.1f, -1.0f, -1.0f };

	float norm[3];
	glTranslatef(0.1f, 0, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texID[4]);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = -1.0f;
	DrawQuad(e, f, g, h, norm, 1);
	norm[0] = 0.0f; norm[1] = 1.0f; norm[2] = 0.0f;
	DrawQuad(a, b, f, e, norm, 1);
	norm[0] = 0.0f; norm[1] = -1.0f; norm[2] = 0.0f;
	DrawQuad(d, c, h, g, norm, 1);
	norm[0] = -1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(a, e, h, c, norm, 1);
	norm[0] = 1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(b, f, g, d, norm, 1);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f;
	DrawQuad(c, d, b, a, norm, 1);
}
void drawTrashCan()
{
	float r = 0.5f;
	glBindTexture(GL_TEXTURE_2D, texID[1]);
	for (int i = 36; i != 0; i--) //바닥면
	{
		float before = ((i - 1) * 10) * 3.141592f / 180.0f;
		float theta = (i * 10) * 3.141592f / 180.0f;
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0, 1);
		glVertex3f(0, -0.5f, 0);
		glTexCoord2f(1, 1);
		glVertex3f(r * cos(before), -0.5, r * sin(before));
		glTexCoord2f(1, 0);
		glVertex3f(r * cos(theta), -0.5, r * sin(theta));
		glEnd();
	}

	float u = 1.0f;
	float du = 1.0f / 36.0f;
	glBindTexture(GL_TEXTURE_2D, texID[10]);
	for (int i = 36; i != 0; i--) //옆면
	{
		float before = ((i - 1) * 10) * 3.141592f / 180.0f;
		float theta = (i * 10) * 3.141592f / 180.0f;
		glBegin(GL_QUADS);
		{
			Point np1, np2, np3;
			SetPoint(&np1, r * cos(before), 0.5, r * sin(before));
			SetPoint(&np2, r * cos(theta), -0.5, r * sin(theta));
			SetPoint(&np3, r * cos(theta), 0.5, r * sin(theta));
			Vector r1 = SubPoint(&np2, &np1);
			Vector r2 = SubPoint(&np3, &np1);
			Vector result = CrossProduct(&r1, &r2);
			Normalize(&result);

			glNormal3f(result.p[0], result.p[1], result.p[2]);
			glTexCoord2f(u, 1);
			glVertex3f(r * cos(theta), 0.5, r * sin(theta));
			glTexCoord2f(u - du, 1);
			glVertex3f(r * cos(before), 0.5, r * sin(before));
			glTexCoord2f(u - du, 0);
			glVertex3f(r * cos(before), -0.5, r * sin(before));
			glTexCoord2f(u, 0);
			glVertex3f(r * cos(theta), -0.5, r * sin(theta));
			u -= du;
		}
		glEnd();
	}
}
void drawBed()
{
	float a[3] = { -1.5f, 0.5f, 3.5f };//
	float b[3] = { 1.5f, 0.5f, 3.5f };
	float c[3] = { -1.5f, -0.5f, 3.5f };//
	float d[3] = { 1.5f, -0.5f, 3.5f };
	float e[3] = { -1.5f, 0.5f, -3.5f };//
	float f[3] = { 1.5f, 0.5f, -3.5f };
	float g[3] = { 1.5f, -0.5f, -3.5f };//
	float h[3] = { -1.5f, -0.5f, -3.5f };

	float norm[3];
	glTranslatef(0.1f, 0, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texID[4]);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = -1.0f;
	DrawQuad(e, f, g, h, norm, 1);
	norm[0] = 0.0f; norm[1] = -1.0f; norm[2] = 0.0f;
	DrawQuad(d, c, h, g, norm, 1);
	norm[0] = -1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(a, e, h, c, norm, 1);
	norm[0] = 1.0f; norm[1] = 0.0f; norm[2] = 0.0f;
	DrawQuad(b, f, g, d, norm, 1);
	norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f;
	DrawQuad(c, d, b, a, norm, 1);
	//침대 커버texture
	glBindTexture(GL_TEXTURE_2D, texID[7]);
	norm[0] = 0.0f; norm[1] = 1.0f; norm[2] = 0.0f;
	DrawQuad(a, b, f, e, norm, 3);
}
void drawSphere(float radius, int tID)
{
	glBindTexture(GL_TEXTURE_2D, texID[tID]);

	float r = radius;
	float du = 1.0f / 36.0f;
	float dv = 1.0f / 19.0f;
	float u = 1.0f;
	float v = 1.0f;
	for (int theta = 36; theta != 0; theta--)
	{

		glBegin(GL_TRIANGLE_STRIP);
		for (int phi = 18; phi >= 0; phi--)
		{
			float phiRad = (phi * 10.0f) * 3.141592f / 180.0f;
			float thetaRad = (theta * 10.0f) * 3.141592f / 180.0f;
			float dRad = (10.0f * 3.141592f) / 180.0f;
			{
				Vector vector;
				SetVector(&vector, r * sin(phiRad) * sin(thetaRad), r * cos(phiRad), 
					r * sin(phiRad) * cos(thetaRad));
				Normalize(&vector);

				glNormal3f(vector.p[0], vector.p[1], vector.p[2]);
				glTexCoord2f(u, v);
				glVertex3f(r * sin(phiRad) * sin(thetaRad), r * cos(phiRad), r * sin(phiRad) * cos(thetaRad));

				SetVector(&vector, r * sin(phiRad) * sin(thetaRad - dRad), 
					r * cos(phiRad), r * sin(phiRad) * cos(thetaRad - dRad));
				Normalize(&vector);

				glNormal3f(vector.p[0], vector.p[1], vector.p[2]);
				glTexCoord2f(u - du, v);
				glVertex3f(r * sin(phiRad) * sin(thetaRad - dRad), r * cos(phiRad), 
					r * sin(phiRad) * cos(thetaRad - dRad));
			}
			v -= dv;
		}
		glEnd();
		u -= du;
	}

}

void LoadTexture()
{
	texRec[0] = auxDIBImageLoad("image/monalisa.bmp");
	texRec[1] = auxDIBImageLoad("image/silver_texture.bmp");
	texRec[2] = auxDIBImageLoad("image/floor.bmp");
	texRec[3] = auxDIBImageLoad("image/door_texture.bmp");
	texRec[4] = auxDIBImageLoad("image/white_textrue.bmp");
	texRec[5] = auxDIBImageLoad("image/black_texture.bmp");
	texRec[6] = auxDIBImageLoad("image/tv_texture.bmp");
	texRec[7] = auxDIBImageLoad("image/bed_texture.bmp");
	texRec[8] = auxDIBImageLoad("image/soccer_texture.bmp");
	texRec[9] = auxDIBImageLoad("image/globe.bmp");
	texRec[10] = auxDIBImageLoad("image/trashcan_texture.bmp");

	glGenTextures(20, &texID[0]);

	for (register int i = 0; i < 11; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		delete[] texRec[i]->data;
		delete texRec[i];
	}
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupProjection();
	setupViewing();
	setupLighting();


	bazierCurve();

	//바닥면
	glPushMatrix();
	glTranslatef(0, -1, 0);
	drawFloor();
	glPopMatrix();

	//현관문
	glPushMatrix();
	glTranslatef(-5.0f, 0, -5.0f);
	glRotatef(doortheta, 0, 1, 0);
	drawDoor();
	glPopMatrix();

	//책상
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -4.0f);
	drawTable();
	glPopMatrix();

	//TV
	glPushMatrix();
	glTranslatef(0.0f, 0.75f, -4.0f);
	drawTelevision();
	glPopMatrix();

	//옷장
	glPushMatrix();
	glTranslatef(4.25f, 0.0f, -4.0f);
	drawCloset();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.50f, 0, -5.0f);
	glRotatef(closetDoortheta, 0, 1, 0);
	drawClosetDoor();
	glPopMatrix();

	//침대
	glPushMatrix();
	glTranslatef(3.50f, -0.5, 0.5f);
	drawBed();
	glPopMatrix();

	//휴지통
	glPushMatrix();
	glTranslatef(-4.0f, -0.4f, 0.0f);
	glRotatef(theta, 0, 1, 0);
	drawTrashCan();
	glPopMatrix();

	//지구본
	glPushMatrix();
	glTranslatef(3.0f, 0.75f, 0.0f);
	glRotatef(theta, 0, 1, 0);
	glRotatef(180, 1, 0, 0);
	drawSphere(0.75f, 9);
	glPopMatrix();

	//Render함수
	//축구공
	glPushMatrix();
	glTranslatef(movingObj.position.p[0], movingObj.position.p[1], movingObj.position.p[2]);
	drawSphere(0.5f, 8);
	glPopMatrix();

	//text
	glColor3f(1, 0, 0);
	glRasterPos3f(0.0f, 3.0f, 0.0f);
	glPrint("HW4_2010720017");

	glutSwapBuffers();

}
void ChangeSize(int w, int h)
{
	win_width = w;
	win_height = h;
	win_aspect_ratio = (double)w / (double)h;

	glViewport(0, 0, win_width, win_height);
}

void SetupRC()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}
void init()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	hWnd = GetActiveWindow();
	hDC = GetDC(hWnd);

	BuildFont();
	LoadTexture();

	SetPoint(&television.position, 0.0f, 0.75f, -4.0f);
	television.radius = 1.0f;

	SetPoint(&movingObj.position, 0.0f, 0.0f, 0.0f);
	movingObj.radius = 1.0f;

	controlPoint[0].p[0] = -2.0f;	controlPoint[0].p[1] = 2.0f;		controlPoint[0].p[2] = -2.0f;
	controlPoint[1].p[0] = 0.0f;		controlPoint[1].p[1] = 2.0f;    controlPoint[1].p[2] = 0.0f;
	controlPoint[2].p[0] = 2.0f;		controlPoint[2].p[1] = 2.0f;    controlPoint[2].p[2] = 2.0f;
	controlPoint[3].p[0] = -2.0f;	controlPoint[3].p[1] = 2.0f;    controlPoint[3].p[2] = 2.0f;

	bazP[1] = controlPoint[0];
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		pickingObject(x, y);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
	}
	glutPostRedisplay();
}
void timer(int value) {

	if (bazT < 1.0)
	{
		bazT += segments;
	}

	theta += 1.0f;

	glutTimerFunc(1000 / 20, timer, 1);

	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		exit(0);
		break;
	case '1': //백열등
		if (glIsEnabled(GL_LIGHT1)) glDisable(GL_LIGHT1);
		else glEnable(GL_LIGHT1);
		break;
	case '2':  //취침등
		if (glIsEnabled(GL_LIGHT2)) glDisable(GL_LIGHT2);
		else glEnable(GL_LIGHT2);
		break;
	case 'D':
		cam_dist += 0.1;
		if (cam_dist > 20.0)	cam_dist = 20.0;
		break;
	case 'd':
		cam_dist -= 0.1;
		if (cam_dist < 0.0)	cam_dist = 0.0;
		break;
	case 'o':
		if (doortheta >= 90)
			doortheta = 89.0f;
		doortheta += 1.0f;
		break;
	case 'O':
		if (doortheta <= 0)
			doortheta = 1.0f;
		doortheta -= 1.0f;
		break;
	case 'i':
		if (closetDoortheta <= -90.0f)
			closetDoortheta = -89.0f;
		closetDoortheta -= 1.0f;
		break;
	case 'I':
		if (closetDoortheta >= 0)
			closetDoortheta = -1.0f;
		closetDoortheta += 1.0f;
		break;
	}
	glutPostRedisplay();
}
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		cam_theta += PI / 180.0;
		break;

	case GLUT_KEY_LEFT:
		cam_theta -= PI / 180.0;
		break;

	case GLUT_KEY_DOWN:
		cam_phi += PI / 180.0;
		if (cam_phi > PI / 180.0 * 150.0)	cam_phi = PI / 180.0 * 150.0;
		break;

	case GLUT_KEY_UP:
		cam_phi -= PI / 180.0;
		if (cam_phi < PI / 180.0 * 30.0)	cam_phi = PI / 180.0 * 30.0;
		break;
	}
	glutPostRedisplay();
}
void menu_main(int option)
{
	if (option == 0) {
		if (isTvOn) isTvOn = false;
		else isTvOn = true;
	}
	glutPostRedisplay();
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("HW4_2010720017");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);

	int mode_menu = glutCreateMenu(menu_main);
	glutAddMenuEntry("Tv On/Off", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(1000 / 50, timer, 1);
	
	init();

	SetupRC();
	glutMainLoop();
}
