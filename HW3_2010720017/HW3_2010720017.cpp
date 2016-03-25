#include <iostream>
#include <vector>
#include <fstream>

#include "Point.h"
#include "Math.h"
#include <math.h>

#include <GL/glut.h>


#pragma comment( lib, "glut32.lib"  )

using namespace std;

void initialize();
void finalize();

// glut callback functions
void reshape( int w, int h );
void display();
void idle();
void finalize();
void timer( int timer_id );

void keyboard( unsigned char key, int x, int y );
void special( int key, int x, int y );
void mouse( int button, int state, int x, int y );
void motion( int x, int y );

//
int win_width = 800;
int win_height = 800;
double win_aspect_ratio = ( (double)win_width / (double)win_height );
int time_interval = 1;

//
const double PI = 3.141593;
double cam_dist = 20.0;
double cam_theta = 0.0;
double cam_phi = PI/2;

double angle = 0.0;

//
void drawFloor();
void drawCube();
void drawPyramid();
void drawTorus();
void drawCorn();
void drawScene();
void drawTriangle(Point p1, Point p2, Point p3);
void setTorusPoints();
void setCornPoints();
Point torusNormal(int phi, int theta);

///points for torus
vector<vector<Point>> points;
///points for corn
vector<Point> cornPoints;

float* pointToFloatArr(Point p);

Math math;

bool isLightEnabled[5] = { false, false,false,false,false };

//
void main(int argc, char **argv) 
{ 
	glutInit( &argc, argv );
	glutInitWindowSize( win_width, win_height );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "HW3_2010720017" );

	// call-back initialization
	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( special );
	glutTimerFunc( 1, timer, 0 );
	

	initialize();
	setTorusPoints();
	setCornPoints();
	glutMainLoop();
} 

void initialize()
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glShadeModel( GL_SMOOTH );
}

void setupProjection()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

	gluPerspective( 60.0, win_aspect_ratio, 0.1, 100.0 );
}

void setupViewing()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	double cam_x = cam_dist * sin( cam_phi ) * sin( cam_theta );
	double cam_y = cam_dist * cos( cam_phi );
	double cam_z = cam_dist * sin( cam_phi ) * cos( cam_theta );
	std::cout << cam_x << ", " << cam_y << ", " << cam_z << std::endl;
	gluLookAt( cam_x, cam_y, cam_z, 0, 0, 0, 0, 1, 0 );
}

void setupLighting()
{
	//distant light
	float ambient0[] = { 0.1, 0.1, 0.1, 1.0 };
	float diffuse0[] = { 0.1, 0.1, 0.1, 1.0 };
	float specular0[] = { 0.5, 0.5, 0.5, 1.0 };
	float position0[] = { 0.0, 1.0, 0.0, 0.0 };

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient0 );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse0 );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular0 );
	glLightfv( GL_LIGHT0, GL_POSITION, position0 );

	glEnable( GL_LIGHT0 );

	//light1
	float ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse1[] =  { 1.0, 1.0, 1.0, 1.0 };
	float specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	float position1[] = { -5.0, 2.0, -5.0, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);

	//light2
	float ambient2[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse2[] = { 1.0, 1.0, 1.0, 1.0 };
	float specular2[] = { 1.0, 1.0, 1.0, 1.0 };
	float position2[] = { 5.0, 2.0, -5.0, 1.0 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT2, GL_POSITION, position2);

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0);

	//light3
	float ambient3[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse3[] = { 1.0, 1.0, 1.0, 1.0 };
	float specular3[] = { 1.0, 1.0, 1.0, 1.0 };
	float position3[] = { -5.0, 2.0, 5.0, 1.0 };

	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular3);
	glLightfv(GL_LIGHT3, GL_POSITION, position3);

	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.0);

	//light4
	float ambient4[] = { 0.5, 0.5, 0.5, 1.0 };
	float diffuse4[] = { 1.0, 1.0, 1.0, 1.0 };
	float specular4[] = { 1.0, 1.0, 1.0, 1.0 };
	float position4[] = { 5.0, 2.0, 5.0, 1.0 };

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient4);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse4);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specular4);
	glLightfv(GL_LIGHT4, GL_POSITION, position4);

	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.0);

}

void setupMaterial( float r, float g, float b )
{
	float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float diffuse[] = { r, g, b, 1.0 };
	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float shininess = 10.0;

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialf( GL_FRONT, GL_SHININESS, shininess );
}

void drawFloor()
{
	glPushMatrix();
	
	setupMaterial(1, 1, 1);
	for (int i = -10; i < 10; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glNormal3d(0, 1, 0);
		for (int j = 10; j >= -10;j--)
		{
			glVertex3f(i, 0, j);
			glVertex3f(i-1, 0, j);
		}
		glEnd();
	}

	glPopMatrix();
}

