#pragma once
#include "point.h"

//line segment, to be more precise
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

	bool isIntersects(Line ln, bool toCheckParallel = true);
	bool havePoint(Point pt);
	bool isOnOneLine(Line ln);
	bool willBeIntersectedBy(Point pt, Point speed);
	bool isPointLeft(Point pt);
	bool isPointRight(Point pt);

	Line operator*(double coeff) {
		double x = coeff * (B.x - A.x), y = coeff * (B.y - A.y); //make vector
		return { A, {B.x + x, B.y + y} };
	}
	Point getVector() {
		return { B.x - A.x, B.y - A.y };
	}
};