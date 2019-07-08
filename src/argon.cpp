#include "argon.h"
#include "log.h"
Log logger(LOG_BASIC);

Argon::Argon(const char* _name, int _fps, uint32_t _flags,void _event(Argon* a, Event e),void _loop(Argon* a)) : name(_name), fps(_fps), eventHandler(_event), renderLoop(_loop) {
  start(_flags);
}
Argon::Argon(const char* _name, int _w, int _h, int _fps, uint32_t _flags, void _event(Argon* a, Event e),void _loop(Argon* a)) : name(_name), w(_w), h(_h), fps(_fps), eventHandler(_event), renderLoop(_loop) {
  start(_flags);
}
Argon::Argon(const char* _name, int _x, int _y, int _w, int _h, int _fps, uint32_t _flags, void _event(Argon* a, Event e),void _loop(Argon* a)) : name(_name), x(_x), y(_y), w(_w), h(_h),fps(_fps), eventHandler(_event), renderLoop(_loop) {
  start(_flags);
}
Argon::~Argon() {
  SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
  SDL_DelEventWatch(eventWatcher,this);
	SDL_Quit();
}


void Argon::start(int flags) {
  SDL_Init(SDL_INIT_VIDEO);
  win = SDL_CreateWindow(name,x,y,w,h,flags);
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetWindowTitle(win,name);
	SDL_GetRendererOutputSize(ren, &w, &h);
  SDL_GL_GetDrawableSize(win, &dw, &dh);
  SDL_AddEventWatch(eventWatcher, this);
  running = true;
  frameTime = 1000 / fps;
  Event loadEvent = {LOAD};
  eventHandler(this, loadEvent);
  loop();
}

void Argon::loop() {
	Uint32 time = SDL_GetTicks();
  SDL_Event e;
  while(running) {
  	SDL_GetMouseState(&mouse.x, &mouse.y);
    Uint32 now = SDL_GetTicks();
    int maxBehind = 10;
    if(time <= now) {
	    while(time <= now  && (maxBehind--)) {
				renderLoop(this);
				time += frameTime;
	    }
      SDL_RenderPresent(ren);
    }
    else {
    	SDL_Delay(time - now);
    }
    while(SDL_PollEvent(&e)) {
      eventWatcher(this,&e);
    }
  }
  quit();
}

int Argon::eventWatcher(void* data, SDL_Event* e) {
	Argon* argon = ((Argon*)data);
	switch(e->type) {
		case SDL_QUIT: {
      Event quitEvent = {QUIT};
      argon->eventHandler(argon,quitEvent);
      argon->quit();
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      SDL_GetMouseState(&argon->mouse.x, &argon->mouse.y);
      Event mouseEvent = {MOUSEDOWN, {argon->mouse.x,argon->mouse.y,e->button.button,true}};
      argon->mouse.down = true;
      argon->mouse.which = e->button.button;
      argon->eventHandler(argon,mouseEvent);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      SDL_GetMouseState(&argon->mouse.x, &argon->mouse.y);
      Event mouseEvent = {MOUSEUP, {argon->mouse.x,argon->mouse.y,e->button.button,false}};
      argon->mouse.down = false;
      argon->mouse.which = e->button.button;
      argon->eventHandler(argon,mouseEvent);
      break;
    }
    case SDL_MOUSEMOTION: {
      SDL_GetMouseState(&argon->mouse.x, &argon->mouse.y);
      Event mouseEvent = {MOUSEUP, {argon->mouse.x,argon->mouse.y,argon->mouse.which,argon->mouse.down}};
      argon->eventHandler(argon,mouseEvent);
      break;
    }
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
    // case SDL_QUIT:
    //   Event quitEvent = {QUIT};
    //   argon->eventHandler(argon,quitEvent);
    //   argon->quit();
    //   break;
		case SDL_WINDOWEVENT: {
			if(e->window.event == SDL_WINDOWEVENT_RESIZED) {
				SDL_GL_GetDrawableSize(argon->win, &argon->dw, &argon->dh);
				SDL_GetRendererOutputSize(argon->ren, &argon->w, &argon->h);
			}
		}
		break;
	}
  // argon->eventHandler(argon, *e);
  return 0;
}
void Argon::quit() {
	running = 0;
	this->~Argon();
}
