#ifndef ARGON_H
#define ARGON_H

#include <SDL2/SDL.h>

#include <chrono>
#define START_SPEED_TEST() auto start = std::chrono::high_resolution_clock::now();
#define END_SPEED_TEST(msg) auto end = std::chrono::high_resolution_clock::now();printf("%s: %lldns\n", msg, std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count());

#define ARGON_FULLSCREEN 1
#define ARGON_HIDDEN 2
#define ARGON_BORDERLESS 4
#define ARGON_RESIZEABLE 8
#define ARGON_MINIMIZED 16
#define ARGON_MAXIMIZED 32
#define ARGON_HIGHDPI 64
#define ARGON_ON_TOP 128
#define ARGON_NO_TASKBAR 256
#define ARGON_MOUSE_CAPTURE 512
#define ARGON_VSYNC 1024

#if defined(ARGON_FIXED_FRAMERATE) && !defined(ARGON_FPS)
	#define ARGON_FPS 60
#endif

//RESOLUTIONS
//Default is to maintin original resolution with letterboxes
// #define ARGON_RESOLUTION_CLIP   // Clipped at top left
// #define ARGON_RESOLUTION_CENTER // Cordinate system where origin is center of the screen

#if defined(ARGON_RESOLUTION_CENTER)
	#define TRANSLATE_PIXEL(x,y) adjustCoordinate(x, y)
	#define TRANSLATE_X(x) adjustCoordinateX(x)
	#define TRANSLATE_Y(y) adjustCoordinateY(y)
#else
	#define TRANSLATE_PIXEL(x,y)  //do nothing
	#define TRANSLATE_X(x) //do nothing
	#define TRANSLATE_Y(y) //do nothing
#endif

typedef SDL_MessageBoxButtonData ButtonData;

struct Color {
	uint8_t r,g,b,a=255;
};
struct Event {
	uint32_t timestamp;
};
struct WindowEvent : Event {
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
};
struct KeyEvent : Event {
	SDL_Scancode scancode;
	SDL_Keycode keycode;
	bool repeat;
	struct {
		bool none 		: 1;
		bool shift 		: 1;
		bool lshift 	: 1;
		bool rshift 	: 1;
		bool ctrl 		: 1;
		bool lctrl 		: 1;
		bool rctrl 		: 1;
		bool alt 			: 1;
		bool lalt 		: 1;
		bool ralt 		: 1;
		bool meta 		: 1;
		bool lmeta 		: 1;
		bool rmeta 		: 1;
		bool capslock : 1;
		bool numlock 	: 1;
		bool altgr 		: 1;
	} mod;
};
struct MouseEvent : Event {
	int32_t x;
	int32_t y;
	int32_t rx;
	int32_t ry;
	bool down;
};
struct MouseButtonEvent : Event {
	int32_t x;
	int32_t y;
	uint8_t which;
	uint8_t clicks;
};
struct WheelEvent : Event {
	int32_t dx;
	int32_t dy;
	bool down;
	bool flipped;
};
struct FileDropEvent : Event {
	char* path;
};

class Argon {
public:
	Argon(const char* title, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t flags = ARGON_RESIZEABLE|ARGON_HIGHDPI) : title(title), _x(x), _y(y), _w(w), _h(h), flags(flags) {}
	Argon(const char* title, int32_t w, int32_t h, uint16_t flags) : Argon(title, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,flags) {}
	Argon(const char* title, int32_t w, int32_t h) : Argon(title, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h) {}
	Argon(const char* title, uint16_t flags) : Argon(title, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500, flags) {}
	Argon(const char* title) : Argon(title, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500, ARGON_RESIZEABLE|ARGON_HIGHDPI) {};
	Argon() : Argon("Argon", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500, ARGON_RESIZEABLE|ARGON_HIGHDPI) {};
	~Argon() {
		_running = false;
	  SDL_DestroyTexture(backbuffer);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	  SDL_DelEventWatch(handleResize, this);
		SDL_Quit();
	}


	void begin() {
		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
  		fprintf(stderr, "Failed to initalize SDL\n");
  		return;
  	}

