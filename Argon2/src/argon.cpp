#include "argon.h"
#include <iostream>
using namespace std;


Argon::Argon(const char* _name, int _fps, int _flags) : name(_name), fps(_fps) {
  init(SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,_flags);
}
Argon::Argon(const char* _name, int _fps, int _w, int _h, int _flags) : name(_name), fps(_fps) {
  init(SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,_w,_h,_flags);
}
Argon::Argon(const char* _name, int _fps, int _x, int _y, int _w, int _h, int _flags) : name(_name), fps(_fps) {
  init(_x,_y,_w,_h,_flags);
}
Argon::~Argon() {
  SDL_DelEventWatch(eventWatcher,this);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}


void Argon::init(int x,int y,int w,int h,int flags) {
  SDL_Init(SDL_INIT_VIDEO);
  win = SDL_CreateWindow(name,x,y,w,h,flags);
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetWindowTitle(win,name);
  SDL_GetRendererOutputSize(ren, &window.w, &window.h);
  SDL_GL_GetDrawableSize(win, &window.dw, &window.dh);
  SDL_AddEventWatch(eventWatcher, this);
  running = true;
  frameTime = 1000 / fps;
  loop();
}

void Argon::loop() {
  uint32_t time = SDL_GetTicks();
  SDL_Event e;
  while(running) {
    uint32_t now = SDL_GetTicks();
    int maxBehind = 10;
    if(time <= now) {
      while(time <= now  && (maxBehind--)) {
        //RUNSHIT
        time += frameTime;
      }
      SDL_RenderPresent(ren);
    }
    else {
      SDL_Delay(time - now);
    }
    SDL_PollEvent(&e);
  }
}

int Argon::eventWatcher(void* data, SDL_Event* e) {
  Argon* a = ((Argon*)data);
  switch(e->type) {
    case SDL_QUIT: {
      WindowEvent event = {
        QUIT,
        a->window.x,
        a->window.y,
        a->window.w,
        a->window.h,
        a->window.dw,
        a->window.dh,
        a->window.shown
      };
      a->eventHandler(&event,&a->quitListeners);
      a->running = false;
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
      a->mouse.which = e->button.button;
      a->mouse.down = true;
      MouseEvent event = {
        MOUSEDOWN, 
        a->mouse.x,
        a->mouse.y,
        true,
        a->mouse.which
      };
      a->eventHandler(&event,&a->mouseDownListeners);
      break;
    }
  }

  return 0;
}
void Argon::eventHandler(Event* event, Listeners* listeners) {
  cout << listeners->size() << " Listeners Found for " << event->type << endl;
  for(auto it = listeners->begin(); it != listeners->end(); ++it) {
    (*it)(event);
  }
}


Listeners* Argon::getListeners(EventType type) {
  Listeners* listeners;
  switch(type) {
    case QUIT: listeners = &quitListeners; break;
    case CLOSE: listeners = &closeListeners; break;
    case SHOWN: listeners = &shownListeners; break;
    case HIDDEN: listeners = &hiddenListeners; break;
    case EXPOSED: listeners = &exposedListeners; break;
    case MOVED: listeners = &movedListeners; break;
    case RESIZED: listeners = &resizedListeners; break;
    case SIZECHANGED: listeners = &sizeChangedListeners; break;
    case MINIMIZED: listeners = &minimizedListeners; break;
    case MAXIMIZED: listeners = &maximizedListeners; break;
    case RESTORED: listeners = &restoredListeners; break;
    case KEYBOARDFOCUS: listeners = &keyboardFocusListeners; break;
    case KEYBOARDBLUR: listeners = &keyboardBlurListeners; break;
    case TAKEFOCUS: listeners = &takeFocusListeners; break;
    case HITTEST: listeners = &hitTestListeners; break;
    case MOUSEENTER: listeners = &mouseEnterListeners; break;
    case MOUSELEAVE: listeners = &mouseLeaveListeners; break;
    case MOUSEUP: listeners = &mouseUpListeners; break;
    case MOUSEDOWN: listeners = &mouseDownListeners; break;
    case MOUSEMOVE: listeners = &mouseMoveListeners; break;
    case CLICK: listeners = &clickListeners; break;
    case DBLCLICK: listeners = &dblclickListeners; break;
    case KEYUP: listeners = &keyUpListeners; break;
    case KEYDOWN: listeners = &keyDownListeners; break;
    case MOUSEWHEEL: listeners = &mouseWheelListeners; break;
    case DROPFILE: listeners = &dropFileListeners; break;
  }
  return listeners;
}
void Argon::addListener(EventType type, Listener listener) {
  Listeners* listeners = getListeners(type);
  cout << "Listener added to " << type << endl;
  listeners->push_back(listener);
}
bool Argon::removeListener(EventType type, Listener listener) {
  Listeners* listeners = getListeners(type);
  Listeners::iterator it = find(listeners->begin(),listeners->end(), listener);
  if(it != listeners->end()) {
    listeners->erase(it--);
    return true;
  }
  return false;
}
void Argon::setFps(int _fps) {
  fps = _fps;
  frameTime = 1000 / fps;
}
int Argon::getFps() {
  return fps;
}
