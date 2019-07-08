#include <iostream>
#include "argon.h"
using namespace std;
void eventHandler(Argon* argon, Event e) {
	if(e.type == LOAD) {

	}
}
void program(Argon* argon) {

}
int main(int argc, char* argv[]) {
	cout << SDL_MOUSEWHEEL_NORMAL << endl;
	cout << SDL_MOUSEWHEEL_FLIPPED << endl;
	Argon argon("Test Window",					//WINDOW NAME
	 						60,											//FPS
							ARGON_BASIC|ARGON_RESIZEABLE,						//FLAGS
							&eventHandler,
							&program
						);
	return 0;
}
