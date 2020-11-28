#include "app.h"



void App::gameLoop() {
	setColor(0,0,0);
	clear();
	setColor(255,255,255);



	//collision
	int testx = bx, testy = by;
	if(bx < px) testx = px;
	else if(bx > px+pw) testx = px+pw;
	if(by < py) testy = py;
	else if(by > by+py) testy = by+py;


	float dx = bx - testx;
	float dy = by - testy;
	float d = sqrt(dx * dx + dy * dy);
	if(d < radius) {
		vx *= -1;
	}
	if(bx < radius) {
		vx *= -1;
	}
	if(by < radius) {
		vy *= -1;
	}
	if(by > 600-radius) {
		vy *= -1;
	}

	//player movement
	if(sDown) {
		py += speed;
	}
	else if(wDown) {
		py -= speed;
	}
	//player bounds
	if(py < 0) {py = 0;}
	else if(py > 600 - ph) {py = h() - ph;}

	//Ball velocity
	bx += vx;
	by += vy;

	circle(bx, by, radius);
	rect(px, py, pw, ph);
}
void App::onKeyDown(const KeyEvent event) {
	if(event.keycode == SDLK_s) {
		sDown = true;
	}
	else if(event.keycode == SDLK_w) {
		wDown = true;
	}
}

void App::onKeyUp(const KeyEvent event) {
	if(event.keycode == SDLK_s) {
		sDown = false;
	}
	else if(event.keycode == SDLK_w) {
		wDown = false;
	}
}

