#include "point.h"
#include "geometry.h"

using std::vector;

//check if point lies in polygon(ray test)
bool Point::isInPolygon(vector<Point> verts) {
	vector<Line> lines = makePolygonLines(verts);
	Point pt = *this;
	double x_max = verts[0].x, y_max = verts[0].y
		, x_min = verts[0].x, y_min = verts[0].y
		, x_max_ind = 0;
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

	Line checking_ray(pt, { x_max + 1, pt.y });
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

//Sector test for convex polygon given by clockwise
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

//Angle test
bool Point::isInSimplePolygon(vector<Point> verts) {
	int s = 0;
	for (int i = 0; i < verts.size() - 1; i++) {
		int beta1 = getOctane(*this, verts[i]);
		int beta2 = getOctane(*this, verts[i + 1]);
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