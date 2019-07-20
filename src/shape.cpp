#include "shape.h"
#include <cmath>

Shape::Shape(std::vector<Vector> _p) : points(_p) {}
Shape::Shape(double x, double y, int sides, double r, double angle) {
	for(int i = 0;i < sides; ++i) {
		Vector pt(x + (r * cos(angle)),y + (r * sin(angle)));
		points.push_back(pt);
		angle += M_PI*2/sides;
	}
}
Shape::~Shape() {};


void Shape::getDimensions() {
	l = INFINITY;r = -INFINITY;t = INFINITY;b = -INFINITY;
	for(auto point : points) {
		if(point.comps[0] < l) {l = point.comps[0];}
		if(point.comps[0] > r) {r = point.comps[0];}
		if(point.comps[1] < t) {t = point.comps[1];}
		if(point.comps[1] > b) {b = point.comps[1];}
	}
	h = b - t;
	w = r - l;
	x = l + w/2;
	y = t + h/2;
}

bool Shape::containsPoint(double x, double y) {
  Vector vec(x,y);
  return containsPoint(vec);
}
bool Shape::containsPoint(Vector point) {
  int wn = 0;
  for(int i = 0;i < points.size();++i) {
    int j;
    i == points.size() - 1 ? j = 0 : j = i+1;
    if(points[i].comps[1] <= point.comps[1]) {
      if(points[j].comps[1] > point.comps[1]) {
        if(((points[j].comps[0] - points[i].comps[0]) * (point.comps[1] - points[i].comps[1]) - (point.comps[0] - points[i].comps[0]) * points[j].comps[1] - points[i].comps[1]) > 0) {
          ++wn;
        }
      }
    }
    else {
      if(points[j].comps[1] <= point.comps[1]) {
        if(((points[j].comps[0] - points[i].comps[0]) * (point.comps[1] - points[i].comps[1]) - (point.comps[0] - points[i].comps[0]) * points[j].comps[1] - points[i].comps[1]) < 0) {
          --wn;
        }
      }
    }
  }
  return wn != 0;
}

void Shape::fill(Argon* argon) {
  for(int y = 0;y < h;++y) {
    for(int x = 0;x < w;++x) {
      if(containsPoint(x,y)) {
        SDL_RenderDrawPoint(argon->ren,x,y);
      }
    }
  }
}
