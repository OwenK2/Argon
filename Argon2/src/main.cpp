#include <iostream>
#include "argon.h"

void meme(Event* event) {
	cout << "MEME" << endl;
}
int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC);
	argon.addListener(MOUSEDOWN, meme);
	return 0;
}
