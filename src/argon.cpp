#include "argon.h"

//CONSTRUCTORS / DESTRUCTORS
Argon::Argon() : Argon("Argon", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,60,ARGON_BASIC) {};
Argon::Argon(const char* name) : Argon(name, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,60,ARGON_BASIC) {};
Argon::Argon(const char* name, int flags) : Argon(name, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,60,flags) {};
Argon::Argon(const char* name, int fps, int flags) : Argon(name, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,fps,flags) {};
Argon::Argon(const char* name, int w, int h, int flags) : Argon(name, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,60,flags) {};
Argon::Argon(const char* name, int w, int h, int fps, int flags) : Argon(name, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,fps,flags) {};
Argon::Argon(const char* name, int x, int y, int w, int h, int fps, int flags) : name(name) {
  if(flags & 0x1) {sdl_flags |= SDL_WINDOW_FULLSCREEN;}
  if((flags >> 1) & 0x1) {sdl_flags |= SDL_WINDOW_HIDDEN;}
  if((flags >> 2) & 0x1) {sdl_flags |= SDL_WINDOW_BORDERLESS;}
  if((flags >> 3) & 0x1) {sdl_flags |= SDL_WINDOW_RESIZABLE;}
  if((flags >> 4) & 0x1) {sdl_flags |= SDL_WINDOW_MINIMIZED;}
  if((flags >> 5) & 0x1) {sdl_flags |= SDL_WINDOW_MAXIMIZED;}
  if((flags >> 6) & 0x1) {sdl_flags |= SDL_WINDOW_ALLOW_HIGHDPI;}
  if((flags >> 7) & 0x1) {quitOnClose = false;}

  SDL_InitSubSystem(SDL_INIT_VIDEO);
  if((flags >> 8) & 0x1) {
    int img_flags = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;
    if((IMG_Init(img_flags) & img_flags) != img_flags) {
      printf("Failed to init Image Library\n");
    }
    else {
      imagesEnabled = true;
    }
  }
  if((flags >> 9) & 0x1) {
    if(TTF_Init() < 0) {
    	printf("Failed to init Text Library\n");
    }
  }
  win = SDL_CreateWindow(name,x,y,w,h,sdl_flags);
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  surface = SDL_GetWindowSurface(win);
  SDL_SetWindowTitle(win,name);
  SDL_AddEventWatch(resizeWatcher, this);
  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  keys.states = SDL_GetKeyboardState(NULL);
  running = true;
  setFPS(fps);
}
Argon::~Argon() {
  quit();
};

//WINDOW FUNCTIONS
void Argon::start() {
  SDL_GetRendererOutputSize(ren, &w, &h);
  SDL_GetWindowPosition(win,&x,&y);
  if(loadListener != NULL) {
    WindowEvent event = {LOAD,x,y,w,h,shown};
    loadListener(*this, event);
  }
  gameLoop();
}
void Argon::resume() {
  win = SDL_CreateWindow(name,x,y,w,h,sdl_flags);
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  surface = SDL_GetWindowSurface(win);
  SDL_SetWindowTitle(win,name);
  running = true;
}
void Argon::close() {
	if(quitOnClose) {quit();}
	else {
		running = false;
		SDL_DestroyWindow(win);
	  SDL_DestroyRenderer(ren);
	  SDL_DestroyWindow(win);
	}
}
void Argon::quit() {
  // if(TTF_WasInit() == 1) {
  //   for(auto f : fonts) {
  //     TTF_CloseFont(f);
  //   }
  //   TTF_Quit();
  // }
  running = false;
  SDL_DelEventWatch(resizeWatcher,this);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
}
void Argon::setIcon(const char* path) {
  if(imagesEnabled) {
    SDL_Surface* surface = IMG_Load(path);
    if(surface == NULL) {
      printf("\e[31m[ERROR]  Failed to load %s\e[0m\n", path);
    }
    else {
      SDL_SetWindowIcon(win,surface);
      SDL_FreeSurface(surface);
    }
  }
  else {
    printf("\e[31m[ERROR]  Images not enabled, please use ARGON_IMAGES flag\e[0m\n");
  }
}

void Argon::grabMouse(bool grab) {
	SDL_SetRelativeMouseMode(grab ? SDL_TRUE : SDL_FALSE);
}
void Argon::notification(const char* title, const char* msg, MSGBoxType type) {
	SDL_ShowSimpleMessageBox(type,title,msg,win);
}
int Argon::prompt(const char* title, const char* msg, int numBtns, Argon_Button* buttons, MSGBoxType type) {
	SDL_MessageBoxButtonData* btns = (SDL_MessageBoxButtonData*)buttons;
  const SDL_MessageBoxData data = {type, win, title, msg, numBtns, btns, NULL};
  int id;
  SDL_ShowMessageBox(&data, &id);
  return id;
}