void drawCube()
{
	glPushMatrix();
	setupMaterial(1, 0, 0);

	Point p1(-1, 1, -1);	Point p2(1, 1, -1);	Point p3(-1, 1, 1);
	Point p4(1, 1, 1); 	Point p5(-1, -1, -1);	Point p6(1, -1, -1);
	Point p7(-1, -1, 1);	Point p8(1, -1, 1);

	//plane #1
	drawTriangle(p2, p1, p3);	drawTriangle(p4, p2, p3);
	
	//plane #2
	drawTriangle(p5, p6, p7);	drawTriangle(p7, p6, p8);

	//plane #3
	drawTriangle(p1, p2, p5);	drawTriangle(p2, p6, p5);

	//plane #4
	drawTriangle(p1, p5, p3);	drawTriangle(p5, p7, p3);

	//plane #5
	drawTriangle(p2, p4, p8);	drawTriangle(p8, p6, p2);

	//plane #6
	drawTriangle(p3, p7, p4);	drawTriangle(p4, p7, p8);

	glPopMatrix();
}
void drawPyramid()
{
	glPushMatrix();
	setupMaterial(0, 1, 0);

	Point p1(0, 1, 0);
	Point p2(-1, -1, 1);
	Point p3(1, -1, 1);
	Point p4(1, -1, -1);
	Point p5(-1, -1, -1);

	//plane #1
	drawTriangle(p1, p2, p3);

	//plane #2
	drawTriangle(p1, p3, p4);

	//plane #3
	drawTriangle(p1, p4, p5);

	//plane #4
	drawTriangle(p1, p5, p2);

	//plane #5
	drawTriangle(p2, p5, p3);

	//plane #6
	drawTriangle(p5, p4, p3);

	glPopMatrix();
}
void drawTorus()
{
	glPushMatrix();

	setupMaterial(0, 0, 1);

	double R = 1.0;
	double r = 0.5;
	for (int phi = 0; phi < 36; phi++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i <= 36;i++)
		{
			int next_phi = phi + 1;
			int theta = i;

			if (next_phi >= 36 ) {
				next_phi = 0;
			}
			if (theta == 36) {
				theta = 0;
			}

			Point p  = torusNormal(phi, theta);
			glNormal3f(p.x,p.y,p.z);
			glVertex3f(points[phi][theta].x, points[phi][theta].y, points[phi][theta].z);

			p = torusNormal(phi + 1, theta);
			glNormal3f(p.x, p.y, p.z);
			glVertex3f(points[next_phi][theta].x, points[next_phi][theta].y, points[next_phi][theta].z);
		}
		glEnd();
	}

	glPopMatrix();
}
void drawCorn()
{
	glPushMatrix();

	setupMaterial(1, 1, 0);
	Point top(0, 1, 0);
	for (int i = 0; i < 36; i++) {
		int nextIdx = (i + 1) % 36;
		drawTriangle(top, cornPoints[nextIdx], cornPoints[i]);
	}
	Point btmCenter(0, -1, 0);
	for (int i = 0; i < 36; i++) {
		int nextIdx = (i + 1) % 36;
		drawTriangle(btmCenter, cornPoints[i], cornPoints[nextIdx]);
	}
	glPopMatrix();
}

Point torusNormal(int phi, int theta)
{
	phi *= 10;
	theta *= 10;

	double nx = cos(phi*3.141592 / 180.0) * cos(theta*3.141592 / 180.0);
	double ny = sin(theta * 3.141592 / 180.0);
	double nz = sin(phi* 3.141592 / 180.0) * cos(theta*3.141592 / 180.0);

	return Point(nx, ny, nz);
}

void setTorusPoints()
{
	double R = 1.0;
	double r = 0.5;
	for (int phi = 0; phi < 360; phi = phi + 10)
	{
		vector<Point> thetaPoints;
		for (int theta = 0; theta <= 360;theta = theta + 10)
		{
			int next_phi = phi + 10;
			if (next_phi > 360) {
				next_phi - 10;
			}

			double x = (R + r*cos(theta*3.141592 / 180.0))*cos(phi*3.141592 / 180.0);
			double y = r*sin(theta*3.141592 / 180.0);
			double z = (R + r*cos(theta*3.141592 / 180.0))*sin(phi*3.141592 / 180.0);
			thetaPoints.push_back(Point(x, y, z));
		}
		points.push_back(thetaPoints);
	}
}

