#include "Argon.h"


Argon::Argon() {
  if(!glfwInit()) {
    printf("\e[31m[ERROR] Failed to initalize GLFW\e[0m\n");
  }
  
  if(!(window = glfwCreateWindow(w, h, title, NULL, NULL))) {
    printf("\e[31m[ERROR] Failed to create window\e[0m\n");
    this->~Argon();
  }
  glfwMakeContextCurrent(window);

}

Argon::~Argon() {
	glfwTerminate();
}



void Argon::start() {
  while (!glfwWindowShouldClose(window)) {

      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
  }
}