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

//int timeOfStart = time(0);
const int windowWidth = 700;
const int windowHeight = 700;
const int msPerFrame = 16.7; //50 for 20fps, 33 for ~30fps, 16.7 for ~60fps
const int relativityCoefficient = 350; /*standard is 300. Determines what distance(~speed, at this case)
									   point will pass with the unit of time 
									   (for 3rd lab result depends only on the time, not on fps)*/

//Global variables to transfering data to draw fucntion(glut opengl specifics)
vector<Point> points;
vector<Point> convexHull;
vector<Point> up;
vector<Point> down;

double xy_len;

bool compare(Point a, Point b) {
	return a.x < b.x || a.x == b.x && a.y < b.y;
} //function to sort the points array
bool clockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) < 0;
} //is three points right
bool counterClockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) > 0;
} //is three points left

/*Используется метод Грэхэма (Graham) (предложен в 1972 г.) с улучшениями Эндрю (Andrew) (1979 г.).
С его помощью можно построить выпуклую оболочку за время O(n*log(n)) с использованием
только операций сравнения, сложения и умножения. Алгоритм является асимптотически 
оптимальным (доказано, что не существует алгоритма с лучшей асимптотикой), хотя в 
некоторых задачах он неприемлем (в случае параллельной обработки или при online-обработке).*/
//Main build function of lab 4
void BuildPolygonFunction(int i) {
	//stop of cycle and end message
	if (i == points.size()) {
		glutLeaveMainLoop();
		std::string str = "Convex polygon has been built.\n";
		str += "End of execution.\n";
		MessageBox(NULL,
			str.c_str(),
			"Informing",
			MB_ICONASTERISK | MB_OK | MB_DEFBUTTON2);
		return;
	}

	Point p1 = points[0], p2 = points.back();
	if (i == points.size() - 1 || clockWise(p1, points[i], p2)) { //making upper part of shell
		while (up.size() >= 2 && !clockWise(up[up.size() - 2], up[up.size() - 1], points[i]))
			up.pop_back();
		up.push_back(points[i]);
	}
	if (i == points.size() - 1 || counterClockWise(p1, points[i], p2)) { //making lower part of shell
		while (down.size() >= 2 && !counterClockWise(down[down.size() - 2], down[down.size() - 1], points[i]))
			down.pop_back();
		down.push_back(points[i]);
	}

	//make hull to draw
	convexHull.clear();
	for (int i = 0; i < up.size(); i++)
		convexHull.push_back(up[i]);
	for (int i = down.size() - 2; i > 0; i--)
		convexHull.push_back(down[i]);

	glutPostRedisplay(); //drawing frame
	glutTimerFunc(relativityCoefficient, BuildPolygonFunction, ++i); //basis of cycle
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
	glClearColor(1, 1, 1, 0);
	gl_display_coordinates(xy_len); //отображение координат

	glPointSize(6.5); //обработка точек многоугольника
	glBegin(GL_POINTS);
	glColor3d(0.2, 0.2, 0.75);
	for (int i = 0; i < points.size(); i++) 
		glVertex2d(points[i].x, points[i].y);
	glEnd();

	glLineWidth(5/4);
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
	removeRepeatings(points);

	//Finding coordinates limits
	xy_len = 5;
	for (int i = 0; i < points.size(); i++) {
		xy_len = abs(points[i].x) > xy_len ? abs(points[i].x) : xy_len;
		xy_len = abs(points[i].y) > xy_len ? abs(points[i].y) : xy_len;
	}
	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //set coordinates limits x,y,z
	glutDisplayFunc(gl_display);//visual output function
	glutIdleFunc(gl_display);

	sort(points.begin(), points.end(), &compare); //sorting points to work
	up = { points[0] }; //initial point
	down = { points[0] }; //initial point
	glutTimerFunc(relativityCoefficient, BuildPolygonFunction, 1);  //timer startup(it is recursive)

	glutMainLoop(); //end of window description
}
