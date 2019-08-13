#include <iostream>
#include "argon.h"

static Argon* argonSingleton = nullptr;

void meme(Event* event) {
	cout << "MEME" << endl;
}

void loop() {

}

int main(int argc, char* argv[]) {
	argonSingleton = new Argon("Argon", [] {
		cout << "Adding event listener from user lambda!" << endl;
		if (argonSingleton == nullptr) {
			printf("Argon singleton is a null ptr\n");
		} else {
			printf("Argon singleton exists\n");
			argonSingleton->addListener(MOUSEDOWN, &meme);
		}
	}, 60, ARGON_BASIC);

	printf("Begining argon...\n");
	argonSingleton->begin();
	
	//ABILITY TO DRAW WHATEVER
	return 0;
}
