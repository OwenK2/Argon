#include "argon.h"
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
  quit();
}

void Argon::init(int x,int y,int w,int h,int flags) {
  int sdl_flags = 0;
  if(flags & 0x1) {sdl_flags |= SDL_WINDOW_FULLSCREEN;}
  if((flags >> 1) & 0x1) {sdl_flags |= SDL_WINDOW_HIDDEN;}
  if((flags >> 2) & 0x1) {sdl_flags |= SDL_WINDOW_BORDERLESS;}
  if((flags >> 3) & 0x1) {sdl_flags |= SDL_WINDOW_RESIZABLE;}
  if((flags >> 4) & 0x1) {sdl_flags |= SDL_WINDOW_MINIMIZED;}
  if((flags >> 5) & 0x1) {sdl_flags |= SDL_WINDOW_MAXIMIZED;}
  if((flags >> 6) & 0x1) {sdl_flags |= SDL_WINDOW_ALLOW_HIGHDPI;}
  if((flags >> 7) & 0x1) {quitOnClose = false;}
  if((flags >> 8) & 0x1) {useImages = true;}

  SDL_InitSubSystem(SDL_INIT_VIDEO);
  if(useImages) {
    int img_flags = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;
    if((IMG_Init(img_flags) & img_flags) != img_flags) {
      lastError = "Failed to init Image Library";
    }
  }
  win = SDL_CreateWindow(name,x,y,w,h,sdl_flags);
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  surface = SDL_GetWindowSurface(win);
  SDL_SetWindowTitle(win,name);
  SDL_GetRendererOutputSize(ren, &window.w, &window.h);
  SDL_GL_GetDrawableSize(win, &window.dw, &window.dh);
  SDL_AddEventWatch(eventWatcher, this);
  running = true;
  frameTime = 1000 / fps;
}
void Argon::quit() {
  running = false;
  SDL_DelEventWatch(eventWatcher,this);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}
