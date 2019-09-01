#include "argon.h"
#include <cmath>

int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	argon.setStroke(255,0,0);

	argon.setBackground(0,0,0,0);
	argon.setStroke(255,255,255,255);
	int x = 0;
	int y = 0;
	int h = 50;
	int w = 50;
	argon.addLoop([&](Argon& a) {
		a.clear();
		a.image("/Users/michaelkuhn/Downloads/IMG_1504.JPG", x,y,w,h);
	});
	argon.addKeyboardListener(KEYDOWN, [&](Argon& a, KeyboardEvent& e) {
		if(strcmp(e.key,"W") == 0) {y -= 3;}
		if(strcmp(e.key,"A") == 0) {x -= 3;}
		if(strcmp(e.key,"S") == 0) {y += 3;}
		if(strcmp(e.key,"D") == 0) {x += 3;}
	});
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
