#include "point.h"
#include "line.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <string>
#include <time.h>
#include <iomanip>

using std::vector;

int previousSecond = 1;
void log(std::string logMessage) {
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	int h = newtime.tm_hour, m = newtime.tm_min, s = newtime.tm_sec;
	//if (s != previousSecond)
		std::cout << "[debug](" << h << ":" << m << ":" << s << "): " << logMessage.c_str() << "\n";
	previousSecond = s;
}

//| a b |
//| c d | quadratic determinant
double qDet(double a, double b, double c, double d) {
	return a * d - b * c;
}

//generate some points with hundredths below the limit. also generate special "speed"
vector<Point> generatePoints(int number, int limit) {
	vector<Point> gPoints(number);
	srand(time(0));
	for (int i = 0; i < number; i++) {
		gPoints[i].x = rand() % (2 * limit) + rand() % 100 / (double)100 - limit;
		if (gPoints[i].x == 0)
			gPoints[i].x += 0.01;
		gPoints[i].y = rand() % (2 * limit) + rand() % 100 / (double)100 - limit;
		if (gPoints[i].y == 0)
			gPoints[i].y += 0.01;
	}
	return gPoints;
}
vector<Point> generateSpeeds(int number) {
	vector<Point> gPoints(number);
	srand(time(0));
	for (int i = 0; i < number; i++) {
		gPoints[i].x = (rand() % 200 / (double)100) - 1;
		gPoints[i].y = sqrt(1 - gPoints[i].x * gPoints[i].x); //|v| = 1
		if (rand() % 2 == 0)
			gPoints[i].y *= -1;
	}
	return gPoints;
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

//findint of cos between two vectors
double cos(Point vector1, Point vector2) {
	return (vector1 * vector2) / (vector1.getLength() * vector2.getLength());
}

//is left or right triade
bool isInClockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) < 0;
} //is three points right
bool isInCounterClockWise(Point a, Point b, Point c) {
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y) > 0;
} //is three points left

//finding of new places of points
void stretchAndMove(vector<Point> &points, Point newOrigin, double stretch) {
	for (int i = 0; i < points.size(); i++) {
		points[i].x = stretch * (points[i].x - newOrigin.x);
		points[i].y = stretch * (points[i].y - newOrigin.y);
	}
}

