#include "point.h"
#include "line.h"
#include "geometry.h"

using std::vector;

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
	while (points[q_index % n] != points[0] && i < n) {
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
	return { points[0], points[points.size() / 2] };
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