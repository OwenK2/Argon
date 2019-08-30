#include "argon.h"

int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	argon.addMouseListener(CLICK, [](Argon& a, Event& e) {
		Points pts;
		pts.push_back({0,0});
		pts.push_back({100,100});
		pts.push_back({0,100});
		a.strokePolygon(pts);
	});
	argon.start();
	return 0;
}