void Argon::close() {
  running = false;
  if(quitOnClose) {quit();return;}
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(ren);
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Argon::start() {
  uint32_t time = SDL_GetTicks();
  SDL_Event e;
  while(running) {
    uint32_t now = SDL_GetTicks();
    int maxBehind = 10;
    if(time <= now) {
      while(time <= now  && (maxBehind--)) {
        skipCallstack = true;
        for(int i = 0;i < tasklist.size();++i) {
          tasklist[i](*this);
        }
        skipCallstack = false;
        for(int i = 0;i < callstack.size();++i) {
          callstack[i](*this);
        }
        callstack.clear();
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
      for(auto it = a->quitListeners.begin(); it != a->quitListeners.end(); ++it) {(**it)(*a,event);}
      a->quit();
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
          for(auto it = a->closeListeners.begin(); it != a->closeListeners.end(); ++it) {(**it)(*a,event);}
          a->close();
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
          for(auto it = a->shownListeners.begin(); it != a->shownListeners.end(); ++it) {(**it)(*a,event);}
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
          for(auto it = a->hiddenListeners.begin(); it != a->hiddenListeners.end(); ++it) {(**it)(*a,event);}
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
          for(auto it = a->exposedListeners.begin(); it != a->exposedListeners.end(); ++it) {(**it)(*a,event);}
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
          for(auto it = a->movedListeners.begin(); it != a->movedListeners.end(); ++it) {(**it)(*a,event);}
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
          SDL_UpdateWindowSurface(a->win);
          for(auto it = a->resizedListeners.begin(); it != a->resizedListeners.end(); ++it) {(**it)(*a,event);}
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
          for(auto it = a->sizeChangedListeners.begin(); it != a->sizeChangedListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
        case SDL_WINDOWEVENT_MINIMIZED: {
          SDL_GetWindowPosition(a->win,&a->window.x,&a->window.y);
          SDL_GetRendererOutputSize(a->ren, &a->window.w, &a->window.h);
          SDL_GL_GetDrawableSize(a->win, &a->window.dw, &a->window.dh);
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
          for(auto it = a->minimizedListeners.begin(); it != a->minimizedListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
        case SDL_WINDOWEVENT_MAXIMIZED: {
          SDL_GetWindowPosition(a->win,&a->window.x,&a->window.y);
          SDL_GetRendererOutputSize(a->ren, &a->window.w, &a->window.h);
          SDL_GL_GetDrawableSize(a->win, &a->window.dw, &a->window.dh);
          a->window.shown = true;
          WindowEvent event = {
            MAXIMIZED,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          for(auto it = a->maximizedListeners.begin(); it != a->maximizedListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
        case SDL_WINDOWEVENT_RESTORED: {
          SDL_GetWindowPosition(a->win,&a->window.x,&a->window.y);
          SDL_GetRendererOutputSize(a->ren, &a->window.w, &a->window.h);
          SDL_GL_GetDrawableSize(a->win, &a->window.dw, &a->window.dh);
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
          for(auto it = a->restoredListeners.begin(); it != a->restoredListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_GAINED: {
          WindowEvent event = {
            FOCUS,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          for(auto it = a->focusListeners.begin(); it != a->focusListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_LOST: {
          WindowEvent event = {
            BLUR,
            a->window.x,
            a->window.y,
            a->window.w,
            a->window.h,
            a->window.dw,
            a->window.dh,
            a->window.shown
          };
          for(auto it = a->blurListeners.begin(); it != a->blurListeners.end(); ++it) {(**it)(*a,event);}
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
          for(auto it = a->takeFocusListeners.begin(); it != a->takeFocusListeners.end(); ++it) {(**it)(*a,event);}
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
          for(auto it = a->hitTestListeners.begin(); it != a->hitTestListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
        case SDL_WINDOWEVENT_ENTER: {
          SDL_GetWindowPosition(a->win,&a->window.x,&a->window.y);
          uint32_t state = SDL_GetGlobalMouseState(&a->mouse.x, &a->mouse.y);
          a->mouse.x -= a->window.x;
          a->mouse.y -= a->window.y;
          a->mouse.ldown = state&SDL_PRESSED;
          a->mouse.mdown = state&(SDL_PRESSED<<1);
          a->mouse.rdown = state&(SDL_PRESSED<<2);
          a->mouse.wdown = state&(SDL_PRESSED<<4);
          a->mouse.down = a->mouse.ldown|a->mouse.mdown|a->mouse.rdown;
          MouseEvent event = {
            MOUSEENTER, 
            a->mouse.x,
            a->mouse.y,
            a->mouse.down,
            a->mouse.ldown,
            a->mouse.mdown,
            a->mouse.rdown,
            a->mouse.wdown,
            a->mouse.which
          };
          for(auto it = a->mouseEnterListeners.begin(); it != a->mouseEnterListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
        case SDL_WINDOWEVENT_LEAVE: {
          a->canCountClick = false;
          uint32_t state = SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
          a->mouse.ldown = state&SDL_PRESSED;
          a->mouse.mdown = state&(SDL_PRESSED<<1);
          a->mouse.rdown = state&(SDL_PRESSED<<2);
          a->mouse.wdown = state&(SDL_PRESSED<<4);
          a->mouse.down = a->mouse.ldown|a->mouse.mdown|a->mouse.rdown;
          MouseEvent event = {
            MOUSELEAVE, 
            a->mouse.x,
            a->mouse.y,
            a->mouse.down,
            a->mouse.ldown,
            a->mouse.mdown,
            a->mouse.rdown,
            a->mouse.wdown,
            a->mouse.which
          };
          for(auto it = a->mouseLeaveListeners.begin(); it != a->mouseLeaveListeners.end(); ++it) {(**it)(*a,event);}
          break;
        }
      }
    }
    case SDL_MOUSEBUTTONUP: {
      uint32_t state = SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
      a->mouse.ldown = state&SDL_PRESSED;
      a->mouse.mdown = state&(SDL_PRESSED<<1);
      a->mouse.rdown = state&(SDL_PRESSED<<2);
      a->mouse.wdown = state&(SDL_PRESSED<<4);
      a->mouse.down = a->mouse.ldown|a->mouse.mdown|a->mouse.rdown;
      a->mouse.which = e->button.button;
      MouseEvent event = {
        MOUSEUP, 
        a->mouse.x,
        a->mouse.y,
        a->mouse.down,
        a->mouse.ldown,
        a->mouse.mdown,
        a->mouse.rdown,
        a->mouse.wdown,
        a->mouse.which
      };
      for(auto it = a->mouseUpListeners.begin(); it != a->mouseUpListeners.end(); ++it) {(**it)(*a,event);}
      if(a->canCountClick) {
        a->canCountClick = false;
        MouseEvent event = {
          CLICK, 
          a->mouse.x,
          a->mouse.y,
          a->mouse.down,
          a->mouse.ldown,
          a->mouse.mdown,
          a->mouse.rdown,
          a->mouse.wdown,
          a->mouse.which
        };
        for(auto it = a->clickListeners.begin(); it != a->clickListeners.end(); ++it) {(**it)(*a,event);}
        if(SDL_GetTicks() - a->lastClick < a->dblClickTime) {
          MouseEvent event = {
            DBLCLICK, 
            a->mouse.x,
            a->mouse.y,
            a->mouse.down,
            a->mouse.ldown,
            a->mouse.mdown,
            a->mouse.rdown,
            a->mouse.wdown,
            a->mouse.which
          };          
          for(auto it = a->dblclickListeners.begin(); it != a->dblclickListeners.end(); ++it) {(**it)(*a,event);}
        }
        a->lastClick = SDL_GetTicks();
      }
      break;
    }   
    case SDL_MOUSEBUTTONDOWN: {
      uint32_t state = SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
      a->mouse.ldown = state&SDL_PRESSED;
      a->mouse.mdown = state&(SDL_PRESSED<<1);
      a->mouse.rdown = state&(SDL_PRESSED<<2);
      a->mouse.wdown = state&(SDL_PRESSED<<4);
      a->mouse.down = a->mouse.ldown|a->mouse.mdown|a->mouse.rdown;
      a->mouse.which = e->button.button;
      a->canCountClick = true;
      MouseEvent event = {
        MOUSEDOWN, 
        a->mouse.x,
        a->mouse.y,
        a->mouse.down,
        a->mouse.ldown,
        a->mouse.mdown,
        a->mouse.rdown,
        a->mouse.wdown,
        a->mouse.which
      };
      for(auto it = a->mouseDownListeners.begin(); it != a->mouseDownListeners.end(); ++it) {(**it)(*a,event);}
      break;
    }
    case SDL_MOUSEMOTION: {
      uint32_t state = SDL_GetMouseState(&a->mouse.x, &a->mouse.y);
      a->mouse.ldown = state&SDL_PRESSED;
      a->mouse.mdown = state&(SDL_PRESSED<<1);
      a->mouse.rdown = state&(SDL_PRESSED<<2);
      a->mouse.wdown = state&(SDL_PRESSED<<4);
      a->mouse.down = a->mouse.ldown|a->mouse.mdown|a->mouse.rdown;
      MouseEvent event = {
        MOUSEMOVE, 
        a->mouse.x,
        a->mouse.y,
        a->mouse.down,
        a->mouse.ldown,
        a->mouse.mdown,
        a->mouse.rdown,
        a->mouse.wdown,
        a->mouse.which
      };
      for(auto it = a->mouseMoveListeners.begin(); it != a->mouseMoveListeners.end(); ++it) {(**it)(*a,event);}
      break;    
    }
    case SDL_KEYUP: {
      a->keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      KeyboardEvent event = {
        KEYUP,e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)
      };
      for(auto it = a->keyUpListeners.begin(); it != a->keyUpListeners.end(); ++it) {(**it)(*a,event);}
      break;
    }
    case SDL_KEYDOWN: {
      a->keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      KeyboardEvent event = {
        KEYDOWN,e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)
      };
      for(auto it = a->keyDownListeners.begin(); it != a->keyDownListeners.end(); ++it) {(**it)(*a,event);}
      break;
    }
    case SDL_MOUSEWHEEL: {
      WheelEvent event = {
        MOUSEWHEEL,
        e->wheel.x,
        e->wheel.y,
        static_cast<bool>(e->wheel.direction)
      };
      for(auto it = a->mouseWheelListeners.begin(); it != a->mouseWheelListeners.end(); ++it) {(**it)(*a,event);}
      break;
    }
    case SDL_DROPFILE: {
      FileEvent event = {
        DROPFILE,
        e->drop.file,
        e->drop.timestamp
      };
      for(auto it = a->dropFileListeners.begin(); it != a->dropFileListeners.end(); ++it) {(**it)(*a,event);}
      break;
    } 
  }

  return 0;
}
void Argon::addWindowListener(EventType type, WindowListener listener) {
  switch(type) {
    case QUIT: quitListeners.push_back(&listener); break;
    case CLOSE: closeListeners.push_back(&listener); break;
    case SHOWN: shownListeners.push_back(&listener); break;
    case HIDDEN: hiddenListeners.push_back(&listener); break;
    case EXPOSED: exposedListeners.push_back(&listener); break;
    case MOVED: movedListeners.push_back(&listener); break;
    case RESIZED: resizedListeners.push_back(&listener); break;
    case SIZECHANGED: sizeChangedListeners.push_back(&listener); break;
    case MINIMIZED: minimizedListeners.push_back(&listener); break;
    case MAXIMIZED: maximizedListeners.push_back(&listener); break;
    case RESTORED: restoredListeners.push_back(&listener); break;
    case FOCUS: focusListeners.push_back(&listener); break;
    case BLUR: blurListeners.push_back(&listener); break;
    case TAKEFOCUS: takeFocusListeners.push_back(&listener); break;
    case HITTEST: hitTestListeners.push_back(&listener); break;
    default: break;
  }
}
void Argon::addMouseListener(EventType type, MouseListener listener) {
  switch(type) {
    case MOUSEENTER: mouseEnterListeners.push_back(&listener); break;
    case MOUSELEAVE: mouseLeaveListeners.push_back(&listener); break;
    case MOUSEUP: mouseUpListeners.push_back(&listener); break;
    case MOUSEDOWN: mouseDownListeners.push_back(&listener); break;
    case MOUSEMOVE: mouseMoveListeners.push_back(&listener); break;
    case CLICK: clickListeners.push_back(&listener); break;
    case DBLCLICK: dblclickListeners.push_back(&listener); break;
    default: break;
  }}
void Argon::addKeyboardListener(EventType type, KeyboardListener listener) {
  switch(type) {
    case KEYUP: keyUpListeners.push_back(&listener); break;
    case KEYDOWN: keyDownListeners.push_back(&listener); break;
    default: break;
  }
}
void Argon::addWheelListener(EventType type, WheelListener listener) {
  switch(type) {
    case MOUSEWHEEL: mouseWheelListeners.push_back(&listener); break;
    default: break;
  }
}
void Argon::addFileListener(EventType type, FileListener listener) {
  switch(type) {
    case DROPFILE: dropFileListeners.push_back(&listener); break;
    default: break;
  }
}
bool Argon::removeListener(EventType type, int index) {
  return false;
  switch(type) {
    case QUIT: if(quitListeners.at(index) != NULL) {quitListeners.erase(quitListeners.begin()+index);return true;} else {return false;} break;
    case CLOSE: if(closeListeners.at(index) != NULL) {closeListeners.erase(closeListeners.begin()+index);return true;} else {return false;} break;
    case SHOWN: if(shownListeners.at(index) != NULL) {shownListeners.erase(shownListeners.begin()+index);return true;} else {return false;} break;
    case HIDDEN: if(hiddenListeners.at(index) != NULL) {hiddenListeners.erase(hiddenListeners.begin()+index);return true;} else {return false;} break;
    case EXPOSED: if(exposedListeners.at(index) != NULL) {exposedListeners.erase(exposedListeners.begin()+index);return true;} else {return false;} break;
    case MOVED: if(movedListeners.at(index) != NULL) {movedListeners.erase(movedListeners.begin()+index);return true;} else {return false;} break;
    case RESIZED: if(resizedListeners.at(index) != NULL) {resizedListeners.erase(resizedListeners.begin()+index);return true;} else {return false;} break;
    case SIZECHANGED: if(sizeChangedListeners.at(index) != NULL) {sizeChangedListeners.erase(sizeChangedListeners.begin()+index);return true;} else {return false;} break;
    case MINIMIZED: if(minimizedListeners.at(index) != NULL) {minimizedListeners.erase(minimizedListeners.begin()+index);return true;} else {return false;} break;
    case MAXIMIZED: if(maximizedListeners.at(index) != NULL) {maximizedListeners.erase(maximizedListeners.begin()+index);return true;} else {return false;} break;
    case RESTORED: if(restoredListeners.at(index) != NULL) {restoredListeners.erase(restoredListeners.begin()+index);return true;} else {return false;} break;
    case FOCUS: if(focusListeners.at(index) != NULL) {focusListeners.erase(focusListeners.begin()+index);return true;} else {return false;} break;
    case BLUR: if(blurListeners.at(index) != NULL) {blurListeners.erase(blurListeners.begin()+index);return true;} else {return false;} break;
    case TAKEFOCUS: if(takeFocusListeners.at(index) != NULL) {takeFocusListeners.erase(takeFocusListeners.begin()+index);return true;} else {return false;} break;
    case HITTEST: if(hitTestListeners.at(index) != NULL) {hitTestListeners.erase(hitTestListeners.begin()+index);return true;} else {return false;} break;
    case MOUSEENTER: if(mouseEnterListeners.at(index) != NULL) {mouseEnterListeners.erase(mouseEnterListeners.begin()+index);return true;} else {return false;} break;
    case MOUSELEAVE: if(mouseLeaveListeners.at(index) != NULL) {mouseLeaveListeners.erase(mouseLeaveListeners.begin()+index);return true;} else {return false;} break;
    case MOUSEUP: if(mouseUpListeners.at(index) != NULL) {mouseUpListeners.erase(mouseUpListeners.begin()+index);return true;} else {return false;} break;
    case MOUSEDOWN: if(mouseDownListeners.at(index) != NULL) {mouseDownListeners.erase(mouseDownListeners.begin()+index);return true;} else {return false;} break;
    case MOUSEMOVE: if(mouseMoveListeners.at(index) != NULL) {mouseMoveListeners.erase(mouseMoveListeners.begin()+index);return true;} else {return false;} break;
    case CLICK: if(clickListeners.at(index) != NULL) {clickListeners.erase(clickListeners.begin()+index);return true;} else {return false;} break;
    case DBLCLICK: if(dblclickListeners.at(index) != NULL) {dblclickListeners.erase(dblclickListeners.begin()+index);return true;} else {return false;} break;
    case KEYUP: if(keyUpListeners.at(index) != NULL) {keyUpListeners.erase(keyUpListeners.begin()+index);return true;} else {return false;} break;
    case KEYDOWN: if(keyDownListeners.at(index) != NULL) {keyDownListeners.erase(keyDownListeners.begin()+index);return true;} else {return false;} break;
    case MOUSEWHEEL: if(mouseWheelListeners.at(index) != NULL) {mouseWheelListeners.erase(mouseWheelListeners.begin()+index);return true;} else {return false;} break;
    case DROPFILE: if(dropFileListeners.at(index) != NULL) {dropFileListeners.erase(dropFileListeners.begin()+index);return true;} else {return false;} break;
  }
}
void Argon::addLoop(Task task) {
  tasklist.push_back(task);
}
bool Argon::removeLoop(int index) {
  if(tasklist.at(index) != NULL) {
    tasklist.erase(tasklist.begin()+index);
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

void Argon::setBackground(Argon_Color color) {
  function<void(Argon&)> f = [=](Argon& a) {a.bg = color;};
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::setBackground(int r, int g, int b, int a) {
  Argon_Color color = {static_cast<uint8_t>(r),static_cast<uint8_t>(g),static_cast<uint8_t>(b),static_cast<uint8_t>(a)};
  setBackground(color);
}
void Argon::setStroke(Argon_Color color) {
  function<void(Argon&)> f = [=](Argon& a) {a.stroke = color;};
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::setStroke(int r, int g, int b, int a) {
  Argon_Color color = {static_cast<uint8_t>(r),static_cast<uint8_t>(g),static_cast<uint8_t>(b),static_cast<uint8_t>(a)};
  setStroke(color);
}
void Argon::setFill(Argon_Color color) {
  function<void(Argon&)> f = [=](Argon& a) {a.fill = color;};
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::setFill(int r, int g, int b, int a) {
  Argon_Color color = {static_cast<uint8_t>(r),static_cast<uint8_t>(g),static_cast<uint8_t>(b),static_cast<uint8_t>(a)};
  setFill(color);
}

void Argon::clear() {
  function<void(Argon&)> f = ([](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.bg.r, a.bg.g, a.bg.b, a.bg.a);
    SDL_RenderClear(a.ren);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::clear(int x, int y, int w, int h) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.bg.r, a.bg.g, a.bg.b, a.bg.a);
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderFillRect(a.ren, &rect);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::point(int x, int y) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    SDL_RenderDrawPoint(a.ren, x, y);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::line(int x1, int y1, int x2, int y2) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    SDL_RenderDrawLine(a.ren, x1, y1, x2, y2);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::rect(int x, int y, int w, int h) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.fill.r, a.fill.g, a.fill.b, a.fill.a);
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderFillRect(a.ren, &rect);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::circle(int cx, int cy, int r) {
  function<void(Argon&)> f = ([=](Argon& a) {
    int x = r;
    int y = 0;
    int err = 0;
    SDL_SetRenderDrawColor(a.ren, a.fill.r, a.fill.g, a.fill.b, a.fill.a);
    while (x >= y) {
      SDL_RenderDrawLine(a.ren, cx-x, cy+y, cx+x, cy+y);
      SDL_RenderDrawLine(a.ren, cx-y, cy+x, cx+y, cy+x);
      SDL_RenderDrawLine(a.ren, cx-x, cy-y, cx+x, cy-y);
      SDL_RenderDrawLine(a.ren, cx-y, cy-x, cx+y, cy-x);
      if (err <= 0) {
        y += 1;
        err += 2*y + 1;
      }
     
      if (err > 0) {
        x -= 1;
        err -= 2*x + 1;
      }
    }
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::strokeCircle(int cx, int cy, int r) {
  function<void(Argon&)> f = ([=](Argon& a) {
    int x = r;
    int y = 0;
    int err = 0;
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    while (x >= y) {
      SDL_RenderDrawPoint(a.ren,cx + x, cy + y);
      SDL_RenderDrawPoint(a.ren,cx + y, cy + x);
      SDL_RenderDrawPoint(a.ren,cx - y, cy + x);
      SDL_RenderDrawPoint(a.ren,cx - x, cy + y);
      SDL_RenderDrawPoint(a.ren,cx - x, cy - y);
      SDL_RenderDrawPoint(a.ren,cx - y, cy - x);
      SDL_RenderDrawPoint(a.ren,cx + y, cy - x);
      SDL_RenderDrawPoint(a.ren,cx + x, cy - y);
      if (err <= 0) {
        y += 1;
        err += 2*y + 1;
      }
     
      if (err > 0) {
        x -= 1;
        err -= 2*x + 1;
      }
    }
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::polygon(Points& points) {
  int top = 2000000000;
  int bottom = -2000000000;
  int left = 2000000000;
  int right = -2000000000;
  for(auto pt : points) {
    cout << "\e[32m(" << pt.x << ", " << pt.y << ")\e[0m" << endl;
    if(pt.y < top) {top = pt.y;}
    else if(pt.y > bottom) {bottom = pt.y;}
    if(pt.x > right) {right = pt.x;}
    else if(pt.x < left) {left = pt.x;}
  }
  function<void(Argon&)> f = ([=](Argon& a) {
    Polygon p(points);

  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
  // int  nodes, nodeX[MAX_POLY_CORNERS], pixelX, pixelY, i, j, swap ;

  // //  Loop through the rows of the image.
  // for (pixelY=IMAGE_TOP; pixelY<IMAGE_BOT; pixelY++) {

  //   //  Build a list of nodes.
  //   nodes=0; j=polyCorners-1;
  //   for (i=0; i<polyCorners; i++) {
  //     if (polyY[i]<(double) pixelY && polyY[j]>=(double) pixelY
  //     ||  polyY[j]<(double) pixelY && polyY[i]>=(double) pixelY) {
  //       nodeX[nodes++]=(int) (polyX[i]+(pixelY-polyY[i])/(polyY[j]-polyY[i])
  //       *(polyX[j]-polyX[i])); }
  //     j=i; }

  //   //  Sort the nodes, via a simple “Bubble” sort.
  //   i=0;
  //   while (i<nodes-1) {
  //     if (nodeX[i]>nodeX[i+1]) {
  //       swap=nodeX[i]; nodeX[i]=nodeX[i+1]; nodeX[i+1]=swap; if (i) i--; }
  //     else {
  //       i++; }}
//  Fill the pixels between node pairs.
// for (i=0; i<nodes; i+=2) {
//   if   (nodeX[i  ]>=IMAGE_RIGHT) break;
//   if   (nodeX[i+1]> IMAGE_LEFT ) {
//     if (nodeX[i  ]< IMAGE_LEFT ) nodeX[i  ]=IMAGE_LEFT ;
//     if (nodeX[i+1]> IMAGE_RIGHT) nodeX[i+1]=IMAGE_RIGHT;
//     for (pixelX=nodeX[i]; pixelX<nodeX[i+1]; pixelX++) fillPixel(pixelX,pixelY); }}}

}
void Argon::strokePolygon(Points& points) {
  if(points.size() < 3) {return;}
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    for(auto it = points.begin()+1;it != points.end();++it) {
      a.line((it-1)->x,(it-1)->y,it->x,it->y);
    }
    a.line(points.back().x,points.back().y,points.begin()->x,points.begin()->y);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
Argon_Rect Argon::image(const char* path, int sx, int sy, int sw, int sh, int dx, int dy,int dw, int dh) {
  SDL_Surface* final;
  SDL_Surface* initial = IMG_Load(path);
  if(initial == NULL) {lastError = "Unable to optimize image " + string(path) + "! SDL Error: " + SDL_GetError();return {-1,-1,-1,-1};}
  final = SDL_ConvertSurface(initial, surface->format, 0);
  SDL_FreeSurface(initial);
  if(sw < 0) {sw = final->w;}
  if(sh < 0) {sh = final->h;}
  if(dw < 0) {dw = final->w;}
  if(dh < 0) {dh = final->h;}
  SDL_Rect src = {sx,sy,sw,sh};
  SDL_Rect dest = {dx,dy,dw,dh};
  SDL_BlitScaled(final, &src, surface, &dest);
  SDL_UpdateWindowSurface(win);
  return {dx,dy,dw,dh};
}
Argon_Rect Argon::image(const char* path,int x, int y, int w, int h) {
  return image(path,0,0,-1,-1,x,y,w,h);
}
Argon_Rect Argon::image(const char* path,int x, int y) {
  return image(path,x,y,-1,-1,0,0,-1,-1);
}
void Argon::wait(int ms) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_Delay(ms);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::messageBox(const char* title,const char* message, uint32_t flags) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_ShowSimpleMessageBox(flags,title,message,a.win);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}}
