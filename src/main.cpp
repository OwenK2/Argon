#include <iostream>
#include "argon.h"
using namespace std;
void eventHandler(Argon* argon, Event e) {
	/*
	EVENTS
	=======================
	LOAD
	SHOWN
	HIDDEN
	EXPOSED
	MOVED
	RESIZED
	SIZE_CHANGED
	MINIMIZED
	MAXIMIZED
	RESTORED
	MOUSEENTER
	MOUSELEAVE
	KEYBOARDFOCUS
	KEYBOARDBLUR
	CLOSE
	TAKE_FOCUS
	HIT_TEST
	MOUSEUP
	MOUSEDOWN
	MOUSEMOVE
	MOUSEWHEEL
	CLICK				----- (DBL CLICK TOO?)
	KEYUP
	KEYDOWN
	QUIT
	*/
	//ECHO EVEnt
	// cout << "\e[1m\e[33m[EVENT]  " << e.type << "\e[0m" << endl;
}
void program(Argon* argon) {

}
int main(int argc, char* argv[]) {
	Argon argon("Test Window",					//WINDOW NAME
	 						60,											//FPS
							ARGON_BASIC|ARGON_RESIZEABLE,						//FLAGS
							&eventHandler,
							&program
						);
	return 0;
}
