#include "argon.h"


Argon::Argon(uint32_t flags) : Argon("Argon", 60, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, flags) {}
Argon::Argon(const char* title, uint32_t flags) : Argon(title, 60, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, flags) {}
Argon::Argon(const char* title, int32_t x, int32_t y, uint16_t w, uint16_t h, uint32_t flags) : Argon(title, 60, x, y, w, h, flags) {}
Argon::Argon(const char* title, uint8_t fps, int32_t x, int32_t y, uint16_t w, uint16_t h, uint32_t flags) : title(title), fps(fps), x(x), y(y), width(w), height(h) {
  uint32_t windowFlags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
  uint32_t rendererFlags = 0;
  if(flags & FULLSCREEN) windowFlags |= SDL_WINDOW_FULLSCREEN;
  if(flags & FULLSCREEN_DESKTOP) windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if(flags & HIDDEN) windowFlags |= SDL_WINDOW_HIDDEN;
  if(flags & BORDERLESS) windowFlags |= SDL_WINDOW_BORDERLESS;
  if(flags & FIXED_SIZE) windowFlags &= ~SDL_WINDOW_RESIZABLE;
  if(flags & MINIMIZED) windowFlags |= SDL_WINDOW_MINIMIZED;
  if(flags & MAXIMIZED) windowFlags |= SDL_WINDOW_MAXIMIZED;
  if(flags & ALWAYS_ON_TOP) windowFlags |= SDL_WINDOW_ALWAYS_ON_TOP;

  //Init SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("\e[31m[ERROR] Failed to initialize SDL\e[0m\n");
  }

  //Create Window
  window = SDL_CreateWindow(title, (int)x, (int)y, (int)width, (int)height, windowFlags);
  if(window == NULL) {
    printf("\e[31m[ERROR] Failed to create window\e[0m\n");
  }

  spf = 1000 / fps;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_AddEventWatch(resizeWatcher, this);
}


