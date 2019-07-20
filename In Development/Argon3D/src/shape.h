#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "matrix.h"
#include <vector>

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

class Shape {
private:
  void getDimensions();    //Calculate x,y,w,h,cx,cy,l,r
public:
  std::vector<Vector> points;
  double t;
  double r;
  double b;
  double l;
  double w;
  double h;
  double x;
  double y;
  Color fillColor = {255,255,255,255};
  Color strokeColor = {255,255,255,255};
  float strokeWidth;
  Shape(std::vector<Vector> _p);
  Shape(double cx, double cy, int sides, double r, double angle = 0);
  ~Shape();
  bool containsPoint(double x, double y);
  bool containsPoint(Vector point);
  void applyTransform(Matrix transform);
  void fill(Argon* argon);
  void stroke();
};

#endif
