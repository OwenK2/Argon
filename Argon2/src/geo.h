#ifndef GEO_H
#define GEO_H

#include <vector>

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
typedef vector<Point*> Points;

class Edge {
public:
	Point a;
	Point b;
  int min;
  int max;
  int curX;
  int msign;
  int dx;
  int dy;
  int sum;

	Edge() : max(0), min(0), curX(0), msign(0), dx(0), dy(0), sum(0) {};
	Edge(Point _a, Point _b) {
		a = (_a.y <= _b.y) ? _a : _b;
		b = (_a.y > _b.y) ? _a : _b;
		min = a.y;
		max = b.y;
		curX = a.x;
		dx = b.x - a.x;
		dy = b.y - a.y;
		msign = (dx == 0 || dy == 0) ? 0 : ((double(dy)/double(dx) > 0) ? 1 : -1);
		sum = 0;
	};
	~Edge() {};

  void print() {
  	cout << "[(" << a.x << "," << a.y << "); (" << b.x << "," << b.y << ")]" << endl;
  }
};
typedef vector<Edge*> EdgeTable;


class Argon_Rect {
public:
	int x;
	int y;
	int w;
	int h;

	Argon_Rect boundOnScreen(int sw, int sh) {
    return Argon_Rect((x < 0) ? 0 : ((x > sw) ? sw : x), (y < 0) ? 0 : ((y > sh) ? sh : y), (x+w > sw) ? sw-x : w, (y+h > sh) ? sh-y : h);
	}
  Argon_Rect() : x(0), y(0), w(0), h(0) {};
	Argon_Rect(SDL_Rect& r) {
		x = r.x;
		y = r.y;
		w = r.w;
		h = r.h;
	}
	Argon_Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};
  ~Argon_Rect() {};
};


class Argon_Polygon {
  public:
    int numVertices;
    Points points;
    EdgeTable edges;
    Argon_Rect boundRect;

    Argon_Polygon(Points _points) : numVertices(_points.size()), points(_points) {
      //get drawable bounding rect
      int top = 2147483647; //MAX_INT
      int bottom = 0;
      int left = 2147483647; //MAX_INT
      int right = 0;
      for(auto p : points) {
        top = std::min(top, p->y);
        bottom = std::max(bottom, p->y);
        left = std::min(left, p->x);
        right = std::max(right, p->x);
      }
      boundRect = Argon_Rect(left, top, right-left, bottom-top);

		  for(int i = 0;i < points.size();++i) {
    	   int j = i+1 >= points.size() ? 0 : i+1;
         Edge* e = new Edge(*(points.at(i)), *(points.at(j)));
         edges.push_back(e);
      }
      sort(edges.begin(),edges.end(),[](Edge* e, Edge* f){return e->min < f->min;});
    };

	  ~Argon_Polygon() {
      for(auto e : edges) {
        delete e;
      }
    };
};

    //     // Create edge table
//     finalEdgeTable = createEdges()// Sort edges by minY
//     sort(finalEdgeTable)processEdgeTable(finalEdgeTable)

// /*
//     Creates edge buckets from the given edges

//     @param n    Number of vertices
//     @param x[]  array of x points
//     @param y[]  array of y points

//     @return     List of edge buckets
//  */
// createEdges(n, x[], y[]) {
//     instantiate a new edge table   loop through x[] & y[] pairs {
//         if the edge's slope is NOT undefined (verticle) {
//             create bucket with edge
//             add bucket to edge table
//         }
//     }
// }/*
//     Given the edge table of the polygon, fill the polygons

//     @param edgeTable The polygon's edge table representation
//  */
// processEdgeTable (edgeTable) {
//     while (edge table is NOT empty) {// Remove edges from the active list if y == ymax
//         if (active list is NOT empty) {
//             for (iterate through all buckets in the active list) {
//                 if (current bucket's ymax == current scanline) {
//                     remove bucket from active list
//                     remove bucket from edge table
//                 }
//             }
//         }// Add edge from edge table to active list if y == ymin
//         for (iterate through the bucket in the edge table) {
//             if (bucket's ymin == scanline) {
//                 add bucket to active list
//             }
//         }// Sort active list by x position and slope
//         sortTheActiveList();// Fill the polygon pixel
//         for (iterate through the active list) {
//             for (from vertex1.x to vertex2.x of the bucket) {
//                 setPixelColor()
//             }
//         }// Increment X variables of buckets based on the slope
//         for (all buckets in the active list) {
//             if (bucketsdX != 0) {
//                 bucket's sum += bucket's dX       while (bucket's sum >= bucket's dY) {
//             increment or decrement bucket's X depending on sign of   bucket's slope
//             edge's sum -= dY
//                 }
//             }
//         }
//     }
// }///
// // Draw a filled polygon in the Canvas C.
// //
// // The polygon has n distinct vertices.  The coordinates of the vertices
// // making up the polygon are stored in the x and y arrays.  The ith
// // vertex will have coordinate (x[i],y[i]).
// //
// // @param n - number of vertices
// // @param x - x coordinates
// // @param y - y coordinates
// ///
// drawArgon_Polygon(n, x[], y[]) {
// }

#endif
