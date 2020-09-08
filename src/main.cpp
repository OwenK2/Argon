#include "argon.h"



int main(int argc, char** argv) {
	Argon a;
	SDL_Rect r = {0,0,0,0};
	a.setEventHandler(QUIT, [](Argon& a, Event e) {
		printf("Quit Event Handled\n");
	});
	
	a.setLoop([](Argon& a, void* data) {
		a.setFill(TRANSPARENT);
		a.setStroke(RED);
		a.drawRect(100,100,10,10);
	}, &r);

	a.start();

	return 0;
}
