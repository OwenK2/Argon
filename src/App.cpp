#include "App.h"



void App::onLoad(const WindowEvent event) {
	setColor(0,0,0);
	clear();
	setColor(255,255,255);
	line(0, 0, 500, 500);
	line(500, 0, 0, 500);
}

void App::gameLoop(const uint32_t deltaTime) {

	// line(-250,-250,250,250);
	// line(250,-250,-250,250);
}
void App::onMouseDown(const MouseButtonEvent event) {
	sx = event.x;
	sy = event.y;
}

void App::onMouseMove(const MouseEvent event) {
	if(event.down) {
		line(sx, sy, event.x, event.y);
		sx = event.x;
		sy = event.y;
	}
}