//additional function of alghoritm 
int getOctane(Point p1, Point p2) {
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

//function to sort the points array for buildConvex;
bool compare(Point a, Point b) {
	return a.x < b.x || a.x == b.x && a.y < b.y;
}
//Grahams alghoritm
vector<Point> buildConvexHullGraham(vector<Point> points) {
	sort(points.begin(), points.end(), &compare); //sorting points to buildHullFunction
	vector<Point> up = { points[0] },
		down = { points[0] };

	for (int i = 0; i < points.size(); i++) {
		Point p1 = points[0], p2 = points.back();
		if (i == points.size() - 1 || isInClockWise(p1, points[i], p2)) { //making upper part of shell
			while (up.size() >= 2 && !isInClockWise(up[up.size() - 2], up[up.size() - 1], points[i]))
				up.pop_back();
			up.push_back(points[i]);
		}
		if (i == points.size() - 1 || isInCounterClockWise(p1, points[i], p2)) { //making lower part of shell
			while (down.size() >= 2 && !isInCounterClockWise(down[down.size() - 2], down[down.size() - 1], points[i]))
				down.pop_back();
			down.push_back(points[i]);
		}
	}

	vector<Point> convexHull;
	for (int i = 0; i < up.size(); i++)
		convexHull.push_back(up[i]);
	for (int i = down.size() - 2; i > 0; i--)
		convexHull.push_back(down[i]);
	return convexHull;
}
///build convex hull(jarvis alghoritm)
vector<Point> buildConvexHullJarvis(vector<Point> points) {
	vector<Point> hull;
	int n = points.size();
	if (n < 3) return hull;

	// Find the leftmost point 
	int l = 0;
	for (int i = 1; i < n; i++)
		if (points[i].x < points[l].x)
			l = i;

	int p = l, q;
	do {
		// Add current point to result 
		hull.push_back(points[p]);
		// Search for a point 'q' such that orientation(p, x, 
		// q) is counterclockwise for all points 'x'
		q = (p + 1) % n;
		for (int i = 0; i < n; i++) {
			if (isInCounterClockWise(points[p], points[i], points[q]))
				q = i;
		}
		p = q;
	} while (p != l); // while not first point

	return hull;
}

double triangleArea(Point p1, Point p2, Point p3) {
	Point v1 = p2 - p1, 
		v2 = p3 - p2,
		v3 = p3 - p1;
	double s_perim = (v1.getLength() + v2.getLength() + v3.getLength()) / 2;
	return sqrt(abs(s_perim * (s_perim - v1.getLength()) * (s_perim - v2.getLength()) * s_perim - v3.getLength()));
}

//diameter of flat set
double getDiameter(vector<Point> points) {
	int p_index = points.size() - 1, q_index = 0;
	int n = points.size() - 1;
	
	while (triangleArea(points[p_index % n], points[(p_index + 1) % n], points[(q_index + 1) % n]) > 
		triangleArea(points[p_index % n], points[(p_index + 1) % n], points[q_index % n]))
		q_index += 1;

	int q0_index = q_index;
	double d = 0;
	int i = 0;
    while (points[q_index % n] != points[0] && i < n){
		p_index += 1;
		i++;
		if (i > 100)
			break;
        while (triangleArea(points[p_index % n], points[(p_index + 1) % n], points[(q_index + 1) % n]) > 
                triangleArea(points[p_index % n], points[(p_index + 1) % n], points[q_index % n])) {
			q_index += 1;
			if ((points[p_index % n] - points[q_index % n]).getLength() != (points[q0_index % n] - points[0]).getLength())
				if ((points[p_index % n] - points[q_index % n]).getLength() >= d)
					d = (points[p_index % n] - points[q_index % n]).getLength();
		}
        if (triangleArea(points[p_index % n], points[(p_index + 1) % n], points[(q_index + 1) % n]) == 
                triangleArea(points[p_index % n], points[(p_index + 1) % n], points[q_index % n])) {
			if ((points[p_index % n] - points[q_index % n]).getLength() != (points[q0_index % n] - points[-1]).getLength())
				if ((points[p_index % n] - points[q_index % n]).getLength() >= d)
					d = (points[p_index % n] - points[q_index % n]).getLength();
		}
	}
	return d;
}
//getting of two points forming diameter
vector<Point> get2PointsDiameter(vector<Point> points) {//return two points!
	for (int i = 0; i < points.size(); i++) {
		for (int j = 0; j < points.size(); j++) {
			if ((points[i] - points[j]).getLength() == getDiameter(points))
				return { points[i], points[j] };
		}
	}
	return { points[0], points[points.size()/2] };
}

Point getPointMinX(vector<Point> points) {
	Point min_point = points[0];
	double min_x = points[0].x;
	for (int i = 1; i < points.size(); i++){
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

vector<Point> getLeftPoints(vector<Point> points, Point p1, Point p2){
	vector<Point> leftPoints;
	for (int i = 0; i < points.size(); i++) {
		if (Line( p1, p2 ).isPointLeft(points[i]))
			leftPoints.push_back(points[i]);
	}
	return leftPoints;
}
vector<Point> getRightPoints(vector<Point> points, Point p1, Point p2) {
	vector<Point> rightPoints;
	for (int i = 0; i < points.size(); i++) {
		if (Line(p1, p2).isPointRight(points[i]))
			rightPoints.push_back(points[i]);
	}
	return rightPoints;
}

// Quick hull algorithm part
void partialQuickHull(Point left, Point right, vector<Point> points, vector<Point> &hull) {
	if (points.size() == 0) return; //if there are no left or right points

	double maxAreaTriangle = triangleArea(left, right, points[0]);
	Point sep = points[0];
	for (int i = 1; i < points.size(); i++) {
        if (triangleArea(left, right, points[i]) > maxAreaTriangle){
			maxAreaTriangle = triangleArea(left, right, points[i]);
			sep = points[i];
		}
	}
	vector<Point> s1 = getLeftPoints(points, left, sep);
	vector<Point> s2 = getLeftPoints(points, sep, right);

	if (s1.size() != 0) {
		partialQuickHull(left, sep, s1, hull);
	}
	hull.push_back(sep);
	if (s2.size() != 0) {
		partialQuickHull(sep, right, s2, hull);
	}
	
}
// Quick hull algorithm
vector<Point> buildConvexHullQuick(vector<Point> points) {
	Point pl = getPointMinX(points),
		pr = getPointMaxX(points);
	vector<Point> hull;
	vector<Point> leftPoints = getLeftPoints(points, pl, pr),
		rightPoints = getRightPoints(points, pl, pr);

	hull.push_back(pl);
	partialQuickHull(pl, pr, leftPoints, hull);
	hull.push_back(pr);
	partialQuickHull(pr, pl, rightPoints, hull);

	return hull;
}

double perimOfPolygon(vector<Point> points) {
	double perimeter = 0;
	for (int i = 0; i < points.size() - 1; i++)
		perimeter += Line(points[i], points[i + 1]).getVector().getLength();
	perimeter += Line(points[points.size() - 1], points[0]).getVector().getLength();
	return perimeter;
}