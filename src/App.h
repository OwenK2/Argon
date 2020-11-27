#ifndef APP_H
#define APP_H


#include "Argon.h"

class App : public Argon {
public:
	App() : Argon("Title") {}
	void gameLoop(const uint32_t deltaTime);
	void onMouseMove(const MouseEvent event);
	void onMouseDown(const MouseButtonEvent event);
	void onLoad(const WindowEvent event);
private:
	int i = 0;
	uint64_t f = 0;
	int32_t sx = 0;
	int32_t sy = 0;

};

#endif
