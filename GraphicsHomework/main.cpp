#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include <vector>

#include "Polygon.h"
#include "Point.h"
#include "Line.h"
#include "Rectangle.h"
#include "PolyLine.h"
#include "Circle.h"



#pragma comment( lib, "glut32.lib"  )


#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

using namespace std;

// GLUT callback functions
void reshape(int w, int h);
void display();
void idle();
void timer(int timer_id);

void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

void menu_main(int option);
void menu_mode(int option);
void menu_color(int option);

void setupRenderStates();
void setupUICallbacks();
void drawScene();


///새로 추가한 기능. 
void renderClear();			//전체적으로 삭제.
void undo();					//되돌리기 기능
void redo();					//되돌리기의 반대 다시 실행 기능


// UI information
unsigned int win_width = 800;
unsigned int win_height = 600;
float win_aspect_ratio = ((float)win_width / (float)win_height);
unsigned int time_interval = 1;
bool is_lbutton_down = false;

// Scene information
// - currently, only a single line can be instantiated & rendered






vector<Polygon*> polygons;				//그린 모든 객체를 저장하는 vector
vector<Polygon*> undoPolygons;		//redo기능을 수행하기 위한 vector


static int mode;
static int color;
//
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(win_width, win_height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Interactive Vector Drawing in 2D");

	setupRenderStates();
	setupUICallbacks();

	glutMainLoop();
}

void setupRenderStates()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(3);
}

void setupUICallbacks()
{
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
	glutTimerFunc(time_interval, timer, 1);

	// menu
	int mode_menu = glutCreateMenu(menu_mode);
	glutAddMenuEntry("Pen", 0);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("Rect", 2);
	glutAddMenuEntry("Circle", 3);

	int color_menu = glutCreateMenu(menu_color);
	glutAddMenuEntry("Red", 0);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Black", 3);

	glutCreateMenu(menu_main);
	glutAddSubMenu("Mode", mode_menu);
	glutAddSubMenu("Color", color_menu);
	glutAddMenuEntry("Clear", 3);
	glutAddMenuEntry("Undo", 4);
	glutAddMenuEntry("Redo", 5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void drawScene()
{
	for (int i = 0; i < polygons.size(); i++) //각 도형을 draw하는 부분
	{
		polygons.at(i)->draw();
		
	}
}

void display()
{
	glClearColor(1, 1, 1, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw the scene on the back buffer
	glPushMatrix();

	glColor3f(0, 0, 0);
	drawScene();

	glPopMatrix();

	// Present the scene to the display (frame buffer)
	glutSwapBuffers();

	
}

void reshape(int w, int h)
{
	win_width = w;
	win_height = h;
	win_aspect_ratio = (float)h / (float)w;
	
	//
	glViewport(0, 0, win_width, win_height);

	glMatrixMode(GL_PROJECTION);      // Projection transformation
	glLoadIdentity();
	glOrtho(0, win_width, win_height, 0, -1, 1);
}

void timer(int timer_id)
{
	glutTimerFunc(time_interval, timer, timer_id);
	glutPostRedisplay();
}

void idle()
{
}

void keyboard(unsigned char key, int x, int y)
{

}

void special(int key, int x, int y)
{
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			switch (mode) {
			case 0:				//pen
			{
				PolyLine* polyLine = new PolyLine();
				polyLine->setModeAndColor(mode, color);
				polyLine->setPoint(Point((double)x, (double)y));
				polygons.push_back(polyLine);
				break;
			}
			case 1:				//line
			{
				Line* line = new Line();
				line->setModeAndColor(mode, color);
				line->setStartPoint(Point((double)x, (double)y));
				line->setEndPoint(Point((double)x, (double)y));
				polygons.push_back(line);
				break;
			}
			case 2:				//rectangle
			{
				Rectangle* rectangle= new Rectangle();
				rectangle->setModeAndColor(mode, color);
				rectangle->setLeftTopPoint(Point((double)x, (double)y));
				rectangle->setRightBottomPoint(Point((double)x, (double)y));
				polygons.push_back(rectangle);
				break;
			}
				break;
			case 3:				//circle
			{
				Circle * circle = new Circle();
				circle->setModeAndColor(mode, color);
				circle->setStartPoint(Point((double)x, (double)y));
				circle->setMotionPoint(Point((double)x, (double)y));
				polygons.push_back(circle);
				break;
			}
			}

			is_lbutton_down = true;
		}
		else
		{
			is_lbutton_down = false;
		}
	}
}

void motion(int x, int y)
{
	if (is_lbutton_down)
	{
		polygons.at(polygons.size() - 1)->setMotionPoint(Point((double)x, (double)y));
	}
}

void menu_main(int option)
{
	switch (option) {
	case 3:				//clear
		renderClear();
		break;
	case 4:				//undo
		undo();
		break;
	case 5:				//redo
		redo();
		break;
	}
}

void menu_mode(int option)
{
	switch (option) {
	case 0:					//pen
		mode = 0;
		break;
	case 1:					//line
		mode = 1;
		break;
	case 2:					//rectangle
		mode = 2;
		break;
	case 3:					//circle
		mode = 3;
		break;
	}
}
void menu_color(int option)
{
	switch (option) {
	case 0:					//red
		color = 0;
		break;
	case 1:					//green
		color = 1;
		break;	
	case 2:					//blue
		color = 2;
		break;
	case 3:					//black
		color = 3;
		break;
	}
}

void renderClear()			//화면 전체를 지우는 기능
{
	if (polygons.size() > 0) 
	{
		polygons.clear();
	}
}

void undo()					//undo기능(되돌리기)
{
	if (polygons.size() > 0)
	{
		undoPolygons.push_back(polygons.at(polygons.size() - 1));
		polygons.pop_back();
	}
}

void redo()					//undo한것을 취소하는 기능
{
	if (undoPolygons.size() > 0)
	{
		polygons.push_back(undoPolygons.at(undoPolygons.size() - 1));
		undoPolygons.pop_back();
	}
}