void setCornPoints() //cone points 생성 및 저장
{
	double r = 1.0;
	for (int theta = 0; theta < 36; theta++)
	{
		double thetaRadian = (theta * 10)*3.141592 / 180.0;

		Point p;
		p.x = r*cos(thetaRadian);
		p.y = -1;
		p.z = r*sin(thetaRadian);

		cornPoints.push_back(p);
	}
}

void drawSphere()
{
	
}

void drawTriangle(Point p1, Point p2, Point p3)
{
	glBegin(GL_TRIANGLES);

	Point temp1 = p1 - p2;
	Point temp2 = p1 - p3;
	Point normal  = math.normalVector(math.crossProduct(temp1, temp2));
	
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3fv(pointToFloatArr(p1));

	temp1 = p2 - p3;
	temp2 = p2 - p1;
	normal = math.normalVector(math.crossProduct(temp1,temp2));
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3fv(pointToFloatArr(p2));

	temp1 = p3 - p1;
	temp2 = p3 - p2;
	normal = math.normalVector(math.crossProduct(temp1, temp2));
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3fv(pointToFloatArr(p3));

	glEnd();
}

void drawScene()
{
	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();

	glPushMatrix(); //floor
		glTranslatef(0, -2, 0);
		drawFloor();
	glPopMatrix();

	//glPushMatrix(); //pyramid
	//	glTranslatef(-5, 0, -5);
	//	glRotatef(angle, 0, 0, 1);
	//	drawPyramid();
	//glPopMatrix();

	//glPushMatrix(); //cube
	//	glTranslatef(5, 0, -5);
	//	glRotatef(angle, 1, 0, 0);
	//	drawCube();
	//glPopMatrix();

	//glPushMatrix(); //torus
	//	glTranslatef(-5, 0, 5);
	//	glRotatef(angle, 0, 0, 1);
	//	drawTorus();
	//glPopMatrix();

	//glPushMatrix();  //cone
	//	glTranslatef(5, 0, 5);
	//	glRotatef(angle, 0, 0, 1);
	//	drawCorn();
	//glPopMatrix();
	
	drawSphere();

	glPopMatrix();
}

void display()
{
	glClearColor( 0.1, 0.1, 0.1, 1 );
	glClearDepth( 1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	setupProjection();
	setupViewing();
	setupLighting();

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	
	drawScene();

	glPopMatrix();

	glutSwapBuffers();
}

void reshape( int w, int h )
{
	win_width = w;
	win_height = h;
	win_aspect_ratio = (double)w / (double)h;

	glViewport( 0, 0, win_width, win_height );
}

void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
	case 'D':
		cam_dist += 0.1;
		if( cam_dist > 10.0 )	cam_dist = 10.0;
		break;

	case 'd':
		cam_dist -= 0.1;
		if( cam_dist < 0.0 )	cam_dist = 0.0;
		break;
	case '1':
		if (!isLightEnabled[1]) {
			glEnable(GL_LIGHT1);
			isLightEnabled[1] = true;
		}
		else {
			glDisable(GL_LIGHT1);
			isLightEnabled[1] = false;
		}
		break;
	case '2':
		if (!isLightEnabled[2]) {
			glEnable(GL_LIGHT2);
			isLightEnabled[2] = true;
		}
		else {
			glDisable(GL_LIGHT2);
			isLightEnabled[2] = false;
		}
		break;
	case '3':
		if (!isLightEnabled[3]) {
			glEnable(GL_LIGHT3);
			isLightEnabled[3] = true;
		}
		else {
			glDisable(GL_LIGHT3);
			isLightEnabled[3] = false;
		}
		break;
	case '4':
		if (!isLightEnabled[4]) {
			glEnable(GL_LIGHT4);
			isLightEnabled[4] = true;
		}
		else {
			glDisable(GL_LIGHT4);
			isLightEnabled[4] = false;
		}
		break;
	}
	glutPostRedisplay();
}

void special( int key, int x, int y )
{
	switch( key ) {
	case GLUT_KEY_RIGHT:
		cam_theta += PI / 180.0;
		break;

	case GLUT_KEY_LEFT:
		cam_theta -= PI / 180.0;
		break;

	case GLUT_KEY_DOWN:
		cam_phi += PI / 180.0;
		if( cam_phi > PI / 180.0 * 150.0 )	cam_phi = PI / 180.0 * 150.0;
		break;

	case GLUT_KEY_UP:
		cam_phi -= PI / 180.0;
		if( cam_phi < PI / 180.0 * 30.0 )	cam_phi = PI / 180.0 * 30.0;
		break;
	}
	glutPostRedisplay();
}

void timer( int timer_id )
{
	angle += 0.5;

	glutPostRedisplay();
	glutTimerFunc( 1, timer, 0 );
}

float* pointToFloatArr(Point p)
{
	float result[3] = { p.x, p.y, p.z };
	return result;
}