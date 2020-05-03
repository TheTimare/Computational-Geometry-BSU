#include "point.h"
#include "line.h"
#include "geometry.h"

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
//qDet > 0 - left, qDet < 0 right, qDet = 0 - on line. First point - position. p1, p2 - make line
int getPointPosToLine(Point p0, Point p1, Point p2) {
	return qDet(p2.x - p1.x, p2.y - p1.y, p0.x - p1.x, p0.y - p1.y);
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

bool clockwiseLess(Point a, Point b) {
	Point center = { 0, 0 };
	if (a.x - center.x >= 0 && b.x - center.x < 0)
		return true;
	if (a.x - center.x < 0 && b.x - center.x >= 0)
		return false;
	if (a.x - center.x == 0 && b.x - center.x == 0) {
		if (a.y - center.y >= 0 || b.y - center.y >= 0)
			return a.y > b.y;
		return b.y > a.y;
	}

	// compute the cross product of vectors (center -> a) x (center -> b)
	int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
	if (det < 0)
		return true;
	if (det > 0)
		return false;

	// points a and b are on the same line from the center
	// check which point is closer to the center
	int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
	int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
	return d1 > d2;
}

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

//functions to sort the points array;
bool compareByX(Point a, Point b) {
	return a.x < b.x;
}
bool compareByY(Point a, Point b) {
	return a.y < b.y;
}

Point findMidPoint(Point p1, Point p2, Point p3){
	if (Line(p3, p1).getVector() * Line(p3, p2).getVector() < 0) {
		return p3;
	}
	else if (Line(p2, p1).getVector() * Line(p2, p3).getVector() < 0) {
		return p2;
	}
	return p1;
}

// Sector test for convex polygon
bool isPointInConvexPolygon(Point p0, vector<Point> points) {
    if (getPointPosToLine(p0, points[0], points[1]) < 0 
		|| getPointPosToLine(p0, points[0], points[points.size() - 1]) > 0) {
		log("Point is outside 1!");
		return  false;
	}

	int start = 0, 
		end = points.size() - 1;
    while (end - start > 1) {
		int separator = floor((start + end) / 2);
		if (getPointPosToLine(p0, points[0], points[separator]) > 0) {
			start = separator;
		}
		else {
			end = separator;
		}
	}

	if (getPointPosToLine(p0, points[start], points[start + 1]) > 0) {
		log("Point is inside!");
		return true;
	}
	log("Point is outside 2!");
	return false;
}

void recycleToMinX(vector<Point> &points) {
	double minX = getMinX(points);
	while (points[0].x != minX) {
		std::rotate(points.begin(), points.begin() + 1, points.end());
	}
}


//Grahams alghoritm
vector<Point> buildConvexHullGraham(vector<Point> points) {
	sort(points.begin(), points.end(), &compareByX); //sorting points to buildHullFunction
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
// Adds a new point to a given convex hull 
vector<Point> buildDynamicConvexHull(vector<Point> hull, vector<Point> points) {
	Point newPoint = points[points.size() - 1];

	if (hull.size() == 0 || hull.size() == 1) {
		hull.push_back(newPoint);
		return hull;
	}

    if (hull.size() == 2) {
        if (getPointPosToLine(hull[0], hull[1], newPoint) == 0) {
			hull.clear();
			Point midPoint = findMidPoint(points[0], points[1], points[2]);

            if (midPoint == points[0]) {
				hull.push_back(points[1]);
				hull.push_back(points[2]);
			}
            else if (midPoint == points[1]) {
				hull.push_back(points[0]);
				hull.push_back(points[2]);
			}
            else {
				hull.push_back(points[1]);
				hull.push_back(points[2]);
			}
		}
		else if (getPointPosToLine(hull[0], hull[1], newPoint) > 0) {
			hull.push_back(newPoint);
		}
        else {
			hull.clear();
			hull.push_back(points[0]);
			hull.push_back(points[2]);
			hull.push_back(points[1]);
		}
		return hull;
	}
    else {
		vector<Point> newHull;
		int rightPointNum = 0;
		int leftPointNum = 0;
		vector<int> right;

		for (int i = 0; i < hull.size() - 1; i++) {
            if (getPointPosToLine(hull[i], hull[i + 1], newPoint) < 0) {
				right.push_back(i);
				right.push_back(i + 1);
			}
		}
		
        if (getPointPosToLine(hull[hull.size() - 1], hull[0], newPoint) < 0) {
			int i = 0;
			while (getPointPosToLine(hull[i], hull[i + 1], newPoint) < 0) {
				i++;
			}
			rightPointNum = i;

			i = hull.size() - 1;
			while (getPointPosToLine(hull[i - 1], hull[i], newPoint) < 0) {
				i--;
			}
			leftPointNum = i;

            if (!isVectorContain(right, (int)hull.size() - 1)) {
				right.push_back(hull.size() - 1);
			}
			if (!isVectorContain(right, 0)) {
				right.push_back(0);
			}
		}

		if (right.size() == 0) {
			return hull;
		}

        if ( getPointPosToLine(hull[hull.size() - 1], hull[0], newPoint) > 0 ) {
			for (int i = 0; i < right[0] + 1; i++) {
				newHull.push_back(hull[i]);
			}
			newHull.push_back(newPoint);
			for (int i = right[right.size() - 1]; i < hull.size(); i++) {
				newHull.push_back(hull[i]);
			}
		}
        else if (getPointPosToLine(hull[hull.size() - 1], hull[0], newPoint) < 0) {
			newHull.push_back(newPoint);
			for (int i = rightPointNum; i < leftPointNum + 1; i++) {
				newHull.push_back(hull[i]);
			}
		}

		return newHull;
	}
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
			if ((points[p_index % n] - points[q_index % n]).getLength() != (points[q0_index % n] - points[points.size() - 1]).getLength())
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

double minSetDistance(vector<Point> points) {
	double minDistance = Line(points[0], points[1]).getVector().getLength();
    for (int i = 0; i < points.size() - 1; i++) {
		for (int j = i + 1; j < points.size(); j++) {
			double ijDistance = Line(points[i], points[j]).getVector().getLength();
            if (ijDistance <= minDistance) {
				minDistance = ijDistance;
			}
		}
	}
	return minDistance;
}

// divide and conquer algorythm
double getClosestPairDistance(vector<Point> sortedByX, vector<Point> sortedByY) {
    if (sortedByX.size() > 1 && sortedByX.size() <= 3) {
		return minSetDistance(sortedByX);
	}

	int sepIndX = floor(sortedByX.size() / 2);
	Point sepPointX = sortedByX[sepIndX];
	vector<Point> leftSetX, rightSetX;

	for (int i = 0; i < sepIndX; i++) {
		leftSetX.push_back(sortedByX[i]);
	}
	for (int i = sepIndX; i < sortedByX.size(); i++) {
		rightSetX.push_back(sortedByX[i]);
	}

	vector<Point> leftSetY, rightSetY;
	for (int i = 0; i < sortedByY.size(); i++) {
		if (sortedByY[i].x < sepPointX.x) {
			leftSetY.push_back(sortedByY[i]);
		}
		else {
			rightSetY.push_back(sortedByY[i]);
		}
	}

	double minDistanceLeft = getClosestPairDistance(leftSetX, leftSetY);
	double minDistanceRight = getClosestPairDistance(rightSetX, rightSetY);
	double minDistance = (minDistanceLeft < minDistanceRight) ? minDistanceLeft : minDistanceRight;

	vector<Point> deltas;
	for (int i = 0; i < sortedByY.size(); i++) {
		if (abs(sortedByY[i].x - sepPointX.x) <= minDistance) {
			deltas.push_back(sortedByY[i]);
		}
	}

	int maxCheckNum = 7;
	int n = (deltas.size() <= maxCheckNum) ? deltas.size() : maxCheckNum;
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			double ijDistance = Line(deltas[i], deltas[j]).getVector().getLength();
			if (ijDistance < minDistance) {
				minDistance = ijDistance;
			}
		}
	}
	return minDistance;
}

std::pair<int, int> getClosestPairInds(vector<Point> points, double minPairDiameter) {
	for (int i = 0; i < points.size() - 1; i++) {
		for (int j = i + 1; j < points.size(); j++) {
			if (Line(points[i], points[j]).getVector().getLength() == minPairDiameter) {
				return { i, j };
			}
		}
	}
}