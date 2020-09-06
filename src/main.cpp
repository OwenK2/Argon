#include "argon.h"



int main(int argc, char** argv) {
	Argon a("Argon", 60, ARGON_BASIC|ARGON_IMAGES);
	a.setBackground(255,28,120);
	a.setLoop([](Argon& a) {
		a.clear();
	});
	a.start();
	return 0;
}
