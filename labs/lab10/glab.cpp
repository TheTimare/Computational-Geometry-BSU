#include "glab.h"
#include <string>
#include <time.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

//full input clear to avoid program stuck
void wrong_input(std::istream &in) {
	in.clear();
	in.ignore(32767, '\n');
	cout << "Некорректный ввод, введите еще раз\n>> ";
}

//int timeOfStart = time(0);
const int windowWidth = 700;
const int windowHeight = 700;
const int msPerFrame = 500; //50 for 20fps, 33 for ~30fps, 16.7 for ~60fps
const int relativityCoefficient = 300; /*standard is 300. Determines what distance(~speed, at this case)
									   point will pass with the unit of time 
									   (move of points depends only on the time, not on fps)*/

//Global variables to transfering data to draw fucntion(glut opengl specifics)
vector<Point> polygon;
vector<Line> segments;

int prevFrameTime = 0;

double xyLen;



//Main function of lab 7
void DynamicHullCycle(int i) {
	if (i >= segments.size()) {
		log("end of execution");
		return;
	}

	log(std::to_string(i));
	
	segments[i] = cutSegmentByPolygon(polygon, segments[i]);

	glutPostRedisplay(); //drawing frame

	prevFrameTime = time(0);
	glutTimerFunc(msPerFrame, DynamicHullCycle, ++i); //basis of cycle
}

//Coordinates output
void gl_display_coordinates(double xyLen) {
	//создание линий координат
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-xyLen, 0);
	glVertex2f(xyLen, 0);
	glVertex2f(0, -xyLen);
	glVertex2f(0, xyLen);

	//вывод сетки координат
	glColor3f(0.9, 0.9, 0.9);
	for (int i = 1; i < xyLen; i++) {
		glVertex2f(-xyLen, i);
		glVertex2f(xyLen, i);
		glVertex2f(-xyLen, -i);
		glVertex2f(xyLen, -i);

		glVertex2f(i, -xyLen);
		glVertex2f(i, xyLen);
		glVertex2f(-i, -xyLen);
		glVertex2f(-i, xyLen);
	}

	//Рисование черточек координатных прямых
	glColor3f(1, 0.5, 0.5);
	for (int i = 0; i < xyLen; i++) {
		glVertex2f(0.1, i);
		glVertex2f(-0.1, i);
		glVertex2f(0.1, -i);
		glVertex2f(-0.1, -i);

		glVertex2f(i, 0.1);
		glVertex2f(i, -0.1);
		glVertex2f(-i, 0.1);
		glVertex2f(-i, -0.1);
	}
	glEnd();
}
//Visual output
void gl_display() {
	glClear(GL_COLOR_BUFFER_BIT); //clear of the screen
	glClearColor(1, 1, 1, 0);
	gl_display_coordinates(xyLen); //coordinates output

	glPointSize(1.5); //points output

	glBegin(GL_LINES);
	glColor3d(0.2, 0.75, 0.2);
	for (int i = 0; i < segments.size(); i++) {
		glVertex2f(segments[i].A.x, segments[i].A.y);
		glVertex2f(segments[i].B.x, segments[i].B.y);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3d(0.2, 0.2, 0.75);
	for (int i = 0; i < polygon.size(); i++) {
		glVertex2f(polygon[i].x, polygon[i].y);
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5 / 3); //polygon output
		
	glutSwapBuffers();
}

void getGlutWindowToTheCenterOfTheScreen(int width, int height) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	//resolution of the screen
	int wRes = desktop.right;
	int hRes = desktop.bottom;

	glutInitWindowPosition(wRes/2 - width/2, hRes/2 - height/2 - 40); //Расположение окна
}
//part of glut initialization
void glut_init_part() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //make use RGB
	glutInitWindowSize(700, 700); //set window size
	getGlutWindowToTheCenterOfTheScreen(700, 700);
	glutCreateWindow("Figures output"); //creating of the window itself
	glClearColor(1, 1, 1, 1);//make the background white
	glMatrixMode(GL_PROJECTION); //set 2d view
	/*glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
		, GLUT_ACTION_GLUTMAINLOOP_RETURNS); //disable program exit after window close
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
		, GLUT_ACTION_CONTINUE_EXECUTION); //disable program exit after window close*/
	glLoadIdentity();
}
//function of opengl work initialization
void glut_init(int argc, char **argv, int numberOfPoints, int limitOfPointsValue) {
	glutInit(&argc, argv); //glut built-in initialization
	glut_init_part();

	segments = { 
		{{-7, -2}, {7, 2}}, {{-4, 4}, {4, -4}}, {{-1, -4}, {5, 2}},
		{{3, -2}, {1, 6}}, {{-3, -4}, {1, 1}}, {{7, 0}, {0, -7}}
	};
	polygon = { {-5,0}, {-3,-3}, {1,-4}, {4,0}, {4,4}, {0,5}, {-2,3} };

	xyLen = 8;
	glOrtho(-xyLen, xyLen, -xyLen, xyLen, -xyLen, xyLen); //set coordinates limits x,y,z
	glutDisplayFunc(gl_display);//visual output function
	//glutIdleFunc(gl_display);

	glutTimerFunc(msPerFrame, DynamicHullCycle, 0);  //timer startup(it is recursive)

	glutMainLoop(); //end of window description
}
