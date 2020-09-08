#ifndef ARGON_H
#define ARGON_H

#include <cstdio>
#include <GLFW/glfw3.h>


class Argon {
private:
	//Window Properties
	GLFWwindow* window;
	const char* title = "Argon";
	int32_t x = 0;
	int32_t y = 0;
	uint32_t w = 500;
	uint32_t h = 500;


public:
	Argon();
	~Argon();

	void start();
};

#endif