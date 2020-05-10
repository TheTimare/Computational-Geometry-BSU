#include "point.h"
#include "line.h"
#include "geometry.h"

using std::vector;

vector<Point> getLeftPoints(vector<Point> points, Point p1, Point p2) {
	vector<Point> leftPoints;
	for (int i = 0; i < points.size(); i++) {
		if (Line(p1, p2).isPointLeft(points[i]))
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
		if (triangleArea(left, right, points[i]) > maxAreaTriangle) {
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