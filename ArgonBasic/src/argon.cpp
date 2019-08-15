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
        case SDL_DROPFILE: {
          Event dropEvent = {DROPFILE,{},{},{},{e->drop.file,e->drop.timestamp}};
          argon->eventHandler(argon,dropEvent);
          SDL_free(e->drop.file);
          break;
        }
      }
    }
	}
  return 0;
}
void Argon::setColor(int r,int g,int b, int a) {
  SDL_SetRenderDrawColor(ren,r,g,b,a);
}
void Argon::point(double x, double y) {
  SDL_RenderDrawPoint(ren, x, y);
}
void Argon::line(double x1, double y1, double x2, double y2) {
  SDL_RenderDrawLine(ren, x1, y1, x2, y2);
}
void Argon::triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
  Vec2 a(x1,y1);
  Vec2 b(x2,y2);
  Vec2 c(x3,y3);
  //http://www-users.mat.uni.torun.pl/~wrona/3d_tutor/tri_fillers.html

  //Sort points by y values (top to bottom)
  if()
  the coordinates of vertices are (A.x,A.y), (B.x,B.y), (C.x,C.y); we assume that A.y<=B.y<=C.y (you should sort them first)
  dx1,dx2,dx3 are deltas used in interpolation
  horizline draws horizontal segment with coordinates (S.x,Y), (E.x,Y)
  S.x, E.x are left and right x-coordinates of the segment we have to draw
  S=A means that S.x=A.x; S.y=A.y;
  *** begin triangle filler ***

  	if (B.y-A.y > 0) dx1=(B.x-A.x)/(B.y-A.y) else dx1=0;
  	if (C.y-A.y > 0) dx2=(C.x-A.x)/(C.y-A.y) else dx2=0;
  	if (C.y-B.y > 0) dx3=(C.x-B.x)/(C.y-B.y) else dx3=0;

  	S=E=A;
  	if(dx1 > dx2) {
  		for(;S.y<=B.y;S.y++,E.y++,S.x+=dx2,E.x+=dx1)
  			horizline(S.x,E.x,S.y,color);
  		E=B;
  		for(;S.y<=C.y;S.y++,E.y++,S.x+=dx2,E.x+=dx3)
  			horizline(S.x,E.x,S.y,color);
  	} else {
  		for(;S.y<=B.y;S.y++,E.y++,S.x+=dx1,E.x+=dx2)
  			horizline(S.x,E.x,S.y,color);
  		S=B;
  		for(;S.y<=C.y;S.y++,E.y++,S.x+=dx3,E.x+=dx2)
  			horizline(S.x,E.x,S.y,color);
  	}

  *** end triangle filler ***
}
void Argon::polygon(std::vector<double> points) {

}
void Argon::rect(double x1, double y1, double x2, double y2) {
  SDL_Rect rect = {x1,y1,x2,y2};
  SDL_RenderFillRect(ren, &rect);
}


void Argon::quit() {
	running = 0;
	this->~Argon();
}
