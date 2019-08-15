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
}

void Argon::start() {
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
      a->window.shown = false;
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
    case SDL_WINDOWEVENT: {
      switch (e->window.event) {
        case SDL_WINDOWEVENT_CLOSE: {
          a->window.shown = false;
          WindowEvent event = {
            CLOSE,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->closeListeners);
          break;
        }
        case SDL_WINDOWEVENT_SHOWN: {
          a->window.shown = true;
          WindowEvent event = {
            SHOWN,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->shownListeners);
          break;
        }
        case SDL_WINDOWEVENT_HIDDEN: {
          a->window.shown = false;
          WindowEvent event = {
            HIDDEN,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->hiddenListeners);
          break;
        }
        case SDL_WINDOWEVENT_EXPOSED: {
          a->window.shown = true;
          WindowEvent event = {
            EXPOSED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->exposedListeners);
          break;
        }
        case SDL_WINDOWEVENT_MOVED: {
          a->window.x = e->window.data1;
          a->window.y = e->window.data2;
          WindowEvent event = {
            MOVED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->movedListeners);
          break;
        }
        case SDL_WINDOWEVENT_RESIZED: {
          SDL_GetWindowPosition(a->win,&a->window.x,&a->window.y);
          SDL_GetRendererOutputSize(a->ren, &a->window.w, &a->window.h);
          SDL_GL_GetDrawableSize(a->win, &a->window.dw, &a->window.dh);
          WindowEvent event = {
            RESIZED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->resizedListeners);
          break;
        }
        case SDL_WINDOWEVENT_SIZE_CHANGED: {
          SDL_GetWindowPosition(a->win,&a->window.x,&a->window.y);
          SDL_GetRendererOutputSize(a->ren, &a->window.w, &a->window.h);
          SDL_GL_GetDrawableSize(a->win, &a->window.dw, &a->window.dh);
          WindowEvent event = {
            SIZECHANGED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->sizeChangedListeners);
          break;
        }
        case SDL_WINDOWEVENT_MINIMIZED: {
          a->window.shown = false;
          WindowEvent event = {
            MINIMIZED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->minimizedListeners);
          break;
        }
        case SDL_WINDOWEVENT_MAXIMIZED: {
          a->window.shown = true;
          WindowEvent event = {
            MINIMIZED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->maximizedListeners);
          break;
        }
        case SDL_WINDOWEVENT_RESTORED: {
          WindowEvent event = {
            RESTORED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->restoredListeners);
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_GAINED: {
          WindowEvent event = {
            KEYBOARDFOCUS,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->keyboardFocusListeners);
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_LOST: {
          WindowEvent event = {
            KEYBOARDBLUR,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->keyboardBlurListeners);
          break;
        }
        case SDL_WINDOWEVENT_TAKE_FOCUS: {
          WindowEvent event = {
            TAKEFOCUS,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->takeFocusListeners);
          break;
        }
        case SDL_WINDOWEVENT_HIT_TEST: {
          WindowEvent event = {
            HITTEST,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          a->eventHandler(&event,&a->hitTestListeners);
          break;
        }
      }
      case SDL_WINDOWEVENT_ENTER: {
        SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
        MouseEvent event = {
          MOUSEENTER, 
          a->mouse.x,
          a->mouse.y,
          a->mouse.down,
          a->mouse.which
        };
        a->eventHandler(&event,&a->mouseEnterListeners);
        break;
      }
      case SDL_WINDOWEVENT_LEAVE: {
        SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
        MouseEvent event = {
          MOUSELEAVE, 
          a->mouse.x,
          a->mouse.y,
          a->mouse.down,
          a->mouse.which
        };
        a->eventHandler(&event,&a->mouseLeaveListeners);
        break;
      }
    }
    case SDL_MOUSEBUTTONUP: {
      SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
      a->mouse.which = e->button.button;
      a->mouse.down = false;
      MouseEvent event = {
        MOUSEUP, 
        a->mouse.x,
        a->mouse.y,
        false,
        a->mouse.which
      };
      a->eventHandler(&event,&a->mouseUpListeners);
      if(a->canCountClick) {
        a->canCountClick = false;
        MouseEvent event = {
          CLICK, 
          a->mouse.x,
          a->mouse.y,
          false,
          a->mouse.which
        };
        a->eventHandler(&event,&a->clickListeners);
        if(SDL_GetTicks() - a->lastClick < a->dblClickTime) {
          MouseEvent event = {
            DBLCLICK, 
            a->mouse.x,
            a->mouse.y,
            false,
            a->mouse.which
          };          
          a->eventHandler(&event,&a->dblclickListeners);
        }
        a->lastClick = SDL_GetTicks();
      }
      break;
    }   
    case SDL_MOUSEBUTTONDOWN: {
      SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
      a->mouse.which = e->button.button;
      a->canCountClick = true;
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
    case SDL_MOUSEMOTION: {
      SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
      MouseEvent event = {
        MOUSEMOVE, 
        a->mouse.x,
        a->mouse.y,
        a->mouse.down,
        a->mouse.which
      };
      a->eventHandler(&event,&a->mouseMoveListeners);
      break;    
    }
    case SDL_KEYUP: {
      a->keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      KeyboardEvent event = {
        KEYUP,e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)
      };
      a->eventHandler(&event,&a->keyUpListeners);
      break;
    }
    case SDL_KEYDOWN: {
      a->keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      KeyboardEvent event = {
        KEYDOWN,e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)
      };
      a->eventHandler(&event,&a->keyDownListeners);
      break;
    }
    case SDL_MOUSEWHEEL: {
      WheelEvent event = {
        MOUSEWHEEL,
        e->wheel.x,
        e->wheel.y,
        static_cast<bool>(e->wheel.direction)
      };
      a->eventHandler(&event,&a->mouseWheelListeners);
      break;
    }
    case SDL_DROPFILE: {
      FileEvent event = {
        DROPFILE,
        e->drop.file,
        e->drop.timestamp
      };
      a->eventHandler(&event,&a->dropFileListeners);
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