    int sdlFlags = 0;
    if(flags & ARGON_FULLSCREEN) {sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;}
    if(flags & ARGON_HIDDEN) {sdlFlags |= SDL_WINDOW_HIDDEN;}
    if(flags & ARGON_BORDERLESS) {sdlFlags |= SDL_WINDOW_BORDERLESS;}
    if(flags & ARGON_RESIZEABLE) {sdlFlags |= SDL_WINDOW_RESIZABLE;}
    if(flags & ARGON_MINIMIZED) {sdlFlags |= SDL_WINDOW_MINIMIZED;}
    if(flags & ARGON_MAXIMIZED) {sdlFlags |= SDL_WINDOW_MAXIMIZED;}
    if(flags & ARGON_HIGHDPI) {sdlFlags |= SDL_WINDOW_ALLOW_HIGHDPI;}
    if(flags & ARGON_ON_TOP) {sdlFlags |= SDL_WINDOW_ALWAYS_ON_TOP;}
    if(flags & ARGON_NO_TASKBAR) {sdlFlags |= SDL_WINDOW_SKIP_TASKBAR;}
    if(flags & ARGON_MOUSE_CAPTURE) {sdlFlags |= SDL_WINDOW_MOUSE_CAPTURE;}
    if(flags & ARGON_VSYNC) {vsyncEnabled=true;sdlFlags |= SDL_RENDERER_PRESENTVSYNC;}

  	window = SDL_CreateWindow(title, _x, _y, _w, _h, sdlFlags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
    SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &displayInfo);
    SDL_SetWindowTitle(window, title);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_GetWindowSize(window, &_w, &_h);
    SDL_GetMouseState(&_mx, &_my);
    SDL_GetWindowPosition(window, &_x, &_y);
    #if defined(ARGON_RESOLUTION_CLIP)
      windowRect = {0,0, _w, _h};
      backbuffer = SDL_CreateTexture(renderer, displayInfo.format, SDL_TEXTUREACCESS_TARGET, displayInfo.w, displayInfo.h);
    #elif defined(ARGON_RESOLUTION_CENTER)
      windowRect = {displayInfo.w/2 - _w/2, displayInfo.h/2 - _h/2, _w, _h};
      backbuffer = SDL_CreateTexture(renderer, displayInfo.format, SDL_TEXTUREACCESS_TARGET, displayInfo.w, displayInfo.h);
      adjustCoordinate2(_mx, _my);
    #else
      backbuffer = SDL_CreateTexture(renderer, displayInfo.format, SDL_TEXTUREACCESS_TARGET, _w, _h);
      SDL_RenderSetLogicalSize(renderer, _w, _h);
    #endif

    SDL_SetRenderTarget(renderer, backbuffer);

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    SDL_AddEventWatch(handleResize, this);

  	_running = true;


  	//Local Variables for loop
  	#ifdef ARGON_FIXED_FRAMERATE
  		uint32_t now = 0;
  		uint32_t nextStep = 0;
  		constexpr uint32_t fps_step = 1000 / ARGON_FPS;
  	#else
  		uint32_t deltaTime = 0;
  	#endif
    
    uint32_t step = 1000 / 60;
    uint8_t limit = 10;
  	SDL_Event event;

    onLoad({SDL_GetTicks(), _x, _y, _w, _h});


