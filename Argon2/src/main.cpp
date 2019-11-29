#include "argon.h"

int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	argon.addWindowListener(LOAD, [](Argon& a, WindowEvent& e) {
		printf("Drawing... %dx%d\n", a.window.w, a.window.h);
		a.setBackground(255,0,0);
		a.clear();

		//render
		a.image("./src/test.jpg", x,y,a.window.w,a.window.h);

		Points pts = {new Point(10, 10), new Point(100, 200), new Point(200, 300), new Point(300, 200), new Point(400, 400)};

		a.setStroke(0, 255, 0);
		a.nicBezier(pts);
		a.setStroke(255, 255, 255);

		a.polygon(pts);

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
		int dx = 0;
		int dy = 0;
		if(arrowkey[0]) {
			dy -= 3;
		}
		if(arrowkey[1]) {
			dx -= 3;
		}
		a.putRGBA(data);
		delete[] data;
	});
	argon.start();
	return 0;
}
