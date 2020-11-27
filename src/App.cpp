#include "App.h"



void App::onLoad(const WindowEvent event) {
	setColor(0x000000FF);
	clear();
	setColor(255,255,255);
	line(0, 0, 500, 500);
	line(500, 0, 0, 500);


	setColor(255,0,0);

	rect(10,10,50,50, 4);
}

void App::gameLoop(const uint32_t deltaTime) {
	// line(-250,-250,250,250);
	// line(250,-250,-250,250);
	fillCircle(300,300, 25);

	ellipse(100,100,25,50);
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
