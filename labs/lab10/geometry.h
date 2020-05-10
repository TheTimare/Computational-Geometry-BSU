#pragma once
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

void log(std::string logMessage);

#pragma region utilities

//quadratic determinant
double qDet(double a, double b, double c, double d);

std::vector<Point> generatePoints(int number, int limit);
Point generatePoint(int limit);
std::vector<Point> generateSpeeds(int number);

void removeRepeatings(std::vector<Point> &points);
void removeRepeatingsWithSpeed(std::vector<Point> &points, std::vector<Point> &speeds);

Point getPointMinX(std::vector<Point> points);
Point getPointMaxX(std::vector<Point> points);

bool compareByX(Point a, Point b);
bool compareByY(Point a, Point b);

double triangleArea(Point p1, Point p2, Point p3);

bool isSimplePolygon(std::vector<Point> verts);

std::vector<Line> makePolygonLines(std::vector<Point> verts);

double getMinY(std::vector<Point> points);
double getMinX(std::vector<Point> points);

bool isInClockWise(Point a, Point b, Point c);
bool isInCounterClockWise(Point a, Point b, Point c);

void stretchAndMove(std::vector<Point> &points, Point newOrigin, double stretch);

template <class T>
bool isVectorContain(std::vector<T> vect, T item) {
	for (int i = 0; i < vect.size(); i++) {
		if (vect[i] == item) {
			return true;
		}
	}
	return false;
}

#pragma endregion

#pragma region quickhull

std::vector<Point> buildConvexHullQuick(std::vector<Point> points);

#pragma endregion

#pragma region dynamic

bool isPointInConvexPolygon(Point p0, std::vector<Point> points);

std::vector<Point> buildDynamicConvexHull(std::vector<Point> hull, std::vector<Point> points);

#pragma endregion

#pragma region graham

std::vector<Point> buildConvexHullGraham(std::vector<Point> points);

#pragma endregion

#pragma region divide and conquer

double getClosestPairDistance(std::vector<Point> sortedByX, std::vector<Point> sortedByY);

std::pair<int, int> getClosestPairInds(std::vector<Point> points, double minPairDiameter);

#pragma endregion

#pragma region jarvis

std::vector<Point> buildConvexHullJarvis(std::vector<Point> points);

std::vector<Point> get2PointsDiameter(std::vector<Point> points);
double getDiameter(std::vector<Point> points);

#pragma endregion

#pragma region cirus-beck

Line cutSegmentByPolygon(std::vector<Point> polygon, Line segment);

#pragma endregion