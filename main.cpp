#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <float.h>
#include <math.h>

#define INPUT_FILE "punkty.txt"

// Shape names

#define NOTHING "nic"
#define POINT "punkt"
#define SEGMENT "odcinek"
#define TRIANGLE "tr\242jk\245t"
#define RHOMB "romb"
#define QUADRANGLE "czworok\245t"
#define OTHER "inny"

class Point {
public:
	Point() : x(0), y(0) {}
	Point(double x, double y) : x(x), y(y) {}
	double x, y;

	bool operator==(const Point& p) const {
		return x == p.x && y == p.y;
	}

	bool operator!=(const Point& p) const {
		return x != p.x || y != p.y;
	}

	// Needed for map
	bool operator<(const Point& p) const {
		return x < p.x || (x == p.x && y < p.y);
	}

	double distance(const Point& p) const {
		return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}
};

class Segment {
public:
	Segment() : p1(0, 0), p2(0, 0) {}
	Segment(Point p1, Point p2) : p1(p1), p2(p2) {}
	Point p1, p2;

	const static Segment NullSegment;

	bool isPointOnSegment(Point p) const {
		// Interpret p1p2 and p1p as vectors and calculate their cross product
		// If the cross product is 0, the point is on the segment
		double crossProduct = (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x);
		return crossProduct == 0;
	}

	double slope() const {
		double denominator = p1.x - p2.x;
		double numerator = p1.y - p2.y;

		if(denominator == 0) {
			return DBL_MAX;
		}

		return numerator / denominator;
	}

	double length() const {
		return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	}

	Segment operator+(const Segment& segment) const {
		if(slope() != segment.slope()) {
			// If the two segments aren't collinear
			return NullSegment;
		}

		// Check if the two segments are "touching" each other
		if(!isPointOnSegment(segment.p1) && !isPointOnSegment(segment.p2)) {
			// If they are not, return an empty segment
			return NullSegment;
		}

		// Find the two points that are the farthest apart
		double max = 0;
		Point farthestPoint1, farthestPoint2;

		if(p1.distance(segment.p1) > max) {
			max = p1.distance(segment.p1);
			farthestPoint1 = p1;
			farthestPoint2 = segment.p1;
		}

		if(p1.distance(segment.p2) > max) {
			max = p1.distance(segment.p2);
			farthestPoint1 = p1;
			farthestPoint2 = segment.p2;
		}

		if(p2.distance(segment.p1) > max) {
			max = p2.distance(segment.p1);
			farthestPoint1 = p2;
			farthestPoint2 = segment.p1;
		}

		if(p2.distance(segment.p2) > max) {
			max = p2.distance(segment.p2);
			farthestPoint1 = p2;
			farthestPoint2 = segment.p2;
		}

		// Return the segment that contains the two farthest points

		return Segment(farthestPoint1, farthestPoint2);
	}

	bool operator==(const Segment& segment) const {
		return p1 == segment.p1 && p2 == segment.p2;
	}

	bool operator!=(const Segment& segment) const {
		return p1 != segment.p1 || p2 != segment.p2;
	}

	friend std::ostream& operator<<(std::ostream& out, const Segment& segment) {
		out << "{(" << segment.p1.x << ", " << segment.p1.y << "), (" << segment.p2.x << ", " << segment.p2.y << ")}";
		return out;
	}
};

const Segment Segment::NullSegment = Segment(Point(0, 0), Point(0, 0));

int main() {
	std::ifstream file(INPUT_FILE);

	if (!file.is_open()) {
		std::cout << "Nie udalo sie otworzyc pliku" << std::endl;
		return 1;
	}

	// Read the points to form a shape

	int pointCount;

	file >> pointCount;

	if(pointCount == 0) {
		std::cout << NOTHING << std::endl;

		return 0;
	}

	double x, y;

	std::vector<Segment> shape;

	Point current, last;

	file >> x >> y;

	const Point first(x, y);

	current = Point(x, y);

	for (int i = 0; i < pointCount - 1; i++) {
		last = current;

		file >> x >> y;

		current = Point(x, y);

		if(current != last)
			shape.push_back(Segment(last, current));
	}

	// Connect the last point to the first point
	if(current != first)
		shape.push_back(Segment(current, first));

	file.close();

	// Merge all the collinear segments

	std::vector<Segment> merged;

	for (int i = 0; i < shape.size(); i++) {
		bool mergedThisSegment = false;

		for (int j = 0; j < merged.size(); j++) {
			Segment mergedSegment = shape[i] + merged[j];

			if(mergedSegment != Segment::NullSegment) {
				merged[j] = shape[i] + merged[j];
				mergedThisSegment = true;
			}
		}

		if (!mergedThisSegment) {
			merged.push_back(shape[i]);
		}
	}

	#ifdef DEBUG
	for(Segment segment : merged) {
		std::cout << segment << std::endl;
	}
	#endif

	// Check if the shape is a point or a line

	if(merged.size() == 0) {
		std::cout << POINT << std::endl;
		return 0;
	}

	if(merged.size() == 1) {
		std::cout << SEGMENT << std::endl;
		return 0;
	}

	// Check if the shape is a polygon

	// Count all points

	std::map<Point, int> pointCounts;

	for(Segment segment : merged) {
		pointCounts[segment.p1]++;
		pointCounts[segment.p2]++;
	}

	// If there are not exactly two segments for each point, it is not a polygon

	for(std::pair<Point, int> pointCount : pointCounts) {
		if(pointCount.second != 2) {
			std::cout << OTHER << std::endl;
			return 0;
		}
	}

	// Find the shape based on the merged segments

	switch(merged.size()) {
	case 3:
		std::cout << TRIANGLE << std::endl;
		break;

	case 4:
		if(merged[0].length() == merged[1].length() && merged[1].length() == merged[2].length() && merged[2].length() == merged[3].length()) {
			std::cout << RHOMB << std::endl;
		} else {
			std::cout << QUADRANGLE << std::endl;
		}
		break;

	default:
		std::cout << OTHER << std::endl;
	}

	return 0;
}