//LOOP FUNCTIONS
void Argon::gameLoop() {
	SDL_Event e;
	uint32_t time = SDL_GetTicks();
  while(running) {
    uint32_t now = SDL_GetTicks();
    int maxBehind = 10;
    if(time <= now) {
      while(time <= now  && (maxBehind--)) {
        if(mainLoop != NULL) {
          mainLoop(*this);
        }
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
void Argon::setLoop(Task loop) {
	mainLoop = loop;
}
void Argon::removeLoop() {
	mainLoop = NULL;
}
void Argon::setFPS(int fps) {
	frameTime = 1000 / fps;
}

//EVENT HANDLING
int Argon::resizeWatcher(void* data, SDL_Event* e) {
  Argon* a = (Argon*)data;
  if(e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_RESIZED) {
    SDL_GetRendererOutputSize(a->ren, &a->w, &a->h);
    if(a->resizeListener != NULL) {
      WindowEvent event = {RESIZE,a->x,a->y,a->w,a->h,a->shown};
      a->resizeListener(*a,event);
    }
  }
  return 0;
}
void Argon::eventWatcher(SDL_Event* e) {
	switch(e->type) {
		case SDL_QUIT: {
      shown = false;
      if(quitListener != NULL) {
        WindowEvent event = {QUIT,x,y,w,h,shown};
  			quitListener(*this,event);
      }
      quit();
			break;
		}
		case SDL_WINDOWEVENT: {
			switch(e->window.event) {
				case SDL_WINDOWEVENT_CLOSE: {
          shown = false;
          if(closeListener != NULL) {
            WindowEvent event = {CLOSE,x,y,w,h,shown};
  					closeListener(*this,event);
          }
          close();
          break;
        }
        case SDL_WINDOWEVENT_SHOWN: {
          shown = true;
          if(shownListener != NULL) {
          	WindowEvent event = {SHOWN,x,y,w,h,shown};
  					shownListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_HIDDEN: {
          shown = false;
          if(hiddenListener != NULL) {
            WindowEvent event = {HIDDEN,x,y,w,h,shown};
  					hiddenListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_EXPOSED: {
          shown = true;
          if(exposedListener != NULL) {
            WindowEvent event = {EXPOSED,x,y,w,h,shown};
  					exposedListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_MOVED: {
          x = e->window.data1;
          y = e->window.data2;
          if(movedListener != NULL) {
            WindowEvent event = {MOVED,x,y,w,h,shown};
  					movedListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_SIZE_CHANGED: {
          SDL_GetRendererOutputSize(ren, &w, &h);
          if(sizeChangedListener != NULL) {
            WindowEvent event = {SIZECHANGED,x,y,w,h,shown};
  					sizeChangedListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_MINIMIZED: {
          shown = false;
          if(minimizedListener != NULL) {
            WindowEvent event = {MINIMIZED,x,y,w,h,shown};
  					minimizedListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_MAXIMIZED: {
          shown = true;
          if(maximizedListener != NULL) {
            WindowEvent event = {MAXIMIZED,x,y,w,h,shown};
  					maximizedListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_RESTORED: {
        	shown = true;
          if(restoredListener != NULL) {
            WindowEvent event = {RESTORED,x,y,w,h,shown};
  					restoredListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_GAINED: {
          if(focusListener != NULL) {
            WindowEvent event = {FOCUS,x,y,w,h,shown};
  					focusListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_FOCUS_LOST: {
          if(blurListener != NULL) {
            WindowEvent event = {BLUR,x,y,w,h,shown};
  					blurListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_TAKE_FOCUS: {
          if(takeFocusListener != NULL) {
            WindowEvent event = {TAKEFOCUS,x,y,w,h,shown};
  					takeFocusListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_HIT_TEST: {
          if(hitTestListener != NULL) {
            WindowEvent event = {HITTEST,x,y,w,h,shown};
  					hitTestListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_ENTER: {
          uint32_t state = SDL_GetGlobalMouseState(&mouse.x, &mouse.y);
          mouse.x -= x;
          mouse.y -= y;
          mouse.ldown = state&SDL_PRESSED;
          mouse.mdown = state&(SDL_PRESSED<<1);
          mouse.rdown = state&(SDL_PRESSED<<2);
          mouse.wdown = state&(SDL_PRESSED<<4);
          mouse.down = mouse.ldown||mouse.mdown||mouse.rdown;
          mouse.which = e->button.button;
          if(mouseEnterListener != NULL) {
            MouseEvent event = {MOUSEENTER,mouse.x,mouse.y,mouse.down,mouse.ldown,mouse.mdown,mouse.rdown,mouse.wdown,mouse.which};
  					mouseEnterListener(*this,event);
          }
          break;
        }
        case SDL_WINDOWEVENT_LEAVE: {
          uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
          mouse.x -= x;
          mouse.y -= y;
          mouse.ldown = state&SDL_PRESSED;
          mouse.mdown = state&(SDL_PRESSED<<1);
          mouse.rdown = state&(SDL_PRESSED<<2);
          mouse.wdown = state&(SDL_PRESSED<<4);
          mouse.down = mouse.ldown||mouse.mdown||mouse.rdown;
          mouse.which = e->button.button;
          if(mouseLeaveListener != NULL) {
            MouseEvent event = {MOUSELEAVE,mouse.x,mouse.y,mouse.down,mouse.ldown,mouse.mdown,mouse.rdown,mouse.wdown,mouse.which};
  					mouseLeaveListener(*this,event);
          }
          break;
        }
      }
    }
    case SDL_MOUSEBUTTONUP: {
      if(e->button.which != 0) {return;} //Mouse Up gets triggered often for no reason with which non 0
      uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
      mouse.ldown = state&SDL_PRESSED;
      mouse.mdown = state&(SDL_PRESSED<<1);
      mouse.rdown = state&(SDL_PRESSED<<2);
      mouse.wdown = state&(SDL_PRESSED<<4);
      mouse.down = mouse.ldown||mouse.mdown||mouse.rdown;
      mouse.which = e->button.button;
      if(mouseUpListener != NULL) {
        MouseEvent event = {MOUSEUP,mouse.x,mouse.y,mouse.down,mouse.ldown,mouse.mdown,mouse.rdown,mouse.wdown,mouse.which};
  			mouseUpListener(*this,event);
      }
      if(e->button.clicks > 0 && clickListener != NULL) {
          MouseEvent event = {CLICK,mouse.x,mouse.y,mouse.down,mouse.ldown,mouse.mdown,mouse.rdown,mouse.wdown,mouse.which};
  				clickListener(*this,event);
      }
      if(e->button.clicks > 1 && dblclickListener != NULL) {
        MouseEvent event = {DBLCLICK,mouse.x,mouse.y,mouse.down,mouse.ldown,mouse.mdown,mouse.rdown,mouse.wdown,mouse.which};
        dblclickListener(*this,event);
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
      mouse.ldown = state&SDL_PRESSED;
      mouse.mdown = state&(SDL_PRESSED<<1);
      mouse.rdown = state&(SDL_PRESSED<<2);
      mouse.wdown = state&(SDL_PRESSED<<4);
      mouse.down = mouse.ldown||mouse.mdown||mouse.rdown;
      mouse.which = e->button.button;
      if(mouseDownListener != NULL) {
        MouseEvent event = {MOUSEDOWN,mouse.x,mouse.y,mouse.down,mouse.ldown,mouse.mdown,mouse.rdown,mouse.wdown,mouse.which};
  			mouseDownListener(*this,event);
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      uint32_t state = SDL_GetMouseState(&mouse.x, &mouse.y);
      mouse.ldown = state&SDL_PRESSED;
      mouse.mdown = state&(SDL_PRESSED<<1);
      mouse.rdown = state&(SDL_PRESSED<<2);
      mouse.wdown = state&(SDL_PRESSED<<4);
      mouse.down = mouse.ldown||mouse.mdown||mouse.rdown;
      mouse.which = e->button.button;
      if(mouseMoveListener != NULL) {
        MouseEvent event = {MOUSEMOVE,mouse.x,mouse.y,mouse.down,mouse.ldown,mouse.mdown,mouse.rdown,mouse.wdown,mouse.which};
  			mouseMoveListener(*this,event);
      }
      break;
    }
    case SDL_KEYUP: {
      if(keyUpListener != NULL) {
        Argon_Key key(SDL_GetKeyName(e->key.keysym.sym),e->key.keysym.sym);
        KeyboardEvent event = {KEYDOWN,key,static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
  			keyUpListener(*this,event);
      }
      break;
    }
    case SDL_KEYDOWN: {
      if(keyDownListener != NULL) {
        Argon_Key key(SDL_GetKeyName(e->key.keysym.sym),e->key.keysym.sym);
        KeyboardEvent event = {KEYDOWN,key,static_cast<bool>(KMOD_LSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_RSHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_LCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_RCTRL & e->key.keysym.mod),static_cast<bool>(KMOD_LALT & e->key.keysym.mod),static_cast<bool>(KMOD_RALT & e->key.keysym.mod),static_cast<bool>(KMOD_LGUI & e->key.keysym.mod),static_cast<bool>(KMOD_RGUI & e->key.keysym.mod),static_cast<bool>(KMOD_NUM & e->key.keysym.mod),static_cast<bool>(KMOD_CAPS & e->key.keysym.mod),static_cast<bool>(KMOD_CTRL & e->key.keysym.mod),static_cast<bool>(KMOD_SHIFT & e->key.keysym.mod),static_cast<bool>(KMOD_ALT & e->key.keysym.mod),static_cast<bool>(KMOD_GUI & e->key.keysym.mod)};
  			keyDownListener(*this,event);
      }
      break;
    }
    case SDL_MOUSEWHEEL: {
      if(mouseWheelListener != NULL) {
        WheelEvent event = {MOUSEWHEEL,e->wheel.x,e->wheel.y,static_cast<bool>(e->wheel.direction)};
  			mouseWheelListener(*this,event);
      }
      break;
    }
    case SDL_DROPFILE: {
      if(dropFileListener != NULL) {
        FileEvent event = {DROPFILE,e->drop.file,e->drop.timestamp};
  			dropFileListener(*this,event);
      }
      break;
    }
	}
}
void Argon::setListener(EventType type, WindowListener listener) {
	switch(type) {
		case LOAD: loadListener = listener; break;
		case QUIT: quitListener = listener; break;
		case CLOSE: closeListener = listener; break;
		case SHOWN: shownListener = listener; break;
		case HIDDEN: hiddenListener = listener; break;
		case EXPOSED: exposedListener = listener; break;
		case MOVED: movedListener = listener; break;
		case RESIZE: resizeListener = listener; break;
		case SIZECHANGED: sizeChangedListener = listener; break;
		case MINIMIZED: minimizedListener = listener; break;
		case MAXIMIZED: maximizedListener = listener; break;
		case RESTORED: restoredListener = listener; break;
		case FOCUS: focusListener = listener; break;
		case BLUR: blurListener = listener; break;
		case TAKEFOCUS: takeFocusListener = listener; break;
		case HITTEST: hitTestListener = listener; break;
		default: printf("\e[31m[ERROR]  Invalid EventType (%s) for WindowListener\e[0m\n", getEventTypeName(type)); break;
	}
}
void Argon::setListener(EventType type, MouseListener listener) {
	switch(type) {
		case MOUSEENTER: mouseEnterListener = listener; break;
		case MOUSELEAVE: mouseLeaveListener = listener; break;
		case MOUSEUP: mouseUpListener = listener; break;
		case MOUSEDOWN: mouseDownListener = listener; break;
		case MOUSEMOVE: mouseMoveListener = listener; break;
		case CLICK: clickListener = listener; break;
		case DBLCLICK: dblclickListener = listener; break;
		default: printf("\e[31m[ERROR]  Invalid EventType (%s) for MouseListener\e[0m\n", getEventTypeName(type)); break;
	}
}
void Argon::setListener(EventType type, KeyboardListener listener) {
	switch(type) {
		case KEYUP: keyUpListener = listener; break;
		case KEYDOWN: keyDownListener = listener; break;
		default: printf("\e[31m[ERROR]  Invalid EventType (%s) for KeyboardListener\e[0m\n", getEventTypeName(type)); break;
	}
}
void Argon::setListener(EventType type, WheelListener listener) {
	switch(type) {
		case MOUSEWHEEL: mouseWheelListener = listener; break;
		default: printf("\e[31m[ERROR]  Invalid EventType (%s) for WheelListener\e[0m\n", getEventTypeName(type)); break;
	}
}
void Argon::setListener(EventType type, FileListener listener) {
	switch(type) {
		case DROPFILE: dropFileListener = listener; break;
		default: printf("\e[31m[ERROR]  Invalid EventType (%s) for FileListener\e[0m\n", getEventTypeName(type)); break;
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

//DRAWING FUNCTIONS
void Argon::clear() {
  SDL_SetRenderDrawColor(ren, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
  SDL_RenderClear(ren);
}
void Argon::point(int x, int y) {
	if(strokeWeight == 1) {
		if(!doStroke) {return;}
	  SDL_SetRenderDrawColor(ren, strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	  SDL_RenderDrawPoint(ren, x, y);
	}
	else {
    bool tmp = doStroke;
    doStroke = false;
		circle(x,y,strokeWeight/2);
    doStroke = tmp;
	}
}
void Argon::line(int x1, int y1, int x2, int y2) {
	if(!doStroke) {return;}
	if(strokeWeight == 1) {
	  SDL_SetRenderDrawColor(ren, strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
	  SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	}
	else {
		//CODE FOR THICK LINE
	}
}
void Argon::circle(int cx, int cy, int r) {
  int o = r + strokeWeight,
      i = r;
  Argon_Shape cir(*this,o<<1,o<<1);
  // int xo = o,
  //     xi = i;
  // int xoerr = 1-xo;
  // int xierr = 1-xi;
  // int y = 0;
  //
  //
  // while (xo >= y) {
  //   if(doFill) {
  //     if(xi >= y) {
  //       cir.hLine(o-y, o+y, o+xi, fillColor);
  //       cir.vLine(o-y, o-y, o+xi, fillColor);
  //       cir.hLine(o-y, o-y, o-xi, fillColor);
  //       cir.vLine(o+y, o-y, o+xi, fillColor);
  //       cir.hLine(o+y, o+y, o+xi, fillColor);
  //       cir.vLine(o-y, o+y, o-xi, fillColor);
  //       cir.hLine(o+y, o-y, o-xi, fillColor);
  //       cir.vLine(o+y, o+y, o-xi, fillColor);
  //     }
  //   }
  //   if(doStroke) {
  //     int start = xi > y ? xi : y;
  //     // cir.hLine(o-y, o+start, o+xo, strokeColor);
  //     // cir.vLine(o-y, o+start, o+xo, strokeColor);
  //     cir.hLine(o-y, o-start, o-xo, strokeColor);
  //     // cir.vLine(o+y, o+start, o+xo, strokeColor);
  //     // cir.hLine(o+y, o+start, o+xo, strokeColor);
  //     cir.vLine(o-y, o-start, o-xo, strokeColor);
  //     // cir.hLine(o+y, o-start, o-xo, strokeColor);
  //     // cir.vLine(o+y, o-start, o-xo, strokeColor);
  //   }
  //
  //
  //
  //   // int start = xi > y ? xi : y;
  //   // SDL_SetRenderDrawColor(ren,strokeColor.r,strokeColor.g,strokeColor.b,strokeColor.a);
  //   // SDL_RenderDrawLine(ren, cx+start, cy-y, cx+xo, cy-y);
  //   // SDL_RenderDrawLine(ren, cx-y, cy+start, cx-y, cy+xo);
  //   // SDL_RenderDrawLine(ren, cx-start, cy-y, cx-xo, cy-y);
  //   // SDL_RenderDrawLine(ren, cx+y, cy+start, cx+y, cy+xo);
  //   // SDL_RenderDrawLine(ren, cx+start, cy+y, cx+xo, cy+y);
  //   // SDL_RenderDrawLine(ren, cx-y, cy-start, cx-y, cy-xo);
  //   // SDL_RenderDrawLine(ren, cx-start, cy+y, cx-xo, cy+y);
  //   // SDL_RenderDrawLine(ren, cx+y, cy-start, cx+y, cy-xo);
  //   //
  //   // if(xi >= y) {
  //   //   SDL_SetRenderDrawColor(ren,fillColor.r,fillColor.g,fillColor.b,fillColor.a);
  //   //   SDL_RenderDrawLine(ren, cx+y, cy-y, cx+xi-1, cy-y);
  //   //   SDL_RenderDrawLine(ren, cx-y, cy+y, cx-y, cy+xi-1);
  //   //   SDL_RenderDrawLine(ren, cx-y, cy-y, cx-xi+1, cy-y);
  //   //   SDL_RenderDrawLine(ren, cx+y, cy+y, cx+y, cy+xi-1);
  //   //   SDL_RenderDrawLine(ren, cx+y, cy+y, cx+xi-1, cy+y);
  //   //   SDL_RenderDrawLine(ren, cx-y, cy-y, cx-y, cy-xi+1);
  //   //   SDL_RenderDrawLine(ren, cx-y, cy+y, cx-xi+1, cy+y);
  //   //   SDL_RenderDrawLine(ren, cx+y, cy-y, cx+y, cy-xi+1);
  //   // }
  //
  //   y++;
  //   if(xoerr < 0) {
  //     xoerr += (y<<1) + 1;
  //   }
  //   else {
  //     xo--;
  //     xoerr += (y - xo + 1)<<1;
  //   }
  //   if(xierr < 0) {
  //     xierr += (y<<1) + 1;
  //   }
  //   else {
  //     xi--;
  //     xierr += (y - xi + 1)<<1;
  //   }
  //
  // }
  cir.updateTexture();
  cir.render(cx-o,cy-o,o<<1,o<<1);

  int xo = strokeWeight+r, xi = r;
  int y = 0;
  int erro = 1 - xo, erri = 1 - xi;
  while (xo >= y) {
  	if(doFill) {
  		SDL_SetRenderDrawColor(ren,fillColor.r,fillColor.g,fillColor.b,fillColor.a);
	    SDL_RenderDrawLine(ren, cx-xi, cy+y, cx+xi, cy+y);
	    SDL_RenderDrawLine(ren, cx-y, cy+xi, cx+y, cy+xi);
	    SDL_RenderDrawLine(ren, cx-xi, cy-y, cx+xi, cy-y);
	    SDL_RenderDrawLine(ren, cx-y, cy-xi, cx+y, cy-xi);
  	}
  	if(doStroke) {
  		SDL_SetRenderDrawColor(ren,strokeColor.r,strokeColor.g,strokeColor.b,strokeColor.a);
     SDL_RenderDrawLine(ren, cx+xi, cy+y, cx+xo, cy+y);
     SDL_RenderDrawLine(ren, cx+y, cy+xi, cx+y, cy+xo);
			SDL_RenderDrawLine(ren, cx-xo, cy+y, cx-xi, cy+y);
			SDL_RenderDrawLine(ren, cx-y, cy+xi, cx-y, cy+xo);
			SDL_RenderDrawLine(ren, cx-xo, cy-y, cx-xi, cy-y);
			SDL_RenderDrawLine(ren, cx-y, cy-xo, cx-y, cy-xi);
			SDL_RenderDrawLine(ren, cx+xi, cy-y, cx+xo, cy-y);
			SDL_RenderDrawLine(ren, cx+y, cy-xo, cx+y, cy-xi);
	  }
    y++;
    if(erro < 0) {
      erro += 2 * y + 1;
    }
    else {
      xo--;
      erro += 2 * (y - xo + 1);
    }
    if(y > r) {
      xi = y;
    }
    else {
      if(erri < 0) {
        erri += 2 * y + 1;
      }
      else {
        xi--;
        erri += 2 * (y - xi + 1);
      }
    }
  }
}

Argon_RGB Argon::getPixel(int x, int y) {
  Argon_RGB c;
  SDL_LockSurface(surface);
  uint32_t pixel = ((uint32_t*)surface->pixels)[y*(surface->pitch/surface->format->BytesPerPixel) + x];
  SDL_UnlockSurface(surface);
  SDL_GetRGBA(pixel,surface->format,&c.r,&c.g,&c.b,&c.a);
  SDL_UnlockSurface(surface);
  return c;
}
void Argon::putPixel(int x, int y, Argon_RGB& c) {
  SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
  SDL_RenderDrawPoint(ren, x, y);
}
void Argon::putPixelData(uint8_t* data, Argon_Rect* rect) {
	SDL_Texture* img = SDL_CreateTexture(ren,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,w,h);
	if(rect == NULL) {
	  SDL_UpdateTexture(img,NULL,data,w * 4);
	  SDL_RenderCopy(ren,img,NULL,NULL);
	}
	else {
		SDL_Rect* r = (SDL_Rect*)rect;
	  SDL_UpdateTexture(img,r,data,r->w * 4);
	  SDL_RenderCopy(ren,img,r,r);
	}
  SDL_DestroyTexture(img);
}
void Argon::getPixelData(uint8_t* data, Argon_Rect* rect) {
	if(rect == NULL) {
		SDL_RenderReadPixels(ren, NULL, SDL_PIXELFORMAT_RGBA32, data, w*4);
	}
	else {
    SDL_Rect* r = (SDL_Rect*)rect;
		SDL_RenderReadPixels(ren, r, SDL_PIXELFORMAT_RGBA32, data, r->w*4);
	}
}
void Argon::getPixelData(uint8_t* data, int x, int y, int w, int h) {
	Argon_Rect r = {x,y,w,h};
	getPixelData(data,&r);
}
void Argon::putPixelData(uint8_t* data, int x, int y, int w, int h) {
	Argon_Rect r = {x,y,w,h};
	putPixelData(data,&r);
}

//DRAWING SETTINGS
void Argon::fill() {doFill = true;}
void Argon::noFill() {doFill = false;}
void Argon::stroke() {doStroke = true;}
void Argon::noStroke() {doStroke = false;}
void Argon::setFill(uint8_t r,uint8_t g,uint8_t b,uint8_t a) {
	fillColor.r = r;
	fillColor.g = g;
	fillColor.b = b;
	fillColor.a = a;
}
void Argon::setFill(Argon_RGB& color) {
	memcpy(&bgColor, &color, sizeof(color));
}
void Argon::setStroke(uint8_t r,uint8_t g,uint8_t b,uint8_t a) {
	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = a;
}
void Argon::setStroke(Argon_RGB& color) {
	memcpy(&bgColor, &color, sizeof(color));
}
void Argon::setBackground(uint8_t r,uint8_t g,uint8_t b,uint8_t a) {
	bgColor.r = r;
	bgColor.g = g;
	bgColor.b = b;
	bgColor.a = a;
}
void Argon::setBackground(Argon_RGB& color) {
	memcpy(&bgColor, &color, sizeof(color));
}

void Argon::setStrokeWeight(uint8_t weight) {
	if(weight < 1) {
		printf("\e[31m[ERROR]  Stroke Weight cannot be less than one\e[0m\n");
	}
	else {
		strokeWeight = weight;
	}
}

//HELPER FUNCTIONS
const char* Argon::getEventTypeName(EventType type) {
	switch(type) {
		case LOAD: return "LOAD"; break;
		case QUIT: return "QUIT"; break;
		case CLOSE: return "CLOSE"; break;
		case SHOWN: return "SHOWN"; break;
		case HIDDEN: return "HIDDEN"; break;
		case EXPOSED: return "EXPOSED"; break;
		case MOVED: return "MOVED"; break;
		case RESIZE: return "RESIZE"; break;
		case SIZECHANGED: return "SIZECHANGED"; break;
		case MINIMIZED: return "MINIMIZED"; break;
		case MAXIMIZED: return "MAXIMIZED"; break;
		case RESTORED: return "RESTORED"; break;
		case FOCUS: return "FOCUS"; break;
		case BLUR: return "BLUR"; break;
		case TAKEFOCUS: return "TAKEFOCUS"; break;
		case HITTEST: return "HITTEST"; break;
		case MOUSEENTER: return "MOUSEENTER"; break;
		case MOUSELEAVE: return "MOUSELEAVE"; break;
		case MOUSEUP: return "MOUSEUP"; break;
		case MOUSEDOWN: return "MOUSEDOWN"; break;
		case MOUSEMOVE: return "MOUSEMOVE"; break;
		case CLICK: return "CLICK"; break;
		case DBLCLICK: return "DBLCLICK"; break;
		case KEYUP: return "KEYUP"; break;
		case KEYDOWN: return "KEYDOWN"; break;
		case MOUSEWHEEL: return "MOUSEWHEEL"; break;
		case DROPFILE: return "DROPFILE"; break;
	}
}


//Images
Argon_Drawable::Argon_Drawable(Argon& context, SDL_Texture* texture) : context(context), texture(texture) {}
Argon_Drawable::~Argon_Drawable() {
  SDL_DestroyTexture(texture);
}

void Argon_Drawable::render(Argon_Rect* rect) {
  if(angle < 0.0001 && angle > -.0001 && flip == ARGON_FLIP_NONE) {
    SDL_RenderCopy(context.ren,texture,crop_ptr,(SDL_Rect*)rect);
  }
  else {
    SDL_RenderCopyEx(context.ren,texture,crop_ptr,(SDL_Rect*)rect,angle,center_ptr,(SDL_RendererFlip)flip);
  }
}
void Argon_Drawable::render(int x, int y) {
  Argon_Rect rect = {x,y,w,h};
  render(&rect);
}
void Argon_Drawable::render(int x, int y, int w, int h) {
  Argon_Rect rect = {x,y,w,h};
  render(&rect);
}

void Argon_Drawable::setAlpha(uint8_t a) {
  alpha = a;
  if(SDL_SetTextureAlphaMod(texture, alpha) == -1) {
    printf("\e[31m[ERROR]  Alpha Modulation Not Supported\e[0m\n");
  }
}
uint8_t Argon_Drawable::getAlpha() {
  return alpha;
}
void Argon_Drawable::modifyColor(uint8_t r, uint8_t g, uint8_t b) {
  SDL_SetTextureColorMod(texture, r,g,b);
}
void Argon_Drawable::setAngle(double a) {
  angle = a;
}
double Argon_Drawable::getAngle() {
  return angle;
}
void Argon_Drawable::setFlip(Argon_Flip f) {
  flip = f;
}
Argon_Flip Argon_Drawable::getFlip() {
  return flip;
}
void Argon_Drawable::crop(int x, int y, int w, int h) {
  crop_rect = {x,y,w,h};
  crop_ptr = &crop_rect;
}
void Argon_Drawable::crop(Argon_Rect* rect) {
  memcpy(&crop_rect, &rect, sizeof(rect));
  crop_ptr = &crop_rect;
}
void Argon_Drawable::resetCrop() {
  crop_ptr = NULL;
}
void Argon_Drawable::setRotateCenter(int cx, int cy) {
  center = {cx,cy};
  center_ptr = &center;
}
void Argon_Drawable::resetRotateCenter() {
  center_ptr = NULL;
}
int Argon_Drawable::width() {
  return w;
}
int Argon_Drawable::height() {
  return h;
}
void Argon_Drawable::getSize(int* _w, int* _h) {
  *_w = w;
  *_h = h;
}

void Argon_Drawable::addDrawable(Argon_Drawable& other) {
  SDL_Texture* tmp = SDL_CreateTexture(context.ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
  SDL_SetRenderTarget(context.ren, tmp);
  SDL_SetRenderDrawColor(context.ren, 0, 0, 0, 0);
  SDL_RenderClear(context.ren);
  SDL_RenderCopy(context.ren, texture, crop_ptr, crop_ptr);
  SDL_RenderCopy(context.ren, other.texture, NULL, other.crop_ptr);
  SDL_SetRenderTarget(context.ren, NULL);
  SDL_DestroyTexture(texture);
  texture = tmp;
}

Argon_Image::Argon_Image(Argon& context, const char* path) : Argon_Drawable(context, NULL), path(path) {
  if(context.imagesEnabled) {
    SDL_Surface* surface = IMG_Load(path);
    if(surface == NULL) {
      printf("\e[31m[ERROR]  Failed to load %s\e[0m\n", path);
    }
    else {
      texture = SDL_CreateTextureFromSurface(context.ren,surface);
      SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
      SDL_QueryTexture(texture, NULL, NULL, &w, &h);
      SDL_FreeSurface(surface);
    }
  }
  else {
    printf("\e[31m[ERROR]  Images not enabled, please use ARGON_IMAGES flag\e[0m\n");
  }
}

const char* Argon_Image::getFileName() {
  return path;
}

Argon_Shape::Argon_Shape(Argon& context, int _w, int _h) : Argon_Drawable(context, NULL) {
  w = _w;
  h = _h;
  surface = SDL_CreateRGBSurface(0,w,h,8,0,0,0,0);
  surface = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGBA32,0);
  memset(((uint8_t*)surface->pixels), 0, surface->pitch*surface->h);
}
Argon_Shape::~Argon_Shape() {
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void Argon_Shape::updateTexture() {
  texture = SDL_CreateTextureFromSurface(context.ren,surface);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}
void Argon_Shape::setPixel(int x, int y, Argon_RGB& color) {
  if((x * w + y)*4 > surface->pitch*surface->h || x < 0 || y < 0) {
    printf("\e[31[ERROR]  Attempted to set pixel out of bounds of image \e[0m\n");
  }
  else {
    memcpy(&(((uint8_t*)surface->pixels)[(x * w + y)*4]), &color, sizeof(color));
  }
}
void Argon_Shape::hLine(int y, int x1, int x2, Argon_RGB& color) {
  int start = x2 > x1 ? x1 : x2;
  int end = x2 > x1 ? x2 : x1;
  for(int i = 0;i < end-start;++i) {
    setPixel(start+i, y, color);
  }
}
void Argon_Shape::vLine(int x, int y1, int y2, Argon_RGB& color) {
  int start = y2 > y1 ? y1 : y2;
  int end = y2 > y1 ? y2 : y1;
  for(int i = 0;i < end-start;++i) {
    setPixel(x, start+i, color);
  }
}

// Argon_HSL Argon_RGB::toHSL() {
//   float H, S, L;
//   float R = r / 255.0;
//   float G = g / 255.0;
//   float B = b / 255.0;
//   float& max = (R > G ? (R > B ? R : B) : (G > B ? G : B));
//   float& min = (R < G ? (R < B ? R : B) : (G < B ? G : B));

//   L = (max + min) / 2.0f;
//   if(max == min) {
//     H = 0;
//     S = 0;
//   }
//   else {
//     if(L < 0.5f) {
//       S = (max-min) / (max+min);
//     }
//     else {
//       S = (max-min) / (2.0f-max-min);
//     }

//     if(&max == &R) {
//       H = (G-B)/(max-min) * 60.0f;
//     }
//     else if(&max == &G) {
//       H = (2.0 + (B-R)/(max-min)) * 60.0f;
//     }
//     else {
//       H = (4.0 + (R-G)/(max-min)) * 60.0f;
//     }
//     if(H < 0) {H += 360;}
//   }
//   return {H,S,L,a / 255.0f};
// }
// Argon_RGB Argon_HSL::toRGB() {
//   if(s == 0) {
//     return {(uint8_t)(l*255.0), (uint8_t)(l*255.0), (uint8_t)(l*255.0), (uint8_t)(a*255.0)};
//   }
//   else {
// ry_1 = Luminance x (1.0+Saturation)
// If Luminance is equal or larger then 0.5 (50%) then temporary_1 = Luminance + Saturation – Luminance x Saturation
//     float t1;
//     if(l < .5) {
//       t1 = l * (1 + s);
//     }
//     else {
//       t1 = l + s - l * s;
//     }
//     float t2 = 2 * l - t1;
//     float hue = h / 360.0f;
//     float r = hue + 0.333;
//     float g = hue = 0.536;
//     float b = hue – 0.333;
    
//   }
// }