Argon::~Argon() {
  running = false;
  SDL_DelEventWatch(resizeWatcher,this);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

// Main Loop
void Argon::start() {
  bool firstTime = true;
  running = true;
  SDL_Event event;
  while(running) {
    uint32_t ticks = SDL_GetTicks();
    if(firstTime) {
      triggerEvent({LOAD, ticks, SDL_GetWindowID(window)});
      firstTime = false;
    }
    if(nextStep <= ticks || useVsync) {
      uint8_t limit = 10;
      while(nextStep <= ticks && limit--) {
        if(gameStep != NULL) {
          gameStep(*this, loopData);
        }
        nextStep += spf;
      }
      SDL_RenderPresent(renderer);
    }
    else {
      SDL_Delay(nextStep - ticks);
    }
    while(SDL_PollEvent(&event)) {
      eventWatcher(&event);
    }
  }
}
void Argon::setLoop(ArgonLooper loop, void* data) {
  loopData = data;
  gameStep = loop;
}
void Argon::removeLoop() {
  gameStep = NULL;
}



//Event Handling
bool Argon::triggerEvent(Event event) {
  try {
    eventMap.at(event.type)(*this, event);
    return true;
  } catch(std::out_of_range) {}
  return false;
}
void Argon::eventWatcher(SDL_Event* e) {
  Event event = {};
  if(e->type == SDL_QUIT) {
    running = false;
    event.type = QUIT;
    event.windowID = SDL_GetWindowID(window);
    event.timestamp = e->quit.timestamp;
  }
  else if(e->type == SDL_WINDOWEVENT) {
    if(e->window.event == SDL_WINDOWEVENT_RESIZED) return;
    event.type = (EventType)(SHOW_WINDOW + e->window.event - 1);
    event.timestamp = e->window.timestamp;
    event.windowID = e->window.windowID;
    event.timestamp = e->window.timestamp;
    if(event.type == WINDOW_SIZE_CHANGED) {
      width = e->window.data2;
      height = e->window.data1;
      event.width = width;
      event.height = height;
    }
    else if(event.type == MOVE_WINDOW) {
      x = e->window.data1;
      y = e->window.data2;
      event.wx = x;
      event.wy = y;
    }
    else if(event.type == MOUSEENTER || event.type == MOUSELEAVE) {
      event.which = getMouseButton(&event.x, &event.y);
      event.relx = 0;
      event.rely = 0;
      event.clicks = 0;
      event.down = event.which > -1;
      event.isWheelInverted = false;
    }
  }
  else if(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
    event.type = e->type == SDL_KEYUP ? KEYUP : KEYDOWN;
    event.timestamp = e->key.timestamp;
    event.windowID = e->key.windowID;
    event.isPressed = e->type == SDL_KEYDOWN;
    event.isKeyRepeat = e->key.repeat > 0;
    event.keycode = e->key.keysym.sym;
    event.scancode = e->key.keysym.scancode;
    event.key = SDL_GetKeyName(e->key.keysym.sym);
    event.lshift = e->key.keysym.mod & KMOD_LSHIFT;
    event.rshift = e->key.keysym.mod & KMOD_RSHIFT;
    event.shift = e->key.keysym.mod & KMOD_SHIFT;
    event.lctrl = e->key.keysym.mod & KMOD_LCTRL;
    event.rctrl = e->key.keysym.mod & KMOD_RCTRL;
    event.ctrl = e->key.keysym.mod & KMOD_CTRL;
    event.lalt = e->key.keysym.mod & KMOD_LALT;
    event.ralt = e->key.keysym.mod & KMOD_RALT;
    event.alt = e->key.keysym.mod & KMOD_ALT;
    event.lmeta = e->key.keysym.mod & KMOD_LGUI;
    event.rmeta = e->key.keysym.mod & KMOD_RGUI;
    event.meta = e->key.keysym.mod & KMOD_GUI;
    event.numlock = e->key.keysym.mod & KMOD_NUM;
    event.capslock = e->key.keysym.mod & KMOD_CAPS;
    event.altgr = e->key.keysym.mod & KMOD_MODE;
  }
  else if(e->type == SDL_MOUSEMOTION) {
    event.type = MOUSEMOVE;
    event.windowID = e->motion.windowID;
    event.timestamp = e->motion.timestamp;
    event.which = getMouseButton();
    event.x = e->motion.x;
    event.y = e->motion.y;
    event.relx = e->motion.xrel;
    event.rely = e->motion.yrel;
    event.clicks = 0;
    event.down = event.which > -1;
    event.isWheelInverted = false;
  }
  else if(e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
    event.type = e->type == SDL_MOUSEBUTTONDOWN ? MOUSEDOWN : MOUSEUP;
    event.windowID = e->button.windowID;
    event.timestamp = e->button.timestamp;
    event.which = e->button.button - 1;
    event.x = e->button.x;
    event.y = e->button.y;
    event.relx = 0;
    event.rely = 0;
    event.clicks = e->button.clicks;
    event.down = event.which > -1;
    event.isWheelInverted = false;
  }
  else if(e->type == SDL_MOUSEWHEEL) {
    event.type = MOUSEWHEEL;
    event.windowID = e->wheel.windowID;
    event.timestamp = e->wheel.timestamp;
    event.which = getMouseButton();
    event.x = e->wheel.x;
    event.y = e->wheel.y;
    event.relx = e->wheel.x;
    event.rely = e->wheel.y;
    event.clicks = 0;
    event.down = event.which > -1;
    event.isWheelInverted = e->wheel.direction == SDL_MOUSEWHEEL_FLIPPED;
  }
  else if(e->type == SDL_DROPFILE || e->type == SDL_DROPTEXT || e->type == SDL_DROPBEGIN || e->type == SDL_DROPCOMPLETE) {
    event.type = (EventType)(e->type - SDL_DROPFILE + DROPFILE);
    event.windowID = e->drop.windowID;
    event.timestamp = e->drop.timestamp;
    event.file = e->drop.file;
  }

  if(event.type != NO_EVENT) {
   try {
      eventMap.at(event.type)(*this, event);
    } catch(std::out_of_range) {}
  }
}
int Argon::resizeWatcher(void* data, SDL_Event* e) {
  if(e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_RESIZED) {
    Argon* a = (Argon*)data;
    Event event = {RESIZE_WINDOW, e->window.timestamp, e->window.windowID};
    a->width = e->window.data2;
    a->height = e->window.data1;
    event.width = a->width;
    event.height = a->height;
    try {
      a->eventMap.at(event.type)(*a, event);
    } catch(std::out_of_range) {}
  }
  return 0;
}

void Argon::setEventHandler(EventType type, EventHandler handler) {
  if(type == NO_EVENT) {
    throw std::invalid_argument("You can only set handlers for real events (not NO_EVENT)");
  }
  else {
    eventMap[type] = handler;
  }
  
}
void Argon::removeEventHandler(EventType type) {
  eventMap.erase(type);
}


//Drawing Shapes
void Argon::drawRect(int32_t x, int32_t y, uint16_t w, uint16_t h) {
  SDL_Rect rect = {x,y,w,h};
  SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, stroke.r, stroke.g, stroke.b, stroke.a);
  SDL_RenderDrawRect(renderer, &rect); 
}
void Argon::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  
}
// thick_octant1(x0,y0,dx,dy):
//   p_error= 0   
//   error= 0
//   y= y0
//   x= x0
//   threshold = dx - 2dy
//   E_diag= -2dx
//   E_square= 2dy
//   length = dx
//   for p= 1 .. length
//     pleft_octant1(x,y, dx, dy, p_error)
//     pright_octant1(x,y, dx, dy, p_error)
//     if error > threshold
//       y= y + 1
//       error = error + E_diag
//       if p_error > threshold
//         pleft_octant1(x,y, dx, dy, p_error+E_diag+E_square)
//         pright_octant1(x,y, dx, dy, p_error+E_diag+E_square)
//         p_error= p_error + E_diag
//       end
//       p_error= p_error + E_square
//     end
//     error = error + E_square
//     x= x + 1
//   end





