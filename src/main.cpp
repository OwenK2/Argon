#include "argon.h"



int main(int argc, char** argv) {
	Argon a("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	a.setBackground(255,28,120);
	a.setLoop([](Argon& a) {
		a.clear();
		Argon_RGB c(255,16,175,255);
		Argon_HSL h = c.toHSL();
		Argon RGB c2 = h.toRGB();
		printf("%f %f %f %f\n", h.h, h.s, h.l, h.a);
		printf("%f %f %f %f\n", c2.r,c2.g,c2.b,c2.a);
	});
	a.start();
	return 0;
}
