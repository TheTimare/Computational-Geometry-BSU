#pragma once
#include <vector>

//quadratic determinant
double qDet(double a, double b, double c, double d);

struct Point {
	double x = 0;
	double y = 0;

	bool isInPolygon(std::vector<Point> verts);
	bool inMinMaxBorderOfPolygon(std::vector<Point> lines);
	double getLength();

	//this operations acts with points like with vectors
	Point operator+(Point pt) {
		return { x + pt.x, y + pt.y };
	}
	Point operator-(Point pt) {
		return { x - pt.x, y - pt.y };
	}
	double operator*(Point pt) {
		return x * pt.x + y * pt.y;
	}
	Point operator*(double coeff) {
		return { x*coeff, y*coeff };
	} 
	bool operator==(Point pt) {
		return x == pt.x && y == pt.y;
	}

	bool isInSectorOfConvex(std::vector<Point> verts);
};

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
		double x = coeff*(B.x - A.x), y = coeff*(B.y - A.y); //make vector
		return { A, {B.x + x, B.y + y} };
	}
	Point getVector() {
		return { B.x - A.x, B.y - A.y};
	}
};

bool isSimplePolygon(std::vector<Point> verts);
std::vector<Line> makePolygonLines(std::vector<Point> verts);
double getMinY(std::vector<Point> points);
double getMinX(std::vector<Point> points);
void removeRepeatings(std::vector<Point> &points);