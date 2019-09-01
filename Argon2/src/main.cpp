#include "argon.h"
#include <cmath>

int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	argon.setStroke(255,0,0);
	argon.addMouseListener(CLICK, [](Argon& a, MouseEvent& e) {
		Points pts;
		pts.push_back({static_cast<int>(e.x + 30*cos(0)),static_cast<int>(e.y + 30*sin(0))});
		pts.push_back({static_cast<int>(e.x + 30*cos(3*M_PI_4)),static_cast<int>(e.y + 30*sin(3*M_PI_4))});
		pts.push_back({static_cast<int>(e.x + 30*cos(-3*M_PI_4)),static_cast<int>(e.y + 30*sin(-3*M_PI_4))});
		a.polygon(pts);
		a.strokePolygon(pts);
	});
	argon.start();
	return 0;
}