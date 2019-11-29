#include "argon.h"

int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	argon.addWindowListener(LOAD, [](Argon& a, WindowEvent& e) {
		printf("Drawing... %dx%d\n", a.window.w, a.window.h);
		a.setBackground(255,0,0);
		a.clear();
		uint8_t* data = new uint8_t[a.window.h*a.window.w*4];
		for(int i = 0;i < a.window.h*a.window.w;++i) {
			memset(&data[i*4], 255, 4);
			data[i*4+3] = 150;
		}
		a.putRGBA(data);
		delete[] data;
	});
	argon.start();
	return 0;
}
