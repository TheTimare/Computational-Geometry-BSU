#include "point.h"
#include "line.h"
#include "geometry.h"

using std::vector;

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