  	//Game Loop
  	while(_running) {
  		//Events
  		while(SDL_PollEvent(&event)) {
  			handleEvent(event);
  		}

  		//Logic
  		#ifdef ARGON_FIXED_FRAMERATE
	  		now = SDL_GetTicks();
	  		if(nextStep <= now || vsyncEnabled) {
		  		SDL_SetRenderTarget(renderer, backbuffer);
		  		limit = 10;
		  		while(nextStep <= now && limit--) {
		  			gameLoop();
		  			nextStep += fps_step;
		  		}
		  	}
		  	else {
		  		#ifndef ARGON_FPS_NODELAY
		  			printf("Waiting: %dms\n", nextStep - SDL_GetTicks());
		  			SDL_Delay(nextStep - SDL_GetTicks());
		  		#endif
		  		continue;
		  	}
		  #else


		  #endif

  		//Render
      SDL_SetRenderTarget(renderer, NULL);

      #if defined(ARGON_RESOLUTION_CLIP) || defined(ARGON_RESOLUTION_CENTER)
  		  SDL_RenderCopy(renderer, backbuffer, &windowRect, NULL);
      #else
        SDL_RenderCopy(renderer, backbuffer, NULL, NULL);
      #endif

      SDL_RenderPresent(renderer);

  	}
	}
	void stop() {this->~Argon();}

	// Event Handlers
	virtual void onQuit(const Event event) {}
	virtual void onLoad(const WindowEvent event) {}
	virtual void onMouseDown(const MouseButtonEvent event) {}
	virtual void onMouseUp(const MouseButtonEvent event) {}
	virtual void onMouseMove(const  MouseEvent event) {}
	virtual void onMouseWheel(const WheelEvent event) {}
	virtual void onKeyDown(const KeyEvent event) {}
	virtual void onKeyUp(const KeyEvent event) {}
	virtual void onWindowShow(const WindowEvent event) {}
	virtual void onWindowExpose(const WindowEvent event) {}
	virtual void onWindowMove(const WindowEvent event) {}
	virtual void onWindowResize(const WindowEvent event) {}
	virtual void onWindowMinimize(const WindowEvent event) {}
	virtual void onWindowMaximize(const WindowEvent event) {}
	virtual void onWindowRestore(const WindowEvent event) {}
	virtual void onMouseEnter(const MouseEvent event) {}
	virtual void onMouseLeave(const MouseEvent event) {}
	virtual void onWindowFocus(const WindowEvent event) {}
	virtual void onWindowBlur(const WindowEvent event) {}
	virtual void onWindowClose(const WindowEvent event) {}
	virtual void onWindowTakeFocus(const WindowEvent event) {}
	virtual void onWindowHitTest(const WindowEvent event) {}
	virtual void onFileDrop(const FileDropEvent event) {}

	//Main loop logic
	#ifdef ARGON_FIXED_FRAMERATE
		virtual void gameLoop() {}
	#else
		virtual void gameLoop(uint32_t deltaTime) {}
	#endif

	// Translating SDL Functions
	void maximize() {SDL_MaximizeWindow(window);}
	void minimize() {SDL_MinimizeWindow(window);}
	void show() {SDL_ShowWindow(window);}
	void hide() {SDL_HideWindow(window);}
	void raise() {SDL_RaiseWindow(window);}
	void restore() {SDL_RestoreWindow(window);}
	void fullscreen() {SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);}
	void setMaxSize(int w, int h) {SDL_SetWindowMaximumSize(window, w, h);}
	void setMinSize(int w, int h) {SDL_SetWindowMinimumSize(window, w, h);}
	void setOpacity(float opacity) {SDL_SetWindowOpacity(window, opacity);}
	void setPosition(int x, int y) {SDL_SetWindowPosition(window, x, y);}
	void setMousePosition(int x, int y) {SDL_WarpMouseInWindow(window, x, y);}
	void setSize(int w, int h) {SDL_SetWindowSize(window, w, h);}
	void setTitle(const char* title) {SDL_SetWindowTitle(window, title);}
	void setIsResizable(bool resizable) {SDL_SetWindowResizable(window, (SDL_bool)resizable);}
	void setBordered(bool bordered) {SDL_SetWindowBordered(window,(SDL_bool) bordered);}
	void setBrightness(float brightness) {SDL_SetWindowBrightness(window, brightness);}
	bool setCaptureMouse(bool capture = true) {return SDL_CaptureMouse((SDL_bool)capture) == 0;}
	bool setRelativeMouseMode(bool relative) {return SDL_SetRelativeMouseMode((SDL_bool)relative) == 0;}
	void setCursorDisplay(bool show) {SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);}
	bool alert(const char* message) {return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, getTitle(), message, window) == 0;}
	bool alert(const char* title, const char* message) {return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, window) == 0;}
	int alert(const char* title, const char* message, uint8_t numButtons, SDL_MessageBoxButtonData* btnData, SDL_MessageBoxFlags flag = SDL_MESSAGEBOX_INFORMATION) {int btn;SDL_MessageBoxData data = {flag, window, title, message, numButtons, btnData, NULL};SDL_ShowMessageBox(&data, &btn);return btn;	}

	//Getters
	bool running() {return _running;}
	int32_t x() {return _x;}
	int32_t y() {return _y;}
	int32_t w() {return _w;}
	int32_t h() {return _h;}
	int32_t mx() {return _mx;}
	int32_t my() {return _my;}
	bool mouseDown() {return _down;}
	bool getBorderSizes(int* top, int* right, int* bottom, int* left) {return SDL_GetWindowBordersSize(window, top, left, bottom, right) == 0;}
	float getBrightness() {return SDL_GetWindowBrightness(window);}
	void getMaxSize(int* w, int* h) {SDL_GetWindowMaximumSize(window, w, h);}
	void getMinSize(int* w, int* h) {SDL_GetWindowMinimumSize(window, w, h);}
	float getOpacity() {float o = -1;SDL_GetWindowOpacity(window, &o);return o;}
	const char* getTitle() {return SDL_GetWindowTitle(window);}


	// Graphics
	void setColor(uint32_t c) {
		SDL_SetRenderDrawBlendMode(renderer, (((c>>24)&255) == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, (c>>24)&255, (c>>16)&255, (c>>8)&255, c&255);
	}
	void setColor(Color c) {
		SDL_SetRenderDrawBlendMode(renderer, (c.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	}
	void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
		SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
	}

	void clear() {SDL_RenderClear(renderer);}
	void pixel(int32_t x, int32_t y) {
		TRANSLATE_PIXEL(x, y);
		SDL_RenderDrawPoint(renderer, x, y);
	}
	void line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t thickness = 1) {
		TRANSLATE_PIXEL(x1, y1);
		TRANSLATE_PIXEL(x2, y2);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}
	void hline(int32_t x1, int32_t x2, int32_t y, uint8_t thickness = 1) {TRANSLATE_X(x1);TRANSLATE_X(x2);TRANSLATE_Y(y);SDL_RenderDrawLine(renderer, x1, y, x2, y);}
	void vline(int32_t x, int32_t y1, int32_t y2, uint8_t thickness = 1) {TRANSLATE_X(x);TRANSLATE_Y(y1);TRANSLATE_Y(y2);SDL_RenderDrawLine(renderer, x, y1, x, y2);}
	void rect(int32_t x, int32_t y, uint16_t w, uint16_t h, uint8_t thickness = 1) {
		TRANSLATE_PIXEL(x, y);
		for(uint8_t t = 0;t < thickness;++t) {
			SDL_RenderDrawLine(renderer, x-t, y-t, x+w+t, y-t);
			SDL_RenderDrawLine(renderer, x+w+t, y-t, x+w+t, y+h+t);
			SDL_RenderDrawLine(renderer, x+w+t, y+h+t, x-t, y+h+t);
			SDL_RenderDrawLine(renderer, x-t, y+h+t, x-t, y-t);
		}
	}
	void filledRect(int32_t x, int32_t y, uint16_t w, uint16_t h) {
		TRANSLATE_PIXEL(x, y);
		SDL_Rect rect = {x, y, w, h};
		SDL_RenderFillRect(renderer, &rect);
	}
	//https://www.geeksforgeeks.org/mid-point-circle-drawing-algorithm/?ref=rp
	void circle(int32_t cx, int32_t cy, uint16_t r, uint8_t thickness = 1) {
		TRANSLATE_PIXEL(cx, cy);
		int32_t xo = r+thickness/2, xi = r-thickness/2, y = 0, dout = 1 - xo, din = 1-xi;
		SDL_RenderDrawPoint(renderer, cx + r, cy);
		SDL_RenderDrawPoint(renderer, cx - r, cy);
		SDL_RenderDrawPoint(renderer, cx, cy + r);
		SDL_RenderDrawPoint(renderer, cx, cy - r);

		while(xo > y) {
			++y;
			if(dout <= 0) {
				dout += 2 * y + 1;
			}
			else {
				--xo;
				dout += 2 * y - 2 * xo + 1;
			}
			if(y > r-thickness/2) {
				xi = y;
			}
			else {
				if(din < 0) {
					din += 2 * y + 1;
				}
				else {
					--xi;
					din += 2 * y - 2 * xi + 1;
				}
			}
			if(xo < y) break;
			SDL_RenderDrawPoint(renderer, cx + xo, cy + y);
			SDL_RenderDrawPoint(renderer, cx - xo, cy + y);
			SDL_RenderDrawPoint(renderer, cx + xo, cy - y);
			SDL_RenderDrawPoint(renderer, cx - xo, cy - y);

			if(xo != y) {
				SDL_RenderDrawPoint(renderer, cx + y, cy + xo);
				SDL_RenderDrawPoint(renderer, cx - y, cy + xo);
				SDL_RenderDrawPoint(renderer, cx + y, cy - xo);
				SDL_RenderDrawPoint(renderer, cx - y, cy - xo);
			}
		}
		// int xo = outer;
    //  int xi = inner;
    //  int y = 0;
    //  int erro = 1 - xo;
    //  int erri = 1 - xi;
		//
    //  while(xo >= y) {
    //      xLine(xc + xi, xc + xo, yc + y,  colour);
    //      yLine(xc + y,  yc + xi, yc + xo, colour);
    //      xLine(xc - xo, xc - xi, yc + y,  colour);
    //      yLine(xc - y,  yc + xi, yc + xo, colour);
    //      xLine(xc - xo, xc - xi, yc - y,  colour);
    //      yLine(xc - y,  yc - xo, yc - xi, colour);
    //      xLine(xc + xi, xc + xo, yc - y,  colour);
    //      yLine(xc + y,  yc - xo, yc - xi, colour);
		//
    //      y++;
		//
    //      if (erro < 0) {
    //          erro += 2 * y + 1;
    //      } else {
    //          xo--;
    //          erro += 2 * (y - xo + 1);
    //      }
		//
    //      if (y > inner) {
    //          xi = y;
    //      } else {
    //          if (erri < 0) {
    //              erri += 2 * y + 1;
    //          } else {
    //              xi--;
    //              erri += 2 * (y - xi + 1);
    //          }
    //      }
    //  }
	}
	void fillCircle(int32_t cx, int32_t cy, uint16_t r) {
		START_SPEED_TEST();
		TRANSLATE_PIXEL(cx, cy);
		int32_t x = r, y = 0, d = 1 - r;
		SDL_RenderDrawLine(renderer, cx-r, cy, cx+r, cy);

		while(x > y) {
			++y;
			if(d <= 0) {
				d += 2 * y + 1;
			}
			else {
				--x;
				d += 2 * y - 2 * x + 1;
			}
			if(x < y) break;
			SDL_RenderDrawLine(renderer, cx-x, cy+y, cx+x, cy+y);
			SDL_RenderDrawLine(renderer, cx-x, cy-y, cx+x, cy-y);

			if(x != y) {
				SDL_RenderDrawLine(renderer, cx-y, cy+x, cx+y, cy+x);
				SDL_RenderDrawLine(renderer, cx-y, cy-x, cx+y, cy-x);
			}
		}
		END_SPEED_TEST("Fill circle");
	}

	//https://dai.fmph.uniba.sk/upload/0/01/Ellipse.pdf
	void ellipse(int32_t cx, int32_t cy, uint16_t xr, uint16_t yr) {
		TRANSLATE_PIXEL(cx, cy);
		int32_t x, y;
		int32_t delX, delY;
		int32_t err;
		int32_t twoXRSquared = 2*xr*xr,
						twoYRSquared = 2*yr*yr;
		int32_t stopX, stopY;

		//go from x-axis until tangent to ellipse is -1
		x = xr;
		y = 0;
		delX = yr*yr*(1-2*xr);
		delY = xr*xr;
		err = 0;
		stopX = twoYRSquared*xr;
		stopY = 0;
		while(stopX >= stopY) {
			//plot
			SDL_RenderDrawPoint(renderer, cx+x, cy+y);
			SDL_RenderDrawPoint(renderer, cx+x, cy-y);
			SDL_RenderDrawPoint(renderer, cx-x, cy+y);
			SDL_RenderDrawPoint(renderer, cx-x, cy-y);

			++y;
			stopY += twoXRSquared;
			err += delY;
			delY += twoXRSquared;
			if((2*err+delX) > 0) {
				--x;
				stopX -= twoYRSquared;
				err += delX;
				delX += twoYRSquared;
			}
		}

		//go from y-axis until tangent to ellipse is -1
		x = 0;
		y = yr;
		delX = yr*yr;
		delY = xr*xr*(1-2*yr);
		err = 0;
		stopX = 0;
		stopY = twoXRSquared*yr;
		while(stopX <= stopY) {
			//plot
			SDL_RenderDrawPoint(renderer, cx+x, cy+y);
			SDL_RenderDrawPoint(renderer, cx+x, cy-y);
			SDL_RenderDrawPoint(renderer, cx-x, cy+y);
			SDL_RenderDrawPoint(renderer, cx-x, cy-y);

			++x;
			stopX += twoYRSquared;
			err += delX;
			delX += twoYRSquared;
			if((2*err+delY) > 0) {
				--y;
				stopY -= twoXRSquared;
				err += delY;
				delY += twoXRSquared;
			}
		}
	}
	void thickEllipse(int32_t cx, int32_t cy, uint16_t ixr, uint16_t oxr, uint16_t iyr, uint16_t oyr) {
		//STUB
		//TODO: implement
		//https://stackoverflow.com/questions/55980376/midpoint-thick-ellipse-drawing-algorithm/55983075
	}




private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_DisplayMode displayInfo;
	SDL_Texture* backbuffer;
	bool _running = false;
	bool vsyncEnabled = false;

	//Window Information
	#if defined(ARGON_RESOLUTION_CLIP) || defined(ARGON_RESOLUTION_CENTER)
		SDL_Rect windowRect = {0};
	#endif
	uint16_t flags; 		// only for init (other functions may change later)
	const char* title;  // only for init (other functions may change later)
	int32_t _x;
	int32_t _y;
	int32_t _w;
	int32_t _h;

	//Mouse Information
	int32_t _mx;
	int32_t _my;
	bool _down = false;


	#if defined(ARGON_RESOLUTION_CENTER)
		template<typename T>
		void adjustCoordinate(T& x, T& y) inline {
			x += displayInfo.w / 2;
			y += displayInfo.h / 2;
		}
		template<typename T>
		void adjustCoordinateX(T& x) inline {
			x += displayInfo.w / 2;
		}
		template<typename T>
		void adjustCoordinateY(T& y) inline {
			y += displayInfo.h / 2;
		}
		template<typename T>
		void adjustCoordinate2(T& x, T& y) inline {
			x -= _w / 2;
			y -= _h / 2;
		}
	#endif


	// Event System
	void handleEvent(SDL_Event& event) {
		switch(event.type) {
  		case SDL_QUIT: {
        onQuit({event.quit.timestamp});
  			stop();
  			break;
      }
      case SDL_MOUSEBUTTONDOWN: {
        _down = true;
        onMouseDown({event.button.timestamp, _mx, _my, event.button.button, event.button.clicks});
      	break;
      }
      case SDL_MOUSEBUTTONUP: {
        _down = false;
        onMouseUp({event.button.timestamp, _mx, _my, event.button.button, event.button.clicks});
      	break;
      }
      case SDL_MOUSEMOTION: {
        _mx = event.motion.x;
        _my = event.motion.y;
        #if defined(ARGON_RESOLUTION_CENTER)
          adjustCoordinate2(_mx, _my);
        #endif
        onMouseMove({event.motion.timestamp, _mx, _my, event.motion.xrel, event.motion.yrel, _down});
        break;
      }
      case SDL_MOUSEWHEEL: {
        onMouseWheel({event.wheel.timestamp, event.wheel.x, event.wheel.y, _down, event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED});
        break;
      }
      case SDL_KEYDOWN: {
        onKeyDown({event.key.timestamp,event.key.keysym.scancode,event.key.keysym.sym,event.key.repeat > 0,{!static_cast<bool>((event.key.keysym.mod & KMOD_SHIFT) || (event.key.keysym.mod & KMOD_ALT) || (event.key.keysym.mod & KMOD_CTRL) || (event.key.keysym.mod & KMOD_GUI) || (event.key.keysym.mod & KMOD_CAPS) || (event.key.keysym.mod & KMOD_NUM) || (event.key.keysym.mod & KMOD_MODE)),static_cast<bool>(event.key.keysym.mod & KMOD_SHIFT),static_cast<bool>(event.key.keysym.mod & KMOD_LSHIFT),static_cast<bool>(event.key.keysym.mod & KMOD_RSHIFT),static_cast<bool>(event.key.keysym.mod & KMOD_CTRL),static_cast<bool>(event.key.keysym.mod & KMOD_LCTRL),static_cast<bool>(event.key.keysym.mod & KMOD_RCTRL),static_cast<bool>(event.key.keysym.mod & KMOD_ALT),static_cast<bool>(event.key.keysym.mod & KMOD_LALT),static_cast<bool>(event.key.keysym.mod & KMOD_RALT),static_cast<bool>(event.key.keysym.mod & KMOD_GUI),static_cast<bool>(event.key.keysym.mod & KMOD_LGUI),static_cast<bool>(event.key.keysym.mod & KMOD_RGUI),static_cast<bool>(event.key.keysym.mod & KMOD_CAPS),static_cast<bool>(event.key.keysym.mod & KMOD_NUM),static_cast<bool>(event.key.keysym.mod & KMOD_MODE)}});
        break;
      }
      case SDL_KEYUP: {
        onKeyUp({event.key.timestamp,event.key.keysym.scancode,event.key.keysym.sym,event.key.repeat > 0,{!static_cast<bool>((event.key.keysym.mod & KMOD_SHIFT) || (event.key.keysym.mod & KMOD_ALT) || (event.key.keysym.mod & KMOD_CTRL) || (event.key.keysym.mod & KMOD_GUI) || (event.key.keysym.mod & KMOD_CAPS) || (event.key.keysym.mod & KMOD_NUM) || (event.key.keysym.mod & KMOD_MODE)),static_cast<bool>(event.key.keysym.mod & KMOD_SHIFT),static_cast<bool>(event.key.keysym.mod & KMOD_LSHIFT),static_cast<bool>(event.key.keysym.mod & KMOD_RSHIFT),static_cast<bool>(event.key.keysym.mod & KMOD_CTRL),static_cast<bool>(event.key.keysym.mod & KMOD_LCTRL),static_cast<bool>(event.key.keysym.mod & KMOD_RCTRL),static_cast<bool>(event.key.keysym.mod & KMOD_ALT),static_cast<bool>(event.key.keysym.mod & KMOD_LALT),static_cast<bool>(event.key.keysym.mod & KMOD_RALT),static_cast<bool>(event.key.keysym.mod & KMOD_GUI),static_cast<bool>(event.key.keysym.mod & KMOD_LGUI),static_cast<bool>(event.key.keysym.mod & KMOD_RGUI),static_cast<bool>(event.key.keysym.mod & KMOD_CAPS),static_cast<bool>(event.key.keysym.mod & KMOD_NUM),static_cast<bool>(event.key.keysym.mod & KMOD_MODE)}});
        break;
      }
      case SDL_WINDOWEVENT: {
        switch(event.window.event) {
          case SDL_WINDOWEVENT_SHOWN: {
            onWindowShow({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_EXPOSED: {
            onWindowExpose({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_MOVED: {
            _x = event.window.data1;
            _y = event.window.data2;
            onWindowMove({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_SIZE_CHANGED: {
            _w = event.window.data1;
            _h = event.window.data2;
            #if defined(ARGON_RESOLUTION_CLIP)
              windowRect = {0,0, _w, _h};
            #elif defined(ARGON_RESOLUTION_CENTER)
              windowRect = {displayInfo.w/2 - _w/2, displayInfo.h/2 - _h/2, _w, _h};
            #endif
            onWindowResize({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_MINIMIZED: {
            onWindowMinimize({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_MAXIMIZED: {
            onWindowMaximize({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_RESTORED: {
            onWindowRestore({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_ENTER: {
            onMouseEnter({event.window.timestamp, _mx, _my, 0, 0, _down});
            break;
          }
          case SDL_WINDOWEVENT_LEAVE: {
            onMouseLeave({event.window.timestamp, _mx, _my, 0, 0, _down});
            break;
          }
          case SDL_WINDOWEVENT_FOCUS_GAINED: {
            onWindowFocus({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_FOCUS_LOST: {
            onWindowBlur({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_CLOSE: {
            onWindowClose({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_TAKE_FOCUS: {
            onWindowTakeFocus({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_WINDOWEVENT_HIT_TEST: {
            onWindowHitTest({event.window.timestamp, _x, _y, _w, _h});
            break;
          }
          case SDL_DROPFILE: {
            onFileDrop({event.drop.timestamp, event.drop.file});
            SDL_free(event.drop.file);
            break;
          }
        }
      }
  	}
	}
	static int handleResize(void* instance, SDL_Event* event) {
  	if(event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
	    Argon* a = (Argon*) instance;
	    a->_w = event->window.data1;
	    a->_h = event->window.data2;
	    #if defined(ARGON_RESOLUTION_CLIP)
	      a->windowRect = {0,0, a->_w, a->_h};
	    #elif defined(ARGON_RESOLUTION_CENTER)
	      a->windowRect = {a->displayInfo.w/2 - a->_w/2, a->displayInfo.h/2 - a->_h/2, a->_w, a->_h};
	    #endif
	    a->onWindowResize({event->window.timestamp, a->_x, a->_y, a->_w, a->_h});
	  }
	  return 0;
	}
};


#endif
