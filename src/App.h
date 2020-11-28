#ifndef APP_H
#define APP_H

#define ARGON_FIXED_FRAMERATE 

#define ARGON_FPS 120

#include "argon.h"

class App : public Argon {
public:
	App() : Argon("Title", 600, 600) {}
	void gameLoop();
	void onKeyDown(const KeyEvent event);
	void onKeyUp(const KeyEvent event);
private:
	int px = 600 - 50;
	int py = 300;
	int pw = 15;
	int ph = 75;
	float speed = 3;
	


	int bx = 300;
	int by = 300;
	int radius = 15;
	float vx = 3;
	float vy = 2;

	

	bool sDown = false;
	bool wDown = false;

};

#endif
