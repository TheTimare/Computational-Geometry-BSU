#include "geometry.h"
#include <cmath>
#include <iostream>

using std::vector;

//| a b |
//| c d | quadratic determinant
double qDet(double a, double b, double c, double d) {
	return a * d - b * c;
}
vector<Line> makePolygonLines(vector<Point> verts) {
	vector<Line> lines(verts.size());
	for (int i = 0; i < lines.size(); i++) {
		if (i == lines.size() - 1)
			lines[i] = Line(verts[i], verts[0]);
		else
			lines[i] = Line(verts[i], verts[i + 1]);
	}
	return lines;
}
bool isSimplePolygon(vector<Point> verts) {
	vector<Line> lines = makePolygonLines(verts);
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
int get_octane(Point p1, Point p2) {
	double x = p2.x - p1.x;
	double y = p2.y - p1.y;

	if (0 <= y && y <= x)
		return 1;
	else if (0 < x && x <= y)
		return 2;
	else if (0 <= -x && -x < y)
		return 3;
	else if (0 < y && y <= -x)
		return 4;
	else if (0 <= -y && -y < -x)
		return 5;
	else if (0 < -x && -x <= -y)
		return 6;
	else if (0 < x && x < -y)
		return 7;
	else if (0 < -y && -y <= -x)
		return 8;
	else
		return 1;
}
double getMinY(vector<Point> points) {
	double minY = points[0].y;
	for (int i = 1; i < points.size(); i++)
		minY = (points[i].y < minY) ? points[i].y : minY;
	return minY;
}
double getMinX(vector<Point> points) {
	double minX = points[0].x;
	for (int i = 1; i < points.size(); i++)
		minX = (points[i].x < minX) ? points[i].x : minX;
	return minX;
}
void removeRepeatings(vector<Point> &points) {
	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++) {
			if (points[i] == points[j]) {
				points.erase(points.begin() + j);
				j--;
			}
		}
	}
}
double cos(Point vector1, Point vector2) {
	return (vector1 * vector2) / (vector1.getLength() * vector2.getLength());
}
bool isInClockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) < 0;
} //is three points right
bool isInCounterClockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) > 0;
} //is three points left
void stretchAndMove(vector<Point> &points, Point newOrigin, double stretch) {
	for (int i = 0; i < points.size(); i++) {
		points[i].x = stretch * (points[i].x - newOrigin.x);
		points[i].y = stretch * (points[i].y - newOrigin.y);
	}
}

