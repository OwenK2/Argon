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
	bool arrowkey[4];
	argon.addLoop([&](Argon& a) {
		a.clear();
		if(a.keyboard.state[SDL_SCANCODE_UP]) {
			y -= 3;
		}
		if(a.keyboard.state[SDL_SCANCODE_LEFT]) {
			x -= 3;
		}
		if(a.keyboard.state[SDL_SCANCODE_DOWN]) {
			y += 3;
		}
		if(a.keyboard.state[SDL_SCANCODE_RIGHT]) {
			x += 3;
		}
		a.image("/Users/owen/Documents/Coding/landing/midground.png", x,y);
	});
	argon.addKeyboardListener(KEYDOWN, [&](Argon& a, KeyboardEvent& e) {
		if(strcmp(e.key,"W") == 0) {arrowkey[0] = true;}
		if(strcmp(e.key,"A") == 0) {arrowkey[1] = true;}
		if(strcmp(e.key,"S") == 0) {arrowkey[2] = true;}
		if(strcmp(e.key,"D") == 0) {arrowkey[3] = true;}
	});
	argon.addKeyboardListener(KEYUP, [&](Argon& a, KeyboardEvent& e) {
		if(strcmp(e.key,"W") == 0) {arrowkey[0] = false;}
		if(strcmp(e.key,"A") == 0) {arrowkey[1] = false;}
		if(strcmp(e.key,"S") == 0) {arrowkey[2] = false;}
		if(strcmp(e.key,"D") == 0) {arrowkey[3] = false;}
	});
	// argon.addMouseListener(CLICK, [](Argon& a, Event& e) {
	// 	Points pts;
	// 	pts.push_back({0,0});
	// 	pts.push_back({100,100});
	// 	pts.push_back({0,100});
	// 	a.strokePolygon(pts);
	//
	// });
	argon.start();
	return 0;
}
