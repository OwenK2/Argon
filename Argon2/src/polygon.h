using namespace std;

struct Point {
	int x;
	int y;
	void print() {
		cout << "(" << x << ", " << y << ")" << endl;
	}
};
typedef vector<Point> Points;
struct Edge {
  int max;
  int min;
  int x;
  int sign;
  int dx;
  int dy;
  double sum;
  void print() {
  	cout << "(" << max << ", " << min << ", " << x << ", " << sign << ", " << dx << ", " << dy << ", " << sum << ")" << endl;
  }
};
typedef vector<Edge> EdgeTable;

class Polygon {
public:
	Polygon(Points points) {
		EdgeTable table;
    sort(points.begin(),points.end(),[](Point& a, Point& b){return a.x < b.x;});
    for(auto pt : points) {
    	pt.print();
    }
		for(int i = 0;i < points.size();++i) {
    	int j = i+1 >= points.size() ? 0 : i+1, ymax,ymin,x,sign;
    	if(points[i].y == points[j].y) {continue;}
    	if(points[i].y > points[j].y) {ymax = points[i].y;ymin = points[j].y;x = points[j].x;}
    	else {ymax = points[j].y;ymin = points[i].y;x = points[i].x;}
    	sign = 1;
    	table.push_back({ymax,ymin,x,sign,abs(points[i].x - points[j].x),abs(points[i].y - points[j].y),0});
    }
    sort(table.begin(),table.end(),[](Edge& a, Edge& b){return a.min < b.min;});
    for(auto ed : table) {
    	ed.print();
    }
  };
	~Polygon() {};
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
// drawPolygon(n, x[], y[]) {
// }