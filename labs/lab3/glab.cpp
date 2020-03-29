#include "glab.h"
#include <string>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

//full input clear to avoid program stuck
void wrong_input(std::istream &in) {
	in.clear();
	in.ignore(32767, '\n');
	cout << "Некорректный ввод, введите еще раз\n>> ";
}
//generate some points with hundredths below the limit. also generate special "speed"
vector<Point> generatePoints(int number ,int limit, bool isSpeed = false) {
	vector<Point> gPoints(number);
	srand(time(0));
	if (isSpeed)
		for (int i = 0; i < number; i++) {
			gPoints[i].x = (rand()%200 / (double)100) - 1;
			gPoints[i].y = sqrt(1 - gPoints[i].x * gPoints[i].x); //|v| = 1
			if (rand() % 2 == 0)
				gPoints[i].y *= -1;
		}
	else
		for (int i = 0; i < number; i++) {
			gPoints[i].x = rand() % (2*limit) + rand() % 100 / (double)100 - limit;
			if (gPoints[i].x == 0)
				gPoints[i].x += 0.01;
			gPoints[i].y = rand() % (2*limit) + rand() % 100 / (double)100 - limit;
			if (gPoints[i].y == 0)
				gPoints[i].y += 0.01;
		}
	return gPoints;
}

int timeOfStart = time(0);
const int windowWidth = 700;
const int windowHeight = 700;
const int msPerFrame = 16.7; //50 for 20fps, 33 for ~30fps, 16.7 for ~60fps
const int relativityCoefficient = 300; /*standard is 300. Determines what distance(~speed, at this case)
									   point will pass with the unit of time 
									   (for 3rd lab result depends only on the time, not on fps)*/

//Global variables to transfering data to draw fucntion(glut opengl specifics)
vector<Point> points;
vector<Point> speed;
vector<Point> outerPolygon;
vector<Point> innerPolygon;
double xy_len;

//main function of 3rd lab
void movePoints(int value) {
	//stop of cycle and end message
	if (points.size() == 0) {
		std::string str = "There are no points remaining.\nElapsed time - ";
		str += std::to_string(time(0) - timeOfStart);
		str += "sec\nEnd of execution.\n";
		MessageBox(NULL,
			str.c_str(),
			"Informing",
			MB_ICONASTERISK | MB_OK | MB_DEFBUTTON2);
		glutLeaveMainLoop();
		return;
	}

	for (int i = 0; i < points.size(); i++) {
		if (points[i].isInSimplePolygon(innerPolygon)) { //remove point in innerPolygon using angle test
			points.erase(points.begin() + i);
			speed.erase(speed.begin() + i);
			i--;
			continue;
		}

		double relativeSpeedX = speed[i].x * msPerFrame / (relativityCoefficient); //coeff = 300 is standart
		double relativeSpeedY = speed[i].y * msPerFrame / (relativityCoefficient);
		Point movedPoint = { points[i].x + 2 * relativeSpeedX, points[i].y + 2 * relativeSpeedY };

		if (!movedPoint.isInSectorOfConvex(outerPolygon)) { //check if point get out of outer polygon
			Line speedLine = { points[i], movedPoint }; // make vector of moving point
			vector<Line> sides = makePolygonLines(outerPolygon);
			int ind = 0; // intersected side index
			for (int j = 1; j < sides.size(); j++) //search for intersected by point side
				if (sides[j].isIntersects(speedLine)) {
					ind = j;
					break;
				}
			Point a = speedLine.getVector(), b = sides[ind].getVector();
			speed[i] = (b * ((a*b) / (b*b))) * 2 - a; //finding reflected speed vector
			speed[i] = speed[i] * (1 / sqrt(speed[i] * speed[i])); //normalize of this speed
		}
		points[i].x += relativeSpeedX; //move of point 
		points[i].y += relativeSpeedY;
	}

	glutPostRedisplay(); //redraw frame
	glutTimerFunc(msPerFrame, movePoints, 1); //basis of recursive cycle
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
	glClear(GL_COLOR_BUFFER_BIT); //Функция очищения экрана
	//glClearColor(1, 1, 1, 0);
	gl_display_coordinates(xy_len); //отображение координат

	glPointSize(6.5); //обработка точек многоугольника
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++) {
		glColor3d(points[i].x, points[i].y, 0);
		glVertex2d(points[i].x, points[i].y);
	}
	glEnd();

	glLineWidth(5 / 2);
	glBegin(GL_LINE_LOOP);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 0.5, 0.5);
	for (int i = 0; i < outerPolygon.size(); i++) {
		glVertex2d(outerPolygon[i].x, outerPolygon[i].y);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.5, 0, 0.5);
	for (int i = 0; i < innerPolygon.size(); i++) {
		glVertex2d(innerPolygon[i].x, innerPolygon[i].y);
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

	points = generatePoints(numberOfPoints, limitOfPointsValue); //передача точек в функцию вывода
	speed = generatePoints(numberOfPoints, 1, true);
	outerPolygon = { {4, 4}, {3, -3}, {0, -5}, {-3, -3}, {-4, 4} };
	innerPolygon = { {0.7, 0.4}, { 0.9,0.7 }, { 1.2,0.7 }, { 1.1, 0.9 }, { 1.1, 1.1 }, { 0.7,1 }, { 0.8,0.8 }, { 0.6,0.6 }, { 0.7, 0.4 } };
	stretchAndMove(innerPolygon, { 0.8, 0.8 }, 4.5);

	for (int i = 0; i < points.size(); i++) {
		if (!points[i].isInPolygon(outerPolygon)) {
			points.erase(points.begin() + i);
			speed.erase(speed.begin() + i);
			i--;
		}
	}

	//Finding coordinates limits
	xy_len = 5;
	for (int i = 0; i < points.size(); i++) {
		xy_len = abs(points[i].x) > xy_len ? abs(points[i].x) : xy_len;
		xy_len = abs(points[i].y) > xy_len ? abs(points[i].y) : xy_len;
	}
	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //set coordinates limits x,y,z
	glutDisplayFunc(gl_display);//visual output function
	glutIdleFunc(gl_display);
	glutTimerFunc(relativityCoefficient, movePoints, 1);  //timer startup(it is recursive)

	glutMainLoop(); //end of window description
}
