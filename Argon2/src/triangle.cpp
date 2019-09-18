#include "triangle.h"

Argon_Triangle::Argon_Triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  //sort points before setting?
  a = Point(x1, y1);
  b = Point(x2, y2);
  c = Point(x3, y3);
}
Argon_Triangle::Argon_Triangle(Point _a, Point _b, Point _c) {
  //sort points before setting?
  a = _a;
  b = _b;
  c = _c;
}
//TODO:: BROKEN FOR SOME REASON
// Argon_Triangle::Argon_Triangle(Points pts) {
//   if(pts.size() != 3) {
//     cout << "Error: Triangle constructor not passed exactly 3 points." << endl;
//   }
//   else {
//     //sort points before setting?
//     a = pts[0];
//     b = pts[1];
//     c = pts[2];
//   }
// }

Argon_Triangle::~Argon_Triangle() {};
