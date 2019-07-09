#include "argon.h"
//FOR TESTING DELETE WHEN DONE
#include <iostream>
using namespace std;

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
  IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
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
    while(SDL_PollEvent(&e)) {} //CRASHES IF YOU DONT HAVE THIS FOR SOME REASON
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
      argon->canCountClick = true;
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
      if(argon->canCountClick) {
        argon->canCountClick = false;
        Event mouseEvent2 = {CLICK, {argon->mouse.x,argon->mouse.y,e->button.button,false}};
        argon->eventHandler(argon,mouseEvent2);
        if(SDL_GetTicks() - argon->lastClick < argon->dblClickTime) {
          Event mouseEvent3 = {DBLCLICK, {argon->mouse.x,argon->mouse.y,e->button.button,false}};
          argon->eventHandler(argon,mouseEvent3);
        }
        argon->lastClick = SDL_GetTicks();
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      SDL_GetMouseState(&argon->mouse.x, &argon->mouse.y);
      Event mouseEvent = {MOUSEMOVE, {argon->mouse.x,argon->mouse.y,argon->mouse.which,argon->mouse.down}};
      argon->eventHandler(argon,mouseEvent);
      break;
    }
    case SDL_MOUSEWHEEL: {
      Event mouseEvent = {MOUSEWHEEL,{}, {},{e->wheel.x,e->wheel.y,static_cast<bool>(e->wheel.direction)}};
      argon->eventHandler(argon,mouseEvent);
      break;
    }
    case SDL_KEYDOWN: {
      argon->keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      Event keyEvent = {KEYDOWN, {}, argon->keyboard};
      argon->eventHandler(argon,keyEvent);
      break;
    }
    case SDL_KEYUP: {
      argon->keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      Event keyEvent = {KEYUP, {}, argon->keyboard};
      argon->eventHandler(argon,keyEvent);
      break;
    }
    case SDL_WINDOWEVENT: {
      switch(e->window.event) {
        case SDL_WINDOWEVENT_SHOWN: {
          Event winEvent = {SHOWN};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_EXPOSED: {
          Event winEvent = {EXPOSED};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_MOVED: {
          Event winEvent = {MOVED};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_SIZE_CHANGED: {
          Event winEvent = {SIZE_CHANGED};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_MINIMIZED: {
          Event winEvent = {MINIMIZED};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_MAXIMIZED: {
          Event winEvent = {MAXIMIZED};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_RESTORED: {
          Event winEvent = {RESTORED};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_ENTER: {
          SDL_GetMouseState(&argon->mouse.x, &argon->mouse.y);
          Event mouseEvent = {MOUSEENTER, {argon->mouse.x,argon->mouse.y,argon->mouse.which,argon->mouse.down}};
          argon->eventHandler(argon,mouseEvent);
          break;
        }
        case SDL_WINDOWEVENT_LEAVE: {
          SDL_GetMouseState(&argon->mouse.x, &argon->mouse.y);
          Event mouseEvent = {MOUSELEAVE, {argon->mouse.x,argon->mouse.y,argon->mouse.which,argon->mouse.down}};
          argon->eventHandler(argon,mouseEvent);
          argon->canCountClick = false;
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_GAINED: {
          Event winEvent = {KEYBOARDFOCUS};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_LOST: {
          Event winEvent = {KEYBOARDBLUR};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_CLOSE: {
          Event winEvent = {CLOSE};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_TAKE_FOCUS: {
          Event winEvent = {TAKE_FOCUS};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_HIT_TEST: {
          Event winEvent = {HIT_TEST};
          argon->eventHandler(argon,winEvent);
          break;
        }
        case SDL_WINDOWEVENT_RESIZED: {
          Event winEvent = {RESIZED};
          SDL_GL_GetDrawableSize(argon->win, &argon->dw, &argon->dh);
  				SDL_GetRendererOutputSize(argon->ren, &argon->w, &argon->h);
          argon->eventHandler(argon,winEvent);
          break;
        }
      }
    }
	}
  return 0;
}
ImageData Argon::screenshot() {
  return screenshot(0,0,w,h);
}
ImageData Argon::screenshot(int _x,int _y,int _w,int _h) {
  ImageData data;
  SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
  SDL_RenderReadPixels(ren, NULL, SDL_PIXELFORMAT_RGBA32, surface->pixels, _w*4);
  IMG_SavePNG(surface, "screenshot.png");
  SDL_FreeSurface(surface);
  return data;
}
void Argon::setColor(int r,int g,int b, int a) {
  SDL_SetRenderDrawColor(ren,r,g,b,a);
}
void Argon::point(int x, int y) {
  SDL_RenderDrawPoint(ren, x, y);
}
void Argon::line(int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(ren, x1, y1, x2, y2);
}
void Argon::rect(int x1, int y1, int x2, int y2) {
  SDL_Rect rect = {x1,y1,x2,y2};
  SDL_RenderFillRect(ren, &rect);
}


void Argon::quit() {
	running = 0;
	this->~Argon();
}