bool Point::isInPolygon(vector<Point> verts) {
	vector<Line> lines = makePolygonLines(verts);
	Point pt = *this;
	double x_max = verts[0].x, y_max = verts[0].y
		, x_min = verts[0].x, y_min = verts[0].y
		, x_max_ind  = 0;
	for (int i = 1; i < verts.size(); i++) { //нахождение пределов фигуры
		if (verts[i].x > x_max) {
			x_max = verts[i].x;
			x_max_ind;
		}
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

	if (lines[x_max_ind].isPointRight(pt)) {
		return false;
	}
	x_max_ind = (x_max_ind == 0) ? lines.size() - 1 : x_max_ind - 1; //нахождение предыдущей стороны
	if (lines[x_max_ind].isPointRight(pt)) {
		return false;
	}

	for (int i = 0; i < lines.size(); i++) {
		if (lines[i].havePoint(pt)) //если точка находится на границе фигуры
			return true;
	}

	Line checking_ray(pt, {x_max + 1, pt.y});
	int num_of_intersections = 0;
	for (int i = 0; i < lines.size(); i++) {
		if (checking_ray.isIntersects(lines[i], false)) {//если параллельная линия пересекается с одной сторон
			if (checking_ray.havePoint(lines[i].A)) {//проверка на прохождение через общую точку двух сторон
				int previous_ind = (i == 0) ? lines.size() - 1 : i - 1; // предыдущая сторона
				if (lines[i].isOnOneLine(lines[previous_ind])) {
					num_of_intersections--;//не учитываем такой случай
				}
			}
			num_of_intersections++;
		}
	}

	/*cout << "\n\n------\n" "Number of intesections - \"" << num_of_intersections //debug information
		<< "\"\nMax/Min values (xmax, xmin, ymax, ymin): " << x_max << ", "
		<< x_min << ", " << y_max << ", " << y_min << ";\n------\n\n";*/

	if (num_of_intersections % 2 == 0)
		return false;
	return true;
}
bool Point::inMinMaxBorderOfPolygon(vector<Point> lines) {
	double xMin = lines[0].x, xMax = lines[0].x
		, yMin = lines[0].y, yMax = lines[0].y;
	for (int i = 1; i < lines.size(); i++) {
		xMin = (lines[i].x < xMin) ? lines[i].x : xMin;
		xMax = (lines[i].x > xMax) ? lines[i].x : xMax;
		yMin = (lines[i].y < yMin) ? lines[i].y : yMin;

		yMax = (lines[i].y > yMax) ? lines[i].y : yMax;
	}
	if (x < xMin || x > xMax || y < yMin || y > yMax)
		return false;
	return true;
}
//Sector test for convex polygon gicen by clockwise
bool Point::isInSectorOfConvex(vector<Point> verts) {
	vector<Line> lines = makePolygonLines(verts);
	if (lines[0].isPointRight(*this) || lines[lines.size() - 1].isPointRight(*this)) {
		return false;
	}

	int start = 0, end = verts.size() - 1;
	while (end - start > 1) {
		int sep = floor((start + end) / 2);
		Line ln_temp = { verts[0] , verts[sep] };
		if (ln_temp.isPointLeft(*this)) {
			start = sep;
		}
		else {
			end = sep;
		}
	}

	Line ln_temp = { verts[start], verts[start + 1] };
	if (ln_temp.isPointLeft(*this)) {
		return true;
	}
	return false;
}
//Угловой тест
bool Point::isInSimplePolygon(vector<Point> verts) {
	int s = 0;
	for (int i = 0; i < verts.size() - 1; i++) {
		int beta1 = get_octane(*this, verts[i]);
		int beta2 = get_octane(*this, verts[i + 1]);
		int delta = beta2 - beta1;
		if (delta > 4)
			delta -= 8;
		if (delta < -4)
			delta += 8;
		if (delta == 4 || delta == -4) {
			double d = qDet(verts[i].x - x, verts[i].y - y, verts[i + 1].x - x, verts[i + 1].y - y);
			if (d > 0)
				delta = 4;
			if (d < 0)
				delta = -4;
			if (d == 0)
				return true;
		}
		s += delta;
	}
	//std::cout << "s = " << s << "\n\n";
	if (s == 8 || s == -8)
		return true;
	return false;
}
double Point::getLength() {
	return sqrt(*this * *this);
}

//line segment intersection check
bool Line::isIntersects(Line ln, bool toCheckParallel) {
	double x1 = A.x
		, x2 = B.x
		, x3 = ln.A.x
		, x4 = ln.B.x
		, y1 = A.y
		, y2 = B.y
		, y3 = ln.A.y
		, y4 = ln.B.y;
	double d1 = qDet(x4 - x3, y4 - y3, x3 - x1, y3 - y1) //подсчет определителей
		, d2 = qDet(x4 - x3, y4 - y3, x3 - x2, y3 - y2)
		, d3 = qDet(x2 - x1, y2 - y1, x3 - x1, y3 - y1)
		, d4 = qDet(x2 - x1, y2 - y1, x4 - x1, y4 - y1);

	if (d1*d2 < 0 && d3*d4 < 0)
		return true;

	if (toCheckParallel && d1 == d2 && d1 == d3 && d1 == d4 && d1 == 0) { //рассмотрение случая нахождения на одной прямой
		double c1 = (x3 - x1)*(x4 - x1) + (y3 - y1)*(y4 - y1) //скалярные произведения
			, c2 = (x3 - x2)*(x4 - x2) + (y3 - y2)*(y4 - y2)
			, c3 = (x1 - x3)*(x2 - x3) + (y1 - y3)*(y2 - y3)
			, c4 = (x1 - x4)*(x2 - x4) + (y1 - y4)*(y2 - y4);
		if (c1 < 0 || c2 < 0 || c3 < 0 || c4 < 0)
			return true;
	}

	if (!toCheckParallel && (this->havePoint(ln.A) || this->havePoint(ln.B)
		|| ln.havePoint(A) || ln.havePoint(B))) {
		return true;
	}

	return false;
}
//Checks if point lies on line segment
bool Line::havePoint(Point pt) {
	double x1 = A.x, y1 = A.y
		, x2 = B.x, y2 = B.y
		, x_max = (x1 > x2) ? x1 : x2
		, y_max = (y1 > y2) ? y1 : y2
		, x_min = (x1 < x2) ? x1 : x2
		, y_min = (y1 < y2) ? y1 : y2;
	if (pt.x < x_min || pt.x > x_max || pt.y < y_min || pt.y > y_max)
		return false;
	//через общее уравнение прямой Ax + By + C = 0.
	double eA = y2 - y1
		, eB = x1 - x2
		, eC = x2 * y1 - x1 * y2;
	if (eA*pt.x + eB * pt.y + eC == 0)
		return true;
	return false;
}
//Checks if two line segments lies on one line
bool Line::isOnOneLine(Line ln) {
	double x1 = A.x, y1 = A.y
		, x2 = B.x, y2 = B.y;
	//общее уравнение прямой Ax + By + C = 0.
	double eA = y2 - y1
		, eB = x1 - x2
		, eC = x2 * y1 - x1 * y2;
	if (eA * ln.A.x + eB * ln.A.y + eC == 0 && eA * ln.B.x + eB * ln.B.y + eC == 0) {//обе точки второго отрезка
		return true;
	}
	return false;
}
//checks if point intersect line segment with current speed on the next tick
bool Line::willBeIntersectedBy(Point pt, Point speed) {
	double det, detNew, x = pt.x, y = pt.y; 
	double x1 = A.x, x2 = B.x, y1 = A.y, y2 = B.y; 
	det = qDet(x2 - x1, y2 - y1, x - x1, y - y1);    //if (det <= 0) точка "ПРАВЕЕ" или "НА"
	detNew = qDet(x2 - x1, y2 - y1, x + speed.x - x1, y + speed.x - y1); //if (det >= 0) точка "ЛЕВЕЕ" или "НА"
	if (det >= 0 && detNew <= 0 || det <= 0 && detNew >= 0)
		return true;
	return false;
}
bool Line::isPointLeft(Point pt) {
	if(qDet(B.x - A.x, B.y - A.y, pt.x - A.x, pt.y - A.y) < 0) 
		return true;
	return false;
}
bool Line::isPointRight(Point pt) {
	if (qDet(B.x - A.x, B.y - A.y, pt.x - A.x, pt.y - A.y) > 0)
		return true;
	return false;
}