#include "argon.h"


int main(int argc, char* argv[]) {
	Argon argon("Argon", 60, ARGON_BASIC|ARGON_NOQUIT);
	argon.setBackground(255,255,255,255);
	argon.setStroke(0,0,0,255);
	argon.clear();
	int x = 0;
	int y = 0;
	bool down = false;;
	argon.addMouseListener(MOUSEENTER, [&](Argon& a, MouseEvent& e) {
		x = e.x;
		y = e.y;
		down = true;
	});
	argon.addMouseListener(MOUSELEAVE, [&](Argon& a, MouseEvent& e) {
		down = false;
	});
	argon.addMouseListener(MOUSEDOWN, [&](Argon& a, MouseEvent& e) {
		x = e.x;
		y = e.y;
		down = true;
	});
	argon.addMouseListener(MOUSEUP, [&](Argon& a, MouseEvent& e) {
		down = false;
	});
	argon.addMouseListener(CLICK, [&](Argon& a, MouseEvent& e) {
		string pos = to_string(e.x) + ", " + to_string(e.y);
		//DOES NOT WORK WHEN PASSING VARIABLES FOR SOME REASON
		a.messageBox("Mouse Position", "test");
	});
	argon.addMouseListener(MOUSEMOVE, [&](Argon& a, MouseEvent& e) {
		if(down) {
			a.line(x,y,e.x,e.y);
			x = e.x;
			y = e.y;
		}
	});
	argon.start();
	return 0;
}