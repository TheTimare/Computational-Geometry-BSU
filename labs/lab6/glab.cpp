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
const int msPerFrame = 16.7; //50 for 20fps, 33 for ~30fps, 16.7 for ~60fps
const int relativityCoefficient = 300; /*standard is 300. Determines what distance(~speed, at this case)
									   point will pass with the unit of time 
									   (move of points depends only on the time, not on fps)*/

//Global variables to transfering data to draw fucntion(glut opengl specifics)
vector<Point> points;
vector<Point> speeds;
vector<Point> convexHull;
double xy_len;

//Main function of lab 6
void QuickHullCycle(int i) {
	int perimLim = 100;
	convexHull = buildConvexHullQuick(points);

	if (perimOfPolygon(convexHull) >= perimLim)
		for (int i = 0; i < speeds.size(); i++) {
			speeds[i].x = -speeds[i].x;
			speeds[i].y = -speeds[i].y;
		}

	for (int i = 0; i < points.size(); i++) {
		double relativeSpeedX = speeds[i].x * msPerFrame / (relativityCoefficient), //coeff = 300 is standart
			relativeSpeedY = speeds[i].y * msPerFrame / (relativityCoefficient);
		points[i].x += relativeSpeedX;
		points[i].y += relativeSpeedY;
	}

	for (int i = 0; i < points.size(); i++) {
		xy_len = abs(points[i].x) > xy_len ? abs(points[i].x) : xy_len;
		xy_len = abs(points[i].y) > xy_len ? abs(points[i].y) : xy_len;
	}
	glLoadIdentity();

	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //set coordinates limits x,y,z

	glutPostRedisplay(); //drawing frame
	glutTimerFunc(msPerFrame, QuickHullCycle, ++i); //basis of cycle
}

//Coordinates output
void gl_display_coordinates(double xy_len) {
	//создание линий координат
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-xy_len, 0);
	glVertex2f(xy_len, 0);
	glVertex2f(0, -xy_len);
	glVertex2f(0, xy_len);

	//вывод сетки координат
	glColor3f(0.9, 0.9, 0.9);
	for (int i = 1; i < xy_len; i++) {
		glVertex2f(-xy_len, i);
		glVertex2f(xy_len, i);
		glVertex2f(-xy_len, -i);
		glVertex2f(xy_len, -i);

		glVertex2f(i, -xy_len);
		glVertex2f(i, xy_len);
		glVertex2f(-i, -xy_len);
		glVertex2f(-i, xy_len);
	}

	//Рисование черточек координатных прямых
	glColor3f(1, 0.5, 0.5);
	for (int i = 0; i < xy_len; i++) {
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
	gl_display_coordinates(xy_len); //coordinates output

	glPointSize(6.5); //points output
	glBegin(GL_POINTS);
	glColor3d(0.2, 0.2, 0.75);
	for (int i = 0; i < points.size(); i++) 
		glVertex2d(points[i].x, points[i].y);
	glEnd();

	glLineWidth(5/4); //polygon output
	glBegin(GL_LINE_LOOP);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 0.5, 0.5);
	for (int i = 0; i < convexHull.size(); i++) {
		glVertex2d(convexHull[i].x, convexHull[i].y);
	}
	glEnd();
		
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

	points = generatePoints(numberOfPoints, limitOfPointsValue); //points transfer to visual output function
	speeds = generateSpeeds(numberOfPoints);
	//removeRepeatingsWithSpeed(points, speeds);

	//Finding coordinates limits
	xy_len = 5;
	for (int i = 0; i < points.size(); i++) {
		xy_len = abs(points[i].x) > xy_len ? abs(points[i].x) : xy_len;
		xy_len = abs(points[i].y) > xy_len ? abs(points[i].y) : xy_len;
	}
	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //set coordinates limits x,y,z
	glutDisplayFunc(gl_display);//visual output function
	glutIdleFunc(gl_display);

	glutTimerFunc(msPerFrame, QuickHullCycle, 0);  //timer startup(it is recursive)

	glutMainLoop(); //end of window description
}
