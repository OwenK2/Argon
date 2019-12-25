#include "argon.h"

int main(int argc, char** argv) {
	int x = 100;
	int y = 100;
	int step = 10;
	int cf = 0;
	Argon_Flip flip = ARGON_FLIP_NONE;
	double angle = 0;
	Argon a("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	Argon_Image image(a, "/Users/owen/Documents/Photography/DSC_0001.JPG");
	Argon_Image i2(a, "/Users/owen/Documents/Photography/DSC_0015.JPG");
	image.setRotateCenter(10,10);
	i2.crop(0,0,3000,2000);
	a.setLoop([&](Argon& a) {
		a.clear();
		i2.render();
		Argon_Rect rect = {x,y,300,300};
		image.setAngle(angle);
		image.setFlip(flip);
		image.render(&rect);
		if(a.keys["W"]) {
			y -= step;
		}
		if(a.keys["A"]) {
			x -= step;
		}
		if(a.keys["S"]) {
			y += step;
		}
		if(a.keys["D"]) {
			x += step;
		}
	});
	a.setListener(KEYDOWN, [&](Argon& a, KeyboardEvent& e) {
		if(e.key == "Up") {
			int a = image.getAlpha() + 20;
			if(a > 255) {a = 255;}
			image.setAlpha(a);
		}
		else if(e.key == "Down") {
			int a = image.getAlpha() - 20;
			if(a < 0) {a = 0;}
			image.setAlpha(a);
		}
		else if(e.key == "Left") {
			angle --;
		}
		else if(e.key == "Right") {
			angle ++;
		}
		else if(e.key == "Tab") {
			cf ++;
			if(cf > 3) {cf = 0;}
			switch(cf) {
				case 0: flip = ARGON_FLIP_NONE; break;
				case 1: flip = ARGON_FLIP_HORIZONTAL; break;
				case 2: flip = ARGON_FLIP_VERTICAL; break;
				case 3: flip = ARGON_FLIP_BOTH; break;
				default: break;
			}
			printf("%d\n", cf);
		}
	});
	a.start();
	return 0;
}