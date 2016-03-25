#include <vector>
#include <fstream>

#include <GL/glut.h>

#include <math.h>

#include "FileReader.h"

#pragma comment( lib, "glut32.lib"  )

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
int win_width = 600;
int win_height = 600;
double win_aspect_ratio = ( (double)win_width / (double)win_height );
int time_interval = 1;

//
void loadModel( const std::string& path );
void drawModel();

int modelStyle;

const double PI = 3.141593;
double cam_dist = 5.0;
double cam_theta = 0.0;
double cam_phi = 90;

std::vector<std::vector<float>> vertexes;
std::vector<std::vector<int>> triangles;

float tx;
float ty;
float tz;
float rotate_y_axis;
//
void main(int argc, char **argv) 
{ 
	glutInit( &argc, argv );
	glutInitWindowSize( win_width, win_height );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "Visual 3D" );

	// call-back initialization
	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( special );
	
	initialize();
	glutMainLoop();
} 

void initialize()
{
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	loadModel( "bunny_origin.txt" );

	modelStyle = 0;
	tx = 0.0;
	ty = 0.0;
	tz = 0.0;
	rotate_y_axis = 0;

	cam_dist = 5.0;
	cam_theta = 0;
	cam_phi = 90;
}

void setupProjection()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

	gluPerspective( 45.0, win_aspect_ratio, 0.1, 100.0 );
}

void setupViewing()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	float theta_radian = cam_theta * PI / 180.0;
	float phi_radian = cam_phi * PI / 180.0;

	double cam_x = cam_dist * sin(phi_radian) * sin(theta_radian);
	double cam_y = cam_dist * cos(phi_radian);
	double cam_z = cam_dist * sin(phi_radian) * cos(theta_radian);

	gluLookAt( cam_x, cam_y, cam_z, 0, 0, 0, 0, 1, 0 );
}

void loadModel( const std::string& path )
{
	// you should fill this function to load the triangle list from the model file
	FileReader* fReader = new FileReader(path);
	fReader->readText(vertexes,triangles);
}

void drawModel()
{
	// you should replace the following with the code of drawing the loaded triangle list

	glBegin(GL_TRIANGLES);
	for (int i = 0;i < triangles.size();i++) {
		std::vector<int> tripoints = triangles[i];
		glVertex3f(vertexes[tripoints[0]][0], vertexes[tripoints[0]][1], vertexes[tripoints[0]][2]);
		glVertex3f(vertexes[tripoints[1]][0], vertexes[tripoints[1]][1], vertexes[tripoints[1]][2]);
		glVertex3f(vertexes[tripoints[2]][0], vertexes[tripoints[2]][1], vertexes[tripoints[2]][2]);
	}
	glEnd();
}

void drawAxes()
{
	glBegin( GL_LINES );

	glColor3f( 1, 0, 0 );
	glVertex3d( 0, 0, 0 );
	glVertex3d( 10, 0, 0 );

	glColor3f( 0, 1, 0 );
	glVertex3d( 0, 0, 0 );
	glVertex3d( 0, 10, 0 );

	glColor3f( 0, 0, 1 );
	glVertex3d( 0, 0, 0 );
	glVertex3d( 0, 0, 10 );

	glEnd();
}

void display()
{
	glClearColor( 1, 1, 1, 1 );
	glClearDepth( 1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// view transform
	setupProjection();
	setupViewing();

	// object transform
	glMatrixMode( GL_MODELVIEW );

	// (1) draw axes
	drawAxes();
	
	

	// (2) draw model with 'fill + line' style
	//  - you should modify the following code 
	//  - to draw model with 'point', 'line', or 'fill + line' style conditionally,
	//  - as well as to transform the model according to the current translation and rotation parameters
	glPushMatrix();


	//Bunny transformation
	glTranslatef(tx, ty, tz);
	glRotatef(rotate_y_axis, 0, 1, 0);

	if (modelStyle == 0) {
		//points
		glPolygonMode(GL_FRONT, GL_POINT);
		glColor3f(0.5, 0.5, 0.5);
		glPointSize(2);
		drawModel();
	}
	else if (modelStyle == 1) {
		//line
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(0, 0, 0);
		drawModel();
	}
	else if (modelStyle == 2) {
		// - 1st pass: fill
		glPolygonOffset(1, 1);
		glEnable(GL_POLYGON_OFFSET_FILL);

		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(0.5, 0.5, 0.5);
		drawModel();

		glDisable(GL_POLYGON_OFFSET_FILL);

		// - 2nd pass: line
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(0, 0, 0);
		drawModel();
	}

	glPopMatrix();

	glutSwapBuffers();
}

void reshape( int w, int h )
{
	win_width = w;
	win_height = h;
	win_aspect_ratio = (double)h / (double)w;

	glViewport( 0, 0, win_width, win_height );
}

void keyboard( unsigned char key, int x, int y )
{
	// you should fill this function to respond to various key inputs (x, y, z, r, d, s, ...)
	switch( key )
	{
	case 'S':
		modelStyle += 1;
		if (modelStyle > 2)
			modelStyle = 0;
		break;
	case 's':
		modelStyle -= 1;
		if (modelStyle < 0)
			modelStyle = 2;
		break;
	case 'x':
		tx -= 0.1;
		break;
	case 'X':
		tx += 0.1;
		break;
	case 'y':
		ty -= 0.1;
		break;
	case 'Y':
		ty += 0.1;
		break;
	case 'z':
		tz -= 0.1;
		break;
	case 'Z':
		tz += 0.1;
		break;
	case 'R':
	{	
		if (rotate_y_axis > 360) {
			rotate_y_axis = 0;
		}
		rotate_y_axis += 1.0;
		break;
	}
	case 'r':
	{	
		if (rotate_y_axis < 0) {
			rotate_y_axis = 360;
		}
		rotate_y_axis -= 1.0;
		break;
	}
	case 'D':
		if (cam_dist >= 10) {
			cam_dist = 10;
		}
		else {
			cam_dist += 0.1;
		}
		break;
	case 'd':
		if (cam_dist <= 0) {
			cam_dist = 0;
		}
		else {
			cam_dist -= 0.1;
		}
		break;
	}
	glutPostRedisplay();
}

void special( int key, int x, int y )
{
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (cam_theta >= 360) {
			cam_theta = 0;
		}
		else {
			cam_theta += 1;
		}
		break;
	case GLUT_KEY_LEFT:
		if (cam_theta <= 0 ) {
			cam_theta = 360;
		}
		else {
			cam_theta -= 1;
		}
		break;

	case GLUT_KEY_DOWN:
		if (cam_phi >= 150) {
			cam_phi = 150;
		}
		else {
			cam_phi += 1;
		}
		std::cout << cam_phi << std::endl;
		break;

	case GLUT_KEY_UP:
		if (cam_phi <= 30) {
			cam_phi = 30;
		}
		else {
			cam_phi -= 1;
		}
		std::cout << cam_phi << std::endl;
		break;
	}
	glutPostRedisplay();
}
