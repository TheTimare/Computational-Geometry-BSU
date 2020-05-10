#include "point.h"
#include "line.h"
#include "geometry.h"

using std::vector;

void log(std::string logMessage) {
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int h = newtime.tm_hour, m = newtime.tm_min, s = newtime.tm_sec;
	std::cout << "[debug](" << h << ":" << m << ":" << s << "): " << logMessage.c_str() << "\n";
}

Point getPointMinX(vector<Point> points) {
	Point min_point = points[0];
	double min_x = points[0].x;
	for (int i = 1; i < points.size(); i++) {
		if (min_x > points[i].x) {
			min_point = points[i];
			min_x = points[i].x;
		}
	}
	return min_point;
}
Point getPointMaxX(vector<Point> points) {
	Point max_point = points[0];
	double max_x = points[0].x;
	for (int i = 1; i < points.size(); i++) {
		if (max_x < points[i].x) {
			max_point = points[i];
			max_x = points[i].x;
		}
	}
	return max_point;
}

//is left or right triade
bool isInClockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) < 0;
} //is three points right
bool isInCounterClockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) > 0;
} //is three points left

//| a b |
//| c d | quadratic determinant
double qDet(double a, double b, double c, double d) {
	return a * d - b * c;
}

//generate some points with hundredths below the limit. also generate special "speed"
vector<Point> generatePoints(int number, int limit) {
	vector<Point> gPoints(number);
	for (int i = 0; i < number; i++) {
		gPoints[i].x = rand() % (2 * limit) + rand() % 100 / (double)100;
		if (gPoints[i].x == 0)
			gPoints[i].x += 0.01;
		gPoints[i].y = rand() % (2 * limit) + rand() % 100 / (double)100 - limit;
		if (gPoints[i].y == 0)
			gPoints[i].y += 0.01;
	}
	return gPoints;
}
Point generatePoint(int limit) {
	Point point;
	point.x = rand() % (2 * limit) + rand() % 100 / (double)100 - limit;
	point.x = (point.x == 0) ? 0.01 : point.x;
	point.y = rand() % (2 * limit) + rand() % 100 / (double)100 - limit;
	point.y = (point.y == 0) ? 0.01 : point.y;
	return point;
}
vector<Point> generateSpeeds(int number) {
	vector<Point> gPoints(number);
	for (int i = 0; i < number; i++) {
		gPoints[i].x = (rand() % 200 / (double)100) - 1;
		gPoints[i].y = sqrt(1 - gPoints[i].x * gPoints[i].x); //|v| = 1
		if (rand() % 2 == 0)
			gPoints[i].y *= -1;
	}
	return gPoints;
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
void removeRepeatingsWithSpeed(vector<Point> &points, vector<Point> &speeds) {
	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++) {
			if (points[i] == points[j]) {
				points.erase(points.begin() + j);
				speeds.erase(speeds.begin() + j);
				j--;
			}
		}
	}
}

double triangleArea(Point p1, Point p2, Point p3) {
	Point v1 = p2 - p1,
		v2 = p3 - p2,
		v3 = p3 - p1;
	double s_perim = (v1.getLength() + v2.getLength() + v3.getLength()) / 2;
	return sqrt(abs(s_perim * (s_perim - v1.getLength()) * (s_perim - v2.getLength()) * s_perim - v3.getLength()));
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

//functions to sort the points array;
bool compareByX(Point a, Point b) {
	return a.x < b.x;
}
bool compareByY(Point a, Point b) {
	return a.y < b.y;
}

//findint of cos between two vectors
double cos(Point vector1, Point vector2) {
	return (vector1 * vector2) / (vector1.getLength() * vector2.getLength());
}

//finding of new places of points
void stretchAndMove(vector<Point> &points, Point newOrigin, double stretch) {
	for (int i = 0; i < points.size(); i++) {
		points[i].x = stretch * (points[i].x - newOrigin.x);
		points[i].y = stretch * (points[i].y - newOrigin.y);
	}
}