#ifndef ARGON_H
#define ARGON_H

#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
using namespace std;


#define ARGON_FULLSCREEN 1
#define ARGON_SHOWN 4
#define ARGON_HIDDEN 8
#define ARGON_BORDERLESS 16
#define ARGON_RESIZEABLE 32
#define ARGON_MINIMIZED 64
#define ARGON_MAXIMIZED 128
#define ARGON_HIGHDPI 8192
#define ARGON_BASIC 8196

enum EventType {
	QUIT,
	CLOSE,
	SHOWN,
	HIDDEN,
	EXPOSED,
	MOVED,
	RESIZED,
	SIZECHANGED,
	MINIMIZED,
	MAXIMIZED,
	RESTORED,
	KEYBOARDFOCUS,
	KEYBOARDBLUR,
	TAKEFOCUS,
	HITTEST,
	MOUSEENTER,
	MOUSELEAVE,
	MOUSEUP,
	MOUSEDOWN,
	MOUSEMOVE,
	CLICK,
	DBLCLICK,
	KEYUP,
	KEYDOWN,
	MOUSEWHEEL,
	DROPFILE
};
struct Event {
	EventType type;
};
struct Window {
	int x;
	int y;
	int w;
	int h;
	int dw;
	int dh;
	bool shown;
};
struct WindowEvent : Event {
	int x;
	int y;
	int w;
	int h;
	int dw;
	int dh;
	bool shown;
};
struct Mouse {
	int x;
	int y;
	bool down;
	uint8_t which;
};
struct MouseEvent : Event {
	int x;
	int y;
	bool down;
	uint8_t which;
};
struct Keyboard {
	int keyCode;
	const char* key;
	bool lshift;
	bool rshift;
	bool lctrl;
	bool rctrl;
	bool lalt;
	bool ralt;
	bool lmeta;
	bool rmeta;
	bool numlock;
	bool capslock;
	bool ctrl;
	bool shift;
	bool alt;
	bool meta;	
};
struct KeyboardEvent : Event {
	int keyCode;
	const char* key;
	bool lshift;
	bool rshift;
	bool lctrl;
	bool rctrl;
	bool lalt;
	bool ralt;
	bool lmeta;
	bool rmeta;
	bool numlock;
	bool capslock;
	bool ctrl;
	bool shift;
	bool alt;
	bool meta;
};
struct WheelEvent : Event {
	int x;
	int y;
	bool dir;
};
struct FileEvent : Event {
	const char* file;
	uint32_t timestamp;
};

typedef void (*Listener)(Event*);
typedef vector<Listener> Listeners;


class Argon {
private:
	int fps;
	double frameTime;
	bool canCountClick = false;
	uint32_t lastClick;
	SDL_Window* win;
	SDL_Renderer* ren;

	//Listeners
	Listeners quitListeners;
	Listeners closeListeners;
	Listeners shownListeners;
	Listeners hiddenListeners;
	Listeners exposedListeners;
	Listeners movedListeners;
	Listeners resizedListeners;
	Listeners sizeChangedListeners;
	Listeners minimizedListeners;
	Listeners maximizedListeners;
	Listeners restoredListeners;
	Listeners keyboardFocusListeners;
	Listeners keyboardBlurListeners;
	Listeners takeFocusListeners;
	Listeners hitTestListeners;
	Listeners mouseEnterListeners;
	Listeners mouseLeaveListeners;
	Listeners mouseUpListeners;
	Listeners mouseDownListeners;
	Listeners mouseMoveListeners;
	Listeners clickListeners;
	Listeners dblclickListeners;
	Listeners keyUpListeners;
	Listeners keyDownListeners;
	Listeners mouseWheelListeners;
	Listeners dropFileListeners;
	

	//Hidden Functions
	void init(int x, int y, int w, int h, int flags);
	Listeners* getListeners(EventType type);
	static int eventWatcher(void* data, SDL_Event* e);
	void eventHandler(Event* event, Listeners* listeners);

public:
	const char* name;
	bool running = false;
	int dblClickTime = 400;
	Window window = {-1, -1, -1, -1, -1, -1, 0};
	Mouse mouse = {0,0,0,false};
	Keyboard keyboard = {-1,"",false,false,false,false,false,false,false,false,false,false,false,false,false,false};


	Argon(const char* name, int fps, int flags);
	Argon(const char* name, int w, int h, int fps, int flags);
	Argon(const char* name, int x, int y, int w, int h, int fps, int flags);
	~Argon();

	void start();
	void addListener(EventType type, Listener listener);
	bool removeListener(EventType type, Listener listener);
	void setFps(int _fps);
	int getFps();

};


#endif