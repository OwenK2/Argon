#include "argon.h"
using namespace std;


//Argon

Argon::Argon(const char* _name, int _fps, int _flags) : name(_name), fps(_fps) {
  init(SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,_flags);
}
Argon::Argon(const char* _name, int _w, int _h, int _fps, int _flags) : name(_name), fps(_fps) {
  init(SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,_w,_h,_flags);
}
Argon::Argon(const char* _name, int _x, int _y, int _w, int _h, int _fps, int _flags) : name(_name), fps(_fps) {
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
  if((flags >> 9) & 0x1) {useText = true;}

  SDL_InitSubSystem(SDL_INIT_VIDEO);
  if(useImages) {
    int img_flags = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;
    if((IMG_Init(img_flags) & img_flags) != img_flags) {
      lastError = "Failed to init Image Library";
    }
  }

  if(useText) {
    if(TTF_Init() < 0) {
      lastError = "Failed to init Text Library";
    }
  }

  win = SDL_CreateWindow(name,x,y,w,h,sdl_flags);
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  surface = SDL_GetWindowSurface(win);
  SDL_SetWindowTitle(win,name);
  SDL_GetRendererOutputSize(ren, &window.w, &window.h);
  SDL_GL_GetDrawableSize(win, &window.dw, &window.dh);
  SDL_AddEventWatch(resizeWatcher, this);
  running = true;
  frameTime = 1000 / fps;
}

