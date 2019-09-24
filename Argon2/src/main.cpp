#include "argon.h"
#include <cmath>

int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	argon.setStroke(255,0,0);

	argon.setBackground(0,0,0,0);
	argon.setStroke(255,255,255,255);
	int x = 0;
	int y = 0;
	int h = -1;
	int w = -1;
	bool arrowkey[4] = {false,false,false,false};
	argon.addLoop([&](Argon& a) {
		//clear
		a.clear();
		
		//render
		a.image("./src/test.jpg", x,y,a.window.w,a.window.h);
		a.setFill(0, 0, 255);
		//a.halfSpaceTriangle(10, 10, 15, 60, 50, 50);
		a.triangle(10, 10, 15, 60, 50, 50);
		a.setFill(0, 255, 0);
		// a.point(10, 10);
		// a.point(15, 60);
		// a.point(50, 50);

		a.rect(0,0,10,10);

		//events
		// if(a.keyboard.state[SDL_SCANCODE_UP]) {
		// 	y -= 3;
		// }
		// if(a.keyboard.state[SDL_SCANCODE_LEFT]) {
		// 	x -= 3;
		// }
		// if(a.keyboard.state[SDL_SCANCODE_DOWN]) {
		// 	y += 3;
		// }
		// if(a.keyboard.state[SDL_SCANCODE_RIGHT]) {
		// 	x += 3;
		// }

		if(arrowkey[0]) {
			y -= 3;
		}
		if(arrowkey[1]) {
			x -= 3;
		}
		if(arrowkey[2]) {
			y += 3;
		}
		if(arrowkey[3]) {
			x += 3;
		}
	});
	argon.addMouseListener(MOUSEMOVE, [&](Argon& a, MouseEvent& e) {
		Argon_Color c = a.getPixel(e.x,e.y);
	});
	argon.addKeyboardListener(KEYDOWN, [&](Argon& a, KeyboardEvent& e) {
		if(strcmp(e.key,"W") == 0 || e.keyCode == 82) {arrowkey[0] = true;}
		if(strcmp(e.key,"A") == 0 || e.keyCode == 80) {arrowkey[1] = true;}
		if(strcmp(e.key,"S") == 0 || e.keyCode == 81) {arrowkey[2] = true;}
		if(strcmp(e.key,"D") == 0 || e.keyCode == 79) {arrowkey[3] = true;}
	});
	argon.addKeyboardListener(KEYUP, [&](Argon& a, KeyboardEvent& e) {
		if(strcmp(e.key,"W") == 0 || e.keyCode == 82) {arrowkey[0] = false;}
		if(strcmp(e.key,"A") == 0 || e.keyCode == 80) {arrowkey[1] = false;}
		if(strcmp(e.key,"S") == 0 || e.keyCode == 81) {arrowkey[2] = false;}
		if(strcmp(e.key,"D") == 0 || e.keyCode == 79) {arrowkey[3] = false;}
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