//Other Settings
bool Argon::setRelativeMouse(bool val) {
  return SDL_SetRelativeMouseMode(val ? SDL_TRUE : SDL_FALSE) == 0;
}

//Setters
void Argon::setWindowSize(uint16_t width, uint16_t height) {
  SDL_SetWindowSize(window, width, height);
}
void Argon::setWindowPosition(int32_t x, int32_t y) {
  SDL_SetWindowPosition(window, x, y);
}
void Argon::setFill(Color& c) {
  fill = c;
}
void Argon::setFill(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  fill.r = r;
  fill.g = g;
  fill.b = b;
  fill.a = a;
}
void Argon::setStroke(Color& c) {
  stroke = c;
}
void Argon::setStroke(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  stroke.r = r;
  stroke.g = g;
  stroke.b = b;
  stroke.a = a;
}
void Argon::setStrokeWidth(uint16_t w) {
  strokeWidth = w;
}



//Getters
uint8_t Argon::getFps() {
  return fps;
}
int32_t Argon::getX() {
  return x;
}
int32_t Argon::getY() {
  return y;
}
uint16_t Argon::getWidth() {
  return width;
}
uint16_t Argon::getHeight() {
  return height;
}
Color& Argon::getFill() {
  return fill;
}
Color& Argon::getStroke() {
  return stroke;
}
uint16_t Argon::getStrokeWidth() {
  return strokeWidth;
}



// Helpers
int8_t Argon::getMouseButton(int32_t* mx, int32_t* my) {
  uint32_t state = SDL_GetMouseState(mx,my);
  if(state & SDL_BUTTON(SDL_BUTTON_LEFT)) return 0;
  if(state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) return 1;
  if(state & SDL_BUTTON(SDL_BUTTON_RIGHT)) return 2;
  if(state & SDL_BUTTON(SDL_BUTTON_X1)) return 3;
  if(state & SDL_BUTTON(SDL_BUTTON_X2)) return 4;
  return -1;
}