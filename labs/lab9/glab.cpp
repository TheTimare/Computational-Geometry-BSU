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
const int relativityCoefficient = 150; /*standard is 300. Determines what distance(~speed, at this case)
									   point will pass with the unit of time 
									   (move of points depends only on the time, not on fps)*/

//Global variables to transfering data to draw fucntion(glut opengl specifics)
vector<Point> points;
vector<Point> speeds;
std::pair<int, int> minDistancePoints;
double radius = 0.7;
int circleSegments = 50; //determines quality of circles
int prevFrameTime = 0;

double xyLen;
double maxLen = 20;



//Main function of lab 7
void DynamicHullCycle(int i) {
	if (prevFrameTime == 0) {
		prevFrameTime = time(0);
	}

	vector<Point> sortedByX;
	vector<Point> sortedByY;
	for (int i = 0; i < points.size(); i++) {
		sortedByX.push_back(points[i]);
		sortedByY.push_back(points[i]);
	}
	sort(sortedByX.begin(), sortedByX.end(), &compareByX);
	sort(sortedByY.begin(), sortedByY.end(), &compareByY);

	double minPairDiameter = getClosestPairDistance(sortedByX, sortedByY);
	minDistancePoints = getClosestPairInds(points, minPairDiameter);

	if (minPairDiameter <= 2 * radius) {
		speeds[minDistancePoints.first].x = -speeds[minDistancePoints.first].x;
		speeds[minDistancePoints.first].y = -speeds[minDistancePoints.first].y;

		speeds[minDistancePoints.second].x = -speeds[minDistancePoints.second].x;
		speeds[minDistancePoints.second].y = -speeds[minDistancePoints.second].y;
	}
	
	for (int i = 0; i < points.size(); i++) {
		double deltaTime = time(0) - prevFrameTime;
		if (deltaTime < msPerFrame) {
			deltaTime = msPerFrame;
		}
		double relativeSpeedX = speeds[i].x * deltaTime / relativityCoefficient,
			relativeSpeedY = speeds[i].y * deltaTime / relativityCoefficient;
		points[i].x += relativeSpeedX;
		points[i].y += relativeSpeedY;
	}
	for (int i = 0; i < points.size(); i++) {
		if (abs(points[i].x) > maxLen) {
			speeds[i].x = -speeds[i].x;
			
		}
		if (abs(points[i].y) > maxLen) {
			speeds[i].y = -speeds[i].y;
		}
	}
	for (int i = 0; i < points.size(); i++) {
		xyLen = abs(points[i].x + radius) > xyLen ? abs(points[i].x + radius) : xyLen;
		xyLen = abs(points[i].y + radius) > xyLen ? abs(points[i].y + radius) : xyLen;
	}
	glLoadIdentity();
	glOrtho(-xyLen, xyLen, -xyLen, xyLen, -xyLen, xyLen); //set coordinates limits x,y,z

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
	glBegin(GL_POINTS);
	glColor3d(0.2, 0.2, 0.75);
	for (int i = 0; i < points.size(); i++) {
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	glBegin(GL_LINES);
	glColor3d(1, 0.2, 0.2);

	int start = minDistancePoints.first
		, end = minDistancePoints.second;

	glVertex2f(points[start].x, points[start].y);
	glVertex2f(points[end].x, points[end].y);

	glEnd();

	for (int i = 0; i < points.size(); i++) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < circleSegments; j++) {
			float theta = 2.0f * 3.1415926f * j / circleSegments;//get the current angle 
			float x = radius * cosf(theta);
			float y = radius * sinf(theta);
			glVertex2f(x + points[i].x, y + points[i].y);//output vertex 
		}
		glEnd();
	}

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

	points = generatePoints(numberOfPoints, limitOfPointsValue);
	speeds = generateSpeeds(numberOfPoints);
	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++) {
			if (points[i].x >= points[j].x - 2*radius && points[i].x <= points[j].x + 2*radius 
				&& points[i].y >= points[j].y - 2*radius && points[i].y <= points[j].y + 2*radius) {
				points.erase(points.begin() + j);
				speeds.erase(speeds.begin() + j);
				j--;
			}
		}
	}

	//Finding coordinates limits
	xyLen = 5;
	for (int i = 0; i < points.size(); i++) {
		xyLen = abs(points[i].x) > xyLen ? abs(points[i].x) : xyLen;
		xyLen = abs(points[i].y) > xyLen ? abs(points[i].y) : xyLen;
	}
	glOrtho(-xyLen, xyLen, -xyLen, xyLen, -xyLen, xyLen); //set coordinates limits x,y,z
	glutDisplayFunc(gl_display);//visual output function
	//glutIdleFunc(gl_display);

	glutTimerFunc(msPerFrame, DynamicHullCycle, 0);  //timer startup(it is recursive)

	glutMainLoop(); //end of window description
}
