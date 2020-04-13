#pragma once
#include <vector>

struct Point {
	double x = 0;
	double y = 0;

	bool isInPolygon(std::vector<Point> verts);
	bool inMinMaxBorderOfPolygon(std::vector<Point> lines);
	double getLength();

	//this operations acts with points like with vectors
	Point operator+(Point pt) {
		return { x + pt.x, y + pt.y };
	}
	Point operator-(Point pt) {
		return { x - pt.x, y - pt.y };
	}
	double operator*(Point pt) {
		return x * pt.x + y * pt.y;
	}
	Point operator*(double coeff) {
		return { x*coeff, y*coeff };
	}
	bool operator==(Point pt) {
		return x == pt.x && y == pt.y;
	}
	bool operator!=(Point pt) {
		return x != pt.x && y != pt.y;
	}

	bool isInSectorOfConvex(std::vector<Point> verts);
	bool isInSimplePolygon(std::vector<Point> verts);
};