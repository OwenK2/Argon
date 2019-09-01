#include "argon.h"


int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
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
	argon.start();
	return 0;
}
