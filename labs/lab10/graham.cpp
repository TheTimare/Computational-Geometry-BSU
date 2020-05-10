#include "point.h"
#include "line.h"
#include "geometry.h"

using std::vector;

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