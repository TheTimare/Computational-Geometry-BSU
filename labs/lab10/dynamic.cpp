#include "point.h"
#include "line.h"
#include "geometry.h"

using std::vector;

//qDet > 0 - left, qDet < 0 right, qDet = 0 - on line. First point - position. p1, p2 - make line
int getPointPosToLine(Point p0, Point p1, Point p2) {
	return qDet(p2.x - p1.x, p2.y - p1.y, p0.x - p1.x, p0.y - p1.y);
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

Point findMidPoint(Point p1, Point p2, Point p3) {
	if (Line(p3, p1).getVector() * Line(p3, p2).getVector() < 0) {
		return p3;
	}
	else if (Line(p2, p1).getVector() * Line(p2, p3).getVector() < 0) {
		return p2;
	}
	return p1;
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

		if (getPointPosToLine(hull[hull.size() - 1], hull[0], newPoint) > 0) {
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