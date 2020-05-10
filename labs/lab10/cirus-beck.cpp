#include "point.h"
#include "segment.h"
#include "geometry.h"

using std::vector;

// Function to calculate the max from a vector of floats 
float getMax(vector<float> t){
	float max = INT_MIN;
	for (int i = 0; i < t.size(); i++)
		if (t[i] > max)
			max = t[i];
	return max;
}

// Function to calculate the min from a vector of floats 
float getMin(vector<float> t) {
	float min = INT_MAX;
	for (int i = 0; i < t.size(); i++)
		if (t[i] < min)
			min = t[i];
	return min;
}

// Cyrus Beck algorithm
Line cutSegmentByPolygon(vector<Point> polygon, Line segment) {
	int n = polygon.size();
	vector<Point> normal(polygon.size());

	// Calculating the normals 
	for (int i = 0; i < n; i++) {
		normal[i].y = polygon[(i + 1) % n].x - polygon[i].x;
		normal[i].x = polygon[i].y - polygon[(i + 1) % n].y;
	}

	Point P1_P0 = segment.getVector();

	// Array of all values of Point0 - PointEi 
	vector<Point> P0_PEi(n);

	// Calculating the values of P0 - PEi for all edges 
	for (int i = 0; i < n; i++) {
		P0_PEi[i].x = polygon[i].x - segment.A.x;
		P0_PEi[i].y = polygon[i].y - segment.A.y;
	}

	vector<float> numerator(n);
	vector<float> denominator(n);

	for (int i = 0; i < n; i++) {
		numerator[i] = normal[i] * P0_PEi[i];
		denominator[i] = normal[i] * P1_P0;
	}

	vector<float> t(n);
	vector<float> tE, tL;

	// Calculating 't' and grouping them accordingly 
	for (int i = 0; i < n; i++) {
		t[i] = numerator[i] / denominator[i];
		if (denominator[i] > 0)
			tE.push_back(t[i]);
		else
			tL.push_back(t[i]);
	}

	float temp[2];
	// Taking the max of all 'tE' and 0, so pushing 0 
	tE.push_back(0);
	temp[0] = getMax(tE);

	// Taking the min of all 'tL' and 1, so pushing 1 
	tL.push_back(1);
	temp[1] = getMin(tL);

	// Entering 't' value cannot be 
	// greater than exiting 't' value, 
	// this is the case when the segment 
	// is completely outside 
	Line newSegment;
	if (temp[0] > temp[1]) {
		newSegment.A = { -1, -1 };
		newSegment.B = { -1, -1 };
		return newSegment;
	}

	// Calculating the coordinates in terms of x and y 
	newSegment.A.x = segment.A.x + P1_P0.x * temp[0];
	newSegment.A.y = segment.A.y + P1_P0.y * temp[0];
	newSegment.B.x = segment.A.x + P1_P0.x * temp[1];
	newSegment.B.y = segment.A.y + P1_P0.y * temp[1];

	return newSegment;
}