void Argon::start() {
  uint32_t time = SDL_GetTicks();
  SDL_Event e;
  if(loadListener != NULL) {
    WindowEvent event = {LOAD,window.x,window.y,window.w,window.h,window.dw,window.dh,window.shown};
    (*loadListener)(*this, event);
  }
  while(running) {
    uint32_t now = SDL_GetTicks();
    int maxBehind = 10;
    if(time <= now) {
      while(time <= now  && (maxBehind--)) {
        skipCallstack = true;
        if(mainLoop != NULL) {
          (*mainLoop)(*this);
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
    while(SDL_PollEvent(&e)) {
      eventWatcher(&e);
    }
  }
}
void Argon::quit() {
  for(auto img : imageCache) {
    delete img;
  }
  int was_init = TTF_WasInit();
  if (was_init == 1) {
    for(auto f : fonts) {
      TTF_CloseFont(f);
    }
    TTF_Quit();
  }
  running = false;
  SDL_DelEventWatch(resizeWatcher,this);
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
int Argon::resizeWatcher(void* data, SDL_Event* e) {
  Argon* a = (Argon*)data;
  if(e->type == SDL_WINDOWEVENT) {
    if(e->window.event == SDL_WINDOWEVENT_RESIZED) {
      SDL_GetWindowPosition(a->win,&a->window.x,&a->window.y);
      SDL_GetRendererOutputSize(a->ren, &a->window.w, &a->window.h);
      SDL_GL_GetDrawableSize(a->win, &a->window.dw, &a->window.dh);
      if(a->resizeListener != NULL) {
        WindowEvent event = {
          RESIZE,
          a->window.x,
          a->window.y,
          a->window.w,
          a->window.h,
          a->window.dw,
          a->window.dh,
          a->window.shown
        };
        (*a->resizeListener)(*a,event);
      }
    }
  }
  return 0;
}
void Argon::eventWatcher(SDL_Event* e) {
  switch(e->type) {
    case SDL_QUIT: {
      window.shown = false;
      if(quitListener != NULL) {
        WindowEvent event = {
          QUIT,
          window.x,
          window.y,
          window.w,
          window.h,
          window.dw,
          window.dh,
          window.shown
        };
  			(*quitListener)(*this,event);
      }
      quit();
      break;
    }
    case SDL_WINDOWEVENT: {
      switch (e->window.event) {
        case SDL_WINDOWEVENT_CLOSE: {
          window.shown = false;
          if(closeListener != NULL) {
            WindowEvent event = {
              CLOSE,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*closeListener)(*this,event);
          }
          close();
          break;
        }
        case SDL_WINDOWEVENT_SHOWN: {
          window.shown = true;
          if(shownListener != NULL) {
            WindowEvent event = {
              SHOWN,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*shownListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_HIDDEN: {
          window.shown = false;
          if(hiddenListener != NULL) {
            WindowEvent event = {
              HIDDEN,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*hiddenListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_EXPOSED: {
          window.shown = true;
          if(exposedListener != NULL) {
            WindowEvent event = {
              EXPOSED,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*exposedListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_MOVED: {
          window.x = e->window.data1;
          window.y = e->window.data2;
          if(movedListener != NULL) {
            WindowEvent event = {
              MOVED,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*movedListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_SIZE_CHANGED: {
          SDL_GetWindowPosition(win,&window.x,&window.y);
          SDL_GetRendererOutputSize(ren, &window.w, &window.h);
          SDL_GL_GetDrawableSize(win, &window.dw, &window.dh);
          if(sizeChangedListener != NULL) {
            WindowEvent event = {
              SIZECHANGED,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*sizeChangedListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_MINIMIZED: {
          SDL_GetWindowPosition(win,&window.x,&window.y);
          SDL_GetRendererOutputSize(ren, &window.w, &window.h);
          SDL_GL_GetDrawableSize(win, &window.dw, &window.dh);
          window.shown = false;
          if(minimizedListener != NULL) {
            WindowEvent event = {
              MINIMIZED,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*minimizedListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_MAXIMIZED: {
          SDL_GetWindowPosition(win,&window.x,&window.y);
          SDL_GetRendererOutputSize(ren, &window.w, &window.h);
          SDL_GL_GetDrawableSize(win, &window.dw, &window.dh);
          window.shown = true;
          if(maximizedListener != NULL) {
            WindowEvent event = {
              MAXIMIZED,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*maximizedListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_RESTORED: {
          SDL_GetWindowPosition(win,&window.x,&window.y);
          SDL_GetRendererOutputSize(ren, &window.w, &window.h);
          SDL_GL_GetDrawableSize(win, &window.dw, &window.dh);
          if(restoredListener != NULL) {
            WindowEvent event = {
              RESTORED,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*restoredListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_GAINED: {
          if(focusListener != NULL) {
            WindowEvent event = {
              FOCUS,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*focusListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_LOST: {
          if(blurListener != NULL) {
            WindowEvent event = {
              BLUR,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*blurListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_TAKE_FOCUS: {
          if(takeFocusListener != NULL) {
            WindowEvent event = {
              TAKEFOCUS,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*takeFocusListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_HIT_TEST: {
          if(hitTestListener != NULL) {
            WindowEvent event = {
              HITTEST,
              window.x,
              window.y,
              window.w,
              window.h,
              window.dw,
              window.dh,
              window.shown
            };
  					(*hitTestListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_ENTER: {
          SDL_GetWindowPosition(win,&window.x,&window.y);
          uint32_t state = SDL_GetGlobalMouseState(&mouse.x, &mouse.y);
          mouse.x -= window.x;
          mouse.y -= window.y;
          mouse.ldown = state&SDL_PRESSED;
          mouse.mdown = state&(SDL_PRESSED<<1);
          mouse.rdown = state&(SDL_PRESSED<<2);
          mouse.wdown = state&(SDL_PRESSED<<4);
          mouse.down = mouse.ldown|mouse.mdown|mouse.rdown;
          if(mouseEnterListener != NULL) {
            MouseEvent event = {
              MOUSEENTER,
              mouse.x,
              mouse.y,
              mouse.down,
              mouse.ldown,
              mouse.mdown,
              mouse.rdown,
              mouse.wdown,
              mouse.which
            };
  					(*mouseEnterListener)(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_LEAVE: {
          canCountClick = false;
          uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
          mouse.ldown = state&SDL_PRESSED;
          mouse.mdown = state&(SDL_PRESSED<<1);
          mouse.rdown = state&(SDL_PRESSED<<2);
          mouse.wdown = state&(SDL_PRESSED<<4);
          mouse.down = mouse.ldown|mouse.mdown|mouse.rdown;
          if(mouseLeaveListener != NULL) {
            MouseEvent event = {
              MOUSELEAVE,
              mouse.x,
              mouse.y,
              mouse.down,
              mouse.ldown,
              mouse.mdown,
              mouse.rdown,
              mouse.wdown,
              mouse.which
            };
  					(*mouseLeaveListener)(*this,event);
          }
          break;
        }
      }
    }
    case SDL_MOUSEBUTTONUP: {
      uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
      mouse.ldown = state&SDL_PRESSED;
      mouse.mdown = state&(SDL_PRESSED<<1);
      mouse.rdown = state&(SDL_PRESSED<<2);
      mouse.wdown = state&(SDL_PRESSED<<4);
      mouse.down = mouse.ldown|mouse.mdown|mouse.rdown;
      mouse.which = e->button.button;
      if(mouseUpListener != NULL) {
        MouseEvent event = {
          MOUSEUP,
          mouse.x,
          mouse.y,
          mouse.down,
          mouse.ldown,
          mouse.mdown,
          mouse.rdown,
          mouse.wdown,
          mouse.which
        };
  			(*mouseUpListener)(*this,event);
      }
      if(canCountClick) {
        canCountClick = false;
        if(clickListener != NULL) {
          MouseEvent event = {
            CLICK,
            mouse.x,
            mouse.y,
            mouse.down,
            mouse.ldown,
            mouse.mdown,
            mouse.rdown,
            mouse.wdown,
            mouse.which
          };
  				(*clickListener)(*this,event);
        }
        if(SDL_GetTicks() - lastClick < dblClickTime && dblclickListener != NULL) {
          MouseEvent event = {
            DBLCLICK,
            mouse.x,
            mouse.y,
            mouse.down,
            mouse.ldown,
            mouse.mdown,
            mouse.rdown,
            mouse.wdown,
            mouse.which
          };
					(*dblclickListener)(*this,event);
        }
        lastClick = SDL_GetTicks();
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
      mouse.ldown = state&SDL_PRESSED;
      mouse.mdown = state&(SDL_PRESSED<<1);
      mouse.rdown = state&(SDL_PRESSED<<2);
      mouse.wdown = state&(SDL_PRESSED<<4);
      mouse.down = mouse.ldown|mouse.mdown|mouse.rdown;
      mouse.which = e->button.button;
      canCountClick = true;
      if(mouseDownListener != NULL) {
        MouseEvent event = {
          MOUSEDOWN,
          mouse.x,
          mouse.y,
          mouse.down,
          mouse.ldown,
          mouse.mdown,
          mouse.rdown,
          mouse.wdown,
          mouse.which
        };
  			(*mouseDownListener)(*this,event);
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
      mouse.ldown = state&SDL_PRESSED;
      mouse.mdown = state&(SDL_PRESSED<<1);
      mouse.rdown = state&(SDL_PRESSED<<2);
      mouse.wdown = state&(SDL_PRESSED<<4);
      mouse.down = mouse.ldown|mouse.mdown|mouse.rdown;
      if(mouseMoveListener != NULL) {
        MouseEvent event = {
          MOUSEMOVE,
          mouse.x,
          mouse.y,
          mouse.down,
          mouse.ldown,
          mouse.mdown,
          mouse.rdown,
          mouse.wdown,
          mouse.which
        };
  			(*mouseMoveListener)(*this,event);
      }
      break;
    }
    case SDL_KEYUP: {
      const uint8_t* state = SDL_GetKeyboardState(NULL);
      keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),state,static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      if(keyUpListener != NULL) {
        KeyboardEvent event = {
          KEYUP,e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),state,static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)
        };
  			(*keyUpListener)(*this,event);
      }
      break;
    }
    case SDL_KEYDOWN: {
      const uint8_t* state = SDL_GetKeyboardState(NULL);
      keyboard = {e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),state,static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
      if(keyDownListener != NULL) {
        KeyboardEvent event = {
          KEYDOWN,e->key.keysym.scancode,SDL_GetKeyName(e->key.keysym.sym),state,static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)
        };
  			(*keyDownListener)(*this,event);
      }
      break;
    }
    case SDL_MOUSEWHEEL: {
      if(mouseWheelListener != NULL) {
        WheelEvent event = {
          MOUSEWHEEL,
          e->wheel.x,
          e->wheel.y,
          static_cast<bool>(e->wheel.direction)
        };
  			(*mouseWheelListener)(*this,event);
      }
      break;
    }
    case SDL_DROPFILE: {
      if(dropFileListener != NULL) {
        FileEvent event = {
          DROPFILE,
          e->drop.file,
          e->drop.timestamp
        };
  			(*dropFileListener)(*this,event);
      }
      break;
    }
  }
}

void Argon::addWindowListener(EventType type, WindowListener listener) {
  switch(type) {
    case LOAD: loadListener = &listener; break;
    case QUIT: quitListener = &listener; break;
    case CLOSE: closeListener = &listener; break;
    case SHOWN: shownListener = &listener; break;
    case HIDDEN: hiddenListener = &listener; break;
    case EXPOSED: exposedListener = &listener; break;
    case MOVED: movedListener = &listener; break;
    case RESIZE: resizeListener = &listener; break;
    case SIZECHANGED: sizeChangedListener = &listener; break;
    case MINIMIZED: minimizedListener = &listener; break;
    case MAXIMIZED: maximizedListener = &listener; break;
    case RESTORED: restoredListener = &listener; break;
    case FOCUS: focusListener = &listener; break;
    case BLUR: blurListener = &listener; break;
    case TAKEFOCUS: takeFocusListener = &listener; break;
    case HITTEST: hitTestListener = &listener; break;
    default: break;
  }
}
void Argon::addMouseListener(EventType type, MouseListener listener) {
  switch(type) {
    case MOUSEENTER: mouseEnterListener = &listener; break;
    case MOUSELEAVE: mouseLeaveListener = &listener; break;
    case MOUSEUP: mouseUpListener = &listener; break;
    case MOUSEDOWN: mouseDownListener = &listener; break;
    case MOUSEMOVE: mouseMoveListener = &listener; break;
    case CLICK: clickListener = &listener; break;
    case DBLCLICK: dblclickListener = &listener; break;
    default: break;
  }}
void Argon::addKeyboardListener(EventType type, KeyboardListener listener) {
  switch(type) {
    case KEYUP: keyUpListener = &listener; break;
    case KEYDOWN: keyDownListener = &listener; break;
    default: break;
  }
}
void Argon::addWheelListener(EventType type, WheelListener listener) {
  switch(type) {
    case MOUSEWHEEL: mouseWheelListener = &listener; break;
    default: break;
  }
}
void Argon::addFileListener(EventType type, FileListener listener) {
  switch(type) {
    case DROPFILE: dropFileListener = &listener; break;
    default: break;
  }
}

void Argon::removeListener(EventType type) {
  switch(type) {
    case LOAD: loadListener = NULL; break;
    case QUIT: quitListener = NULL; break;
    case CLOSE: closeListener = NULL; break;
    case SHOWN: shownListener = NULL; break;
    case HIDDEN: hiddenListener = NULL; break;
    case EXPOSED: exposedListener = NULL; break;
    case MOVED: movedListener = NULL; break;
    case RESIZE: resizeListener = NULL; break;
    case SIZECHANGED: sizeChangedListener = NULL; break;
    case MINIMIZED: minimizedListener = NULL; break;
    case MAXIMIZED: maximizedListener = NULL; break;
    case RESTORED: restoredListener = NULL; break;
    case FOCUS: focusListener = NULL; break;
    case BLUR: blurListener = NULL; break;
    case TAKEFOCUS: takeFocusListener = NULL; break;
    case HITTEST: hitTestListener = NULL; break;
    case MOUSEENTER: mouseEnterListener = NULL; break;
    case MOUSELEAVE: mouseLeaveListener = NULL; break;
    case MOUSEUP: mouseUpListener = NULL; break;
    case MOUSEDOWN: mouseDownListener = NULL; break;
    case MOUSEMOVE: mouseMoveListener = NULL; break;
    case CLICK: clickListener = NULL; break;
    case DBLCLICK: dblclickListener = NULL; break;
    case KEYUP: keyUpListener = NULL; break;
    case KEYDOWN: keyDownListener = NULL; break;
    case MOUSEWHEEL: mouseWheelListener = NULL; break;
    case DROPFILE: dropFileListener = NULL; break;
  }
}

void Argon::addLoop(Task task) {
  mainLoop = &task;
}
void Argon::removeLoop() {
  mainLoop = NULL;
}

void Argon::setFps(int _fps) {
  fps = _fps;
  frameTime = 1000 / fps;
}
int Argon::getFps() {
  return fps;
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

void Argon::quadraticBezier(int x1, int y1, int cpx, int cpy, int x2, int y2) {
  //overestimate the number of values of t we need to use to make our curve
  int totalDelX = abs(cpx - x1) + abs(x2 - cpx);
  int totalDelY = abs(cpy - y1) + abs(y2 - cpy);
  int steps = totalDelX + totalDelY;

  //calculate how much t will change by for each step
  double delT = double(1)/double(steps);

  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    //draw curve
    int prevX = x1;
    int prevY = y1;
    for(double t = 0; t <= 1; t += delT) {
      int nextX = int(round(cpx + (1-t)*(1-t)*(x1-cpx) + t*t*(x2-cpx)));
      int nextY = int(round(cpy + (1-t)*(1-t)*(y1-cpy) + t*t*(y2-cpy)));
      SDL_RenderDrawLine(a.ren, prevX, prevY, nextX, nextY);
      prevX = nextX;
      prevY = nextY;
    }
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::cubicBezier(int x1, int y1, int cpx1, int cpy1, int cpx2, int cpy2, int x2, int y2) {
  //overestimate the number of values of t we need to use to make our curve
  int totalDelX = abs(x1 - cpx1) + abs(cpx1 - cpx2) + abs(x2 - cpx2);
  int totalDelY = abs(y1 - cpy1) + abs(cpy1 - cpy2) + abs(y2 - cpy2);
  int steps = totalDelX + totalDelY;

  //calculate how much t will change by for each step
  double delT = double(1)/double(steps);

  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    //draw curve
    int prevX = x1;
    int prevY = y1;
    for(double t = 0; t <= 1; t += delT) {
      int nextX = int(round((1 - t)*(1 - t)*(1 - t)*x1 + 3*(1 - t)*(1 - t)*(t*cpx1) + 3*(1 - t)*t*t*cpx2 + t*t*t*x2));
      int nextY = int(round((1 - t)*(1 - t)*(1 - t)*y1 + 3*(1 - t)*(1 - t)*(t*cpy1) + 3*(1 - t)*t*t*cpy2 + t*t*t*y2));
      SDL_RenderDrawLine(a.ren, prevX, prevY, nextX, nextY);
      prevX = nextX;
      prevY = nextY;
    }
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::nicBezier(Points& pts) {
  int n = pts.size() - 1;
  if(n <= 0) {
    return;
  }
  //overestimate the number of values of t we need to use to make our curve
  int totalDelX = abs(pts.back()->x - pts.at(0)->x);
  int totalDelY = abs(pts.back()->y - pts.at(0)->y);
  for(auto p = 0; p < n; ++p) {
    totalDelX += abs(pts.at(p+1)->x - pts.at(p)->x);
    totalDelY += abs(pts.at(p+1)->y - pts.at(p)->y);
  }
  int steps = totalDelX + totalDelY;

  //calculate how much t will change by for each step
  double delT = double(1)/double(steps);

  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    //draw curve
    int prevX = pts.at(0)->x;
    int prevY = pts.at(0)->y;
    for(double t = 0; t <= 1; t += delT) {
      double nextX = 0;
      double nextY = 0;
      for(int i = 0; i <= n; ++i) {
        double nCi = tgamma(double(n+1))/(tgamma(double(i+1))*tgamma(double(n-i+1)));
        double t_a = pow(1-t, double(n-i));
        double t_b = pow(t, double(i));
        nextX += nCi*t_a*t_b*pts.at(i)->x;
        nextY += nCi*t_a*t_b*pts.at(i)->y;
      }
      SDL_RenderDrawLine(a.ren, prevX, prevY, int(round(nextX)), int(round(nextY)));
      prevX = int(round(nextX));
      prevY = int(round(nextY));
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

void Argon::strokeTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.fill.r, a.fill.g, a.fill.b, a.fill.a);
    SDL_RenderDrawLine(a.ren, x1, y1, x2, y2);
    SDL_RenderDrawLine(a.ren, x2, y2, x3, y3);
    SDL_RenderDrawLine(a.ren, x3, y3, x1, y1);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
//maybe refactor with point structs???
void Argon::scanLineTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  //sort points by y
  if(y1 > y2) {
    int tmpY = y1;
    int tmpX = x1;
    y1 = y2;
    x1 = x2;
    y2 = tmpY;
    x2 = tmpX;
  }
  if(y2 > y3) {
    int tmpY = y2;
    int tmpX = x2;
    y2 = y3;
    x2 = x3;
    y3 = tmpY;
    x3 = tmpX;
  }
  if(y1 > y2) {
    int tmpY = y1;
    int tmpX = x1;
    y1 = y2;
    x1 = x2;
    y2 = tmpY;
    x2 = tmpX;
  }

  //get change in x for each increment in y for each side of the triangle
  double dx1 = 0;
  double dx2 = 0;
  double dx3 = 0;
  if(y2 - y1 != 0) { //make sure points aren't at same y value
    dx1 = double(x2-x1)/(y2-y1);
  }
  if(y3 - y1 != 0) { //make sure points aren't at same y value
    dx2 = double(x3-x1)/(y3-y1);
  }
  if(y3 - y2 != 0) { //make sure points aren't at same y value
    dx3 = double(x3-x2)/(y3-y2);
  }

  //begin scanline
  function<void(Argon&)> f = ([=](Argon& a) {
    double sx = double(x1);
    double sy = double(y1);
    double ex = double(sx);
    double ey = double(sy);
    SDL_SetRenderDrawColor(a.ren, a.fill.r, a.fill.g, a.fill.b, a.fill.a);
    for(; sy < y2; ++sy, ++ey, sx += dx1, ex += dx2) {
      SDL_RenderDrawLine(a.ren, int(sx), int(sy), int(ex), int(ey));
    }
    for(; sy < y3; ++sy, ++ey, sx += dx3, ex += dx2) {
      SDL_RenderDrawLine(a.ren, int(sx), int(sy), int(ex), int(ey));
    }
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::halfSpaceTriangle(int x1, int y1, int x2, int y2, int x3, int y3) { //TODO: FIX THIS!!!!!
  //Reading:
  /*
  https://fgiesen.wordpress.com/2013/02/17/optimizing-sw-occlusion-culling-index/
  https://www.uni-obudna.hu/journal/Mileff_Nehez_Dudra_63.pdf
  */

  //get bounding box
  int xMin = min(min(x1, x2), x3);
  int xMax = max(max(x1, x2), x3);
  int yMin = min(min(y1, y2), y3);
  int yMax = max(max(y1, y2), y3);
  //clip bounding box if it goes off screen
  xMin = max(xMin, 0);
  xMax = min(xMax, window.w);
  yMin = max(yMin, 0);
  yMax = min(yMax, window.h);

  //strokeRect(xMin, yMin, xMax-xMin, yMax-yMin);

  //begin lambda
  function<void(Argon&)> f = ([=](Argon& a) {
    //loop through pixels in bounding box
    for(int i = xMin; i <= xMax; ++i) {
      for(int j = yMin; j <= xMax; ++j) {
        int c1 = (x3-x1)*(j-y1) - (y3-y1)*(i-x1);
        int c2 = (x3-x2)*(j-y2) - (y3-y2)*(i-x2);
        int c3 = (x2-x3)*(j-y3) - (y2-y3)*(i-x3);
        if((c1 >=  0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0)) {
          SDL_SetRenderDrawColor(a.ren, a.fill.r, a.fill.g, a.fill.b, a.fill.a);
          SDL_RenderDrawPoint(a.ren, i, j);
        }
      }
    }
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
  scanLineTriangle(x1, y1, x2, y2, x3, y3); //half space algorithm is not yet functional
}

void Argon::strokeRect(int x, int y, int w, int h) {
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.fill.r, a.fill.g, a.fill.b, a.fill.a);
    SDL_RenderDrawLine(a.ren, x, y, x+w, y);
    SDL_RenderDrawLine(a.ren, x+w, y, x+w, y+h);
    SDL_RenderDrawLine(a.ren, x, y+h, x+w, y+h);
    SDL_RenderDrawLine(a.ren, x, y, x, y+h);
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

void Argon::strokePolygon(Points& points) {
  if(points.size() < 3) {return;}
  function<void(Argon&)> f = ([=](Argon& a) {
    SDL_SetRenderDrawColor(a.ren, a.stroke.r, a.stroke.g, a.stroke.b, a.stroke.a);
    for(auto it = points.begin()+1;it != points.end();++it) {
      a.line((*(it-1))->x,(*(it-1))->y,(*it)->x,(*it)->y);
    }
    a.line(points.back()->x,points.back()->y, points.front()->x,points.front()->y);
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::polygon(Points& points) {
  //make polygon
  Argon_Polygon poly(points);
  Argon_Rect drawRect = poly.boundRect.boundOnScreen(window.w, window.h);
  EdgeTable drawable;
  for(auto e : poly.edges) {
    if(e->min <= drawRect.y+drawRect.h) {
      drawable.push_back(e);
    }
  }
  //lambda
  function<void(Argon&)> f = ([=](Argon& a) {
    EdgeTable active;
    for(auto e : drawable) {
      if(e->min < drawRect.y) {
        active.push_back(e);
      }
    }

    for(int y = 0; y <= drawRect.h; ++y) {

      for(auto it = active.begin(); it < active.end(); ++it) {
        if((*it)->max == y+drawRect.y) {
          active.erase(it--);
        }
      }
      for(auto e : drawable) {
        if(e->min == y+drawRect.y) {
          active.push_back(e);
        }
      }
      sort(active.begin(),active.end(),[](Edge* e, Edge* f){return e->curX < f->curX;});

      SDL_SetRenderDrawColor(a.ren, a.fill.r, a.fill.g, a.fill.b, a.fill.a);
      for(int i = 0; i < active.size(); i+=2) {
        SDL_RenderDrawLine(a.ren, active[i]->curX, y+drawRect.y, active[i+1]->curX, y+drawRect.y);
      }

      for(auto e : active) {
        e->sum += abs(e->dx);
        while(e->sum >= abs(e->dy)) {
          e->curX += e->msign;
          e->sum -= abs(e->dy);
        }
      }
    }
  });
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
  //OTHER THING
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
Argon_Rect Argon::image(const char* path, int sx, int sy, int sw, int sh, int dx, int dy,int dw, int dh) {
  auto it = find_if(imageCache.begin(),imageCache.end(), [=](const CachedImage* obj) {
    return strcmp(obj->path, path) == 0;
  });
  if(it == imageCache.end()) {
    CachedImage* img = new CachedImage(*this,path);
    if(!img->loaded) {return {-1,-1,-1,-1};}
    imageCache.push_back(img);
    return img->draw(sx,sy,sw,sh,dx,dy,dw,dh);
  }
  else {
    return (*it)->draw(sx,sy,sw,sh,dx,dy,dw,dh);
  }
}
Argon_Rect Argon::image(const char* path,int x, int y, int w, int h) {
  return image(path,0,0,-1,-1,x,y,w,h);
}
Argon_Rect Argon::image(const char* path,int x, int y) {
  return image(path,0,0,-1,-1,x,y,-1,-1);
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

// UHOH so here is a problem because you cant return a value if you make this
// function a lambda so what should we do? just not have a lambda?
// or you clould make the usder pass a callback lambda to getPixel
Argon_Color Argon::getPixel(int x, int y) {
  Argon_Color c;
  SDL_PixelFormat* fmt = surface->format;
  SDL_LockSurface(surface);
  uint32_t pixel = ((uint32_t*)surface->pixels)[y*(surface->pitch/fmt->BytesPerPixel) + x];
  SDL_UnlockSurface(surface);
  SDL_GetRGBA(pixel,fmt,&c.r,&c.g,&c.b,&c.a);
  return c;
}
void Argon::putRGB(uint8_t* pixels, int x, int y, int w, int h) {
  function<void(Argon&)> f = [=](Argon& a) {
    int width = w == -1 ? a.window.w : w;
    int height = h == -1 ? a.window.h : h;
    SDL_Texture* img = SDL_CreateTexture(a.ren,SDL_PIXELFORMAT_RGB24,SDL_TEXTUREACCESS_STATIC,width,height);
    SDL_UpdateTexture(img,NULL,&pixels[0],width * 3);
    SDL_Rect dst = {x,y,width,height};
    SDL_RenderCopy(a.ren,img,NULL,&dst);
    SDL_DestroyTexture(img);
  };
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}
void Argon::putRGBA(uint8_t* pixels, int x, int y, int w, int h) {
  function<void(Argon&)> f = [=](Argon& a) {
    int width = w == -1 ? a.window.w : w;
    int height = h == -1 ? a.window.h : h;
    SDL_Texture* img = SDL_CreateTexture(a.ren,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,width,height);
    SDL_UpdateTexture(img,NULL,&pixels[0],width * 4);
    SDL_Rect dst = {x,y,width,height};
    SDL_RenderCopy(a.ren,img,NULL,&dst);
    SDL_DestroyTexture(img);
  };
  if(skipCallstack) {f(*this);}
  else {callstack.push_back(f);}
}




//Friend Classes


//Cached Image
#include <chrono>
using namespace std::chrono;
auto start = high_resolution_clock::now();
// auto stop = high_resolution_clock::now();
// auto duration = duration_cast<microseconds>(stop - start);
// duration.count();

CachedImage::CachedImage(Argon& a, const char* path) : argon(a), path(path) {
  SDL_Surface* initial = IMG_Load(path);
  if(initial == NULL) {
    argon.lastError = "Unable to optimize image " + string(path) + "! SDL Error: " + SDL_GetError();
    cerr << "\e[31m[ERROR] Failed to load " << path << "\e[0m" << endl;
    loaded = false;
    return;
  }
  img = SDL_CreateTextureFromSurface(a.ren,initial);
  SDL_QueryTexture(img, NULL, NULL, &w, &h);
  SDL_FreeSurface(initial);
  loaded = true;
};
CachedImage::~CachedImage() {
  remove();
};
Argon_Rect CachedImage::draw(int sx, int sy, int sw, int sh, int dx, int dy,int dw, int dh) {
  if(sw < 0) {sw = w;}
  if(sh < 0) {sh = h;}
  if(dw < 0) {dw = w;}
  if(dh < 0) {dh = h;}
  SDL_Rect src = {sx,sy,sw,sh};
  SDL_Rect dest = {dx,dy,dw,dh};
  SDL_RenderCopy(argon.ren,img,&src,&dest);
  return {dx,dy,dw,dh};
}

void CachedImage::remove() {
  auto it = find(argon.imageCache.begin(),argon.imageCache.end(),this);
  if(it != argon.imageCache.end()) {
    argon.imageCache.erase(it);
  }
  SDL_DestroyTexture(img);
}
