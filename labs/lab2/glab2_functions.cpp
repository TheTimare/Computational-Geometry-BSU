#include "glab2.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

//полная очистка ввода для избежания зависания программы
void wrong_input(std::istream &in) {
	in.clear();
	in.ignore(32767, '\n');
	cout << "Некорректный ввод, введите еще раз\n>> ";
}
//| a b |
//| c d | квадратный определитель
double q_det(double a, double b, double c, double d) {
	return a * d - b * c;
}
bool is_simple_polygon(vector<Point> &verts, vector<Line> &lines) {
	lines.resize(verts.size());
	for (int i = 0; i < verts.size() - 1; i++) //создание отрезков между точками
		lines[i] = { verts[i], verts[i + 1] };
	lines[lines.size() - 1] = { verts[verts.size() - 1], verts[0] }; //отрезок между первой и последней точкой

	bool is_intersect;
	for (int i = 0; i < lines.size(); i++) { //проверка пересечения каждой стороны с каждой
		for (int j = i + 1; j < lines.size(); j++) {
			is_intersect = lines[i].isIntersects(lines[j]);
			if (is_intersect) //прерывание цикла при хотя бы одном пересечении
				break;
		}
		if (is_intersect)
			break;
	}

	return is_intersect;
}
bool is_point_in(vector<Point> verts, vector<Line> lines, Point pt) {
	double x_max = verts[0].x, y_max = verts[0].y
		, x_min = verts[0].x, y_min = verts[0].y;
	for (int i = 1; i < verts.size(); i++) { //нахождение пределов фигуры
		if (verts[i].x > x_max)
			x_max = verts[i].x;
		if (verts[i].x < x_min)
			x_min = verts[i].x;
		if (verts[i].y > y_max)
			y_max = verts[i].y;
		if (verts[i].y < y_min)
			y_min = verts[i].y;
	}
	if (pt.x > x_max || pt.x < x_min || pt.y > y_max || pt.y < y_min) { //габаритный тест
		return false;
	}
	for (int i = 0; i < lines.size(); i++) {
		if (lines[i].havePoint(pt)) //если точка находится на границе фигуры
			return true;
	}

	Line checking_ray(pt, {x_max + 1, pt.y});
	int num_of_intersections = 0;
	for (int i = 0; i < lines.size(); i++) {
		if (checking_ray.isIntersects(lines[i], false)) {//если параллельная oX линия пересекается с одной из сторон
			if (checking_ray.havePoint(lines[i].A)) {//проверка на прохождение через общую точку двух сторон
				int previous_ind = (i == 0) ? lines.size() - 1 : i - 1; // предыдущая сторона
				if (lines[i].isOnOneLine(lines[previous_ind])) {
					num_of_intersections--;//не учитываем такой случай
				}
			}
			num_of_intersections++;
		}
	}

	cout << "\n\n------\n" "Number of intesections - \"" << num_of_intersections //debug information. logs for weak
		<< "\"\nMax/Min values (xmax, xmin, ymax, ymin): " << x_max << ", "
		<< x_min << ", " << y_max << ", " << y_min << ";\n------\n\n";

	if (num_of_intersections % 2 == 0)
		return false;
	return true;
}

//Глобальные переменные для передачи данных в функцию отрисовки(специфика glut opengl)
vector<Point> dp_points;
double xy_len;
//Отображение координат
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
//Отображение задачи
void gl_display() {
	glClear(GL_COLOR_BUFFER_BIT); //Функция очищения экрана
	gl_display_coordinates(xy_len); //отображение координат

	glPointSize(10); //обработка точек многоугольника
	glBegin(GL_POINTS);
	glColor3f(0, 1, 0);
	glVertex2d(dp_points[0].x, dp_points[0].y);
	glEnd();

	glPointSize(7); //обработка точек многоугольника
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	for (int i = 1; i < dp_points.size(); i++)
		glVertex2d(dp_points[i].x, dp_points[i].y);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0.7, 0.3);
	for (int i = 1; i < dp_points.size(); i++)
		glVertex2d(dp_points[i].x, dp_points[i].y);
	glEnd();

	glFlush(); //Отрисовывает все
}

//части инициализации glut
void glut_init_part() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //формат цвета RGB
	glutInitWindowSize(700, 700); //Размер окна
	glutInitWindowPosition(25, 50); //Расположение окна
	glutCreateWindow("Figures output"); //Создание окна
	glClearColor(1, 1, 1, 1);//Белый фон экрана
	glMatrixMode(GL_PROJECTION); //Установка двумерного вида
	glLoadIdentity();
}
void glut_init_end_part() {
	//Отключение завершения программы при закрытии окна с фигурой
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
		, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
		, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop(); //Завершение описания окна
}
//Функция инициализации opengl для задачи проверки места точки
void glut_init(int argc, char **argv, vector<Point> verts, Point pt) {
	glutInit(&argc, argv); //инициализация glut
	glut_init_part();

	verts.insert(verts.begin(), pt); //добавление точки в начало массива для обработки в display

	//Поиск пределов координат
	xy_len = 5;
	for (int i = 0; i < verts.size(); i++) {
		xy_len = abs(verts[i].x) > xy_len ? abs(verts[i].x) : xy_len;
		xy_len = abs(verts[i].y) > xy_len ? abs(verts[i].y) : xy_len;
	}

	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //Пределы изменение координат x,y,z
	dp_points = verts; //передача точек в функцию вывода
	glutDisplayFunc(gl_display);//Функция вывода на экран

	glut_init_end_part();
}