#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>

using namespace std;

struct Point {
	int x;
	int y;

  Point() : x(0), y(0) {};
  Point(int _x, int _y) : x(_x), y(_y) {};
  ~Point() {};

	void print() {
		cout << "(" << x << ", " << y << ")" << endl;
	}
};
typedef vector<Point> Points;


class Argon_Triangle {
  public:
    Point a;
    Point b;
    Point c;

    Argon_Triangle() {};
    Argon_Triangle(int x1, int y1, int x2, int y2, int x3, int y3);
    Argon_Triangle(Point _a, Point _b, Point _c);
    //TODO: FOR SOME REASON THIS IS BROKEN
    //Argon_Triangle(Points pts);

    ~Argon_Triangle();
};

#endif
