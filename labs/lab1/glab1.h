#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>

#include <gl/GL.h>
#include <gl/freeglut.h>

//квадратный определитель
double q_det(double a, double b, double c, double d);

struct Point {
	double x = 0;
	double y = 0;
};

//отрезок
struct Line {
	Point A;
	Point B;

	Line() { //По умолчанию прямая соответствующая оX
 		A = { 0, 0 };
		B = { 1, 0 };
	}
	Line(const Point &_A, const Point &_B) {
		A = _A;
		B = _B;
	}

	//проверка на пересечение отрезков
	bool isIntersects(Line ln) {
		double x1 = A.x
			, x2 = B.x
			, x3 = ln.A.x
			, x4 = ln.B.x
			, y1 = A.y
			, y2 = B.y
			, y3 = ln.A.y
			, y4 = ln.B.y;
		double d1 = q_det(x4 - x3, y4 - y3, x3 - x1, y3 - y1) //подсчет определителей
			, d2 = q_det(x4 - x3, y4 - y3, x3 - x2, y3 - y2)
			, d3 = q_det(x2 - x1, y2 - y1, x3 - x1, y3 - y1)
			, d4 = q_det(x2 - x1, y2 - y1, x4 - x1, y4 - y1);

		if (d1*d2 < 0 && d3*d4 < 0)
			return true;

		if (d1 == d2 && d1 == d3 && d1 == d4 && d1 == 0) { //рассмотрение случая нахождения на одной прямой
			double c1 = (x3 - x1)*(x4 - x1) + (y3 - y1)*(y4 - y1) //скалярные произведения
				, c2 = (x3 - x2)*(x4 - x2) + (y3 - y2)*(y4 - y2)
				, c3 = (x1 - x3)*(x2 - x3) + (y1 - y3)*(y2 - y3)
				, c4 = (x1 - x4)*(x2 - x4) + (y1 - y4)*(y2 - y4);
			if (c1 < 0 || c2 < 0 || c3 < 0 || c4 < 0)
				return true;
		}

		//общая точка начала
		/*if (x1 == x2 && y1 == y2 || x1 == x3 && y1 == y3 || x1 == x4 && y1 == y4
			|| x2 == x3 && y2 == y3 || x2 == x4 && y2 == y4
			|| x3 == x4 && y3 == y4)
			return true;*/

		return false;
	}
};

void wrong_input(std::istream &in);
void menu_case1(int argc, char **argv);
void menu_case2(int argc, char **argv);
void menu_case3(int argc, char **argv);
void menu_case4(int argc, char **argv);