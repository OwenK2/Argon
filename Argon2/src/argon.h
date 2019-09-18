#ifndef ARGON_H
#define ARGON_H

#include <vector>
#include <algorithm>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "triangle.h"

#include "polygon.h"

using namespace std;

//WINDOW FLAGS
#define ARGON_FULLSCREEN 1
#define ARGON_HIDDEN 2
#define ARGON_BORDERLESS 4
#define ARGON_RESIZEABLE 8
#define ARGON_MINIMIZED 16
#define ARGON_MAXIMIZED 32
#define ARGON_HIGHDPI 64
#define ARGON_NOQUIT 128
#define ARGON_IMAGES 256

#define ARGON_BASIC 72

//MESSAGE FLAGS
#define ARGON_MESSAGE_ERROR SDL_MESSAGEBOX_ERROR
#define ARGON_MESSAGE_WARNING SDL_MESSAGEBOX_WARNING
#define ARGON_MESSAGE_INFO SDL_MESSAGEBOX_INFORMATION

class Argon;
class CachedImage;

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
	FOCUS,
	BLUR,
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
	bool ldown;
	bool mdown;
	bool rdown;
	bool wdown;
	uint8_t which;
};
struct MouseEvent : Event {
	int x;
	int y;
	bool down;
	bool ldown;
	bool mdown;
	bool rdown;
	bool wdown;
	uint8_t which;
};
struct Keyboard {
	int keyCode;
	const char* key;

	const uint8_t* state;

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

	const uint8_t* state;

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

struct Argon_Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	bool operator==(Argon_Color c) {
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}
	bool operator !=(Argon_Color c) {
		return r != c.r || g != c.g || b != c.b || a != c.a;
	}
	Argon_Color operator+(Argon_Color c) {
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}
	Argon_Color operator-(Argon_Color c) {
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;
		return *this;
	}
};
struct Argon_Rect {
	int x; int y; int w; int h;
};

typedef function<void(Argon&,WindowEvent&)> WindowListener;
typedef function<void(Argon&,MouseEvent&)> MouseListener;
typedef function<void(Argon&,KeyboardEvent&)> KeyboardListener;
typedef function<void(Argon&,WheelEvent&)> WheelListener;
typedef function<void(Argon&,FileEvent&)> FileListener;
typedef function<void(Argon&)> Task;
typedef vector<Task> Tasklist;


class Argon {
private:
	int fps;
	double frameTime;
	bool canCountClick = false;
	bool quitOnClose = true;
	bool skipCallstack = false;
	bool useImages = false;
	Argon_Color bg = {0,0,0,0};
	Argon_Color fill = {255,255,255,255};
	Argon_Color stroke = {255,255,255,255};
	uint32_t lastClick;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Surface* surface;

	//Listeners
	vector<WindowListener*> quitListeners;
	vector<WindowListener*> closeListeners;
	vector<WindowListener*> shownListeners;
	vector<WindowListener*> hiddenListeners;
	vector<WindowListener*> exposedListeners;
	vector<WindowListener*> movedListeners;
	vector<WindowListener*> resizedListeners;
	vector<WindowListener*> sizeChangedListeners;
	vector<WindowListener*> minimizedListeners;
	vector<WindowListener*> maximizedListeners;
	vector<WindowListener*> restoredListeners;
	vector<WindowListener*> focusListeners;
	vector<WindowListener*> blurListeners;
	vector<WindowListener*> takeFocusListeners;
	vector<WindowListener*> hitTestListeners;
	vector<MouseListener*> mouseEnterListeners;
	vector<MouseListener*> mouseLeaveListeners;
	vector<MouseListener*> mouseUpListeners;
	vector<MouseListener*> mouseDownListeners;
	vector<MouseListener*> mouseMoveListeners;
	vector<MouseListener*> clickListeners;
	vector<MouseListener*> dblclickListeners;
	vector<KeyboardListener*> keyUpListeners;
	vector<KeyboardListener*> keyDownListeners;
	vector<WheelListener*> mouseWheelListeners;
	vector<FileListener*> dropFileListeners;

	//Cache
	vector<CachedImage*> imageCache;

	//Tasklist
	Tasklist tasklist;
	Tasklist callstack;



	//Hidden Functions
	void init(int x, int y, int w, int h, int flags);

	static int eventWatcher(void* data, SDL_Event* e);

	friend class CachedImage;

public:
	const char* name;
	bool running = false;
	int dblClickTime = 400;
	string lastError = "";
	Window window = {-1, -1, -1, -1, -1, -1, 0};
	Mouse mouse = {0,0,0,0,0,0,0,false};
	Keyboard keyboard = {-1,"",{},false,false,false,false,false,false,false,false,false,false,false,false,false,false};

	Argon(const char* name, int fps, int flags);
	Argon(const char* name, int w, int h, int fps, int flags);
	Argon(const char* name, int x, int y, int w, int h, int fps, int flags);

	~Argon();

	void start();
	void quit();
	void close();

	void addWindowListener(EventType type, WindowListener listener);
	void addMouseListener(EventType type, MouseListener listener);
	void addKeyboardListener(EventType type, KeyboardListener listener);
	void addWheelListener(EventType type, WheelListener listener);
	void addFileListener(EventType type, FileListener listener);

	bool removeListener(EventType type, int index);

	void addLoop(Task task);
	bool removeLoop(int index);

	void setFps(int _fps);
	int getFps();

	//Add draw to call stack
	void clear();
	void clear(int x, int y, int w, int h);

	void point(int x, int y);

	void line(int x1, int y1, int x2, int y2);

	void strokeCircle(int x, int y, int r);
	void circle(int x, int y, int r);

	void strokeTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void scanLineTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void halfSpaceTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void triangle(int x1, int y1, int x2, int y2, int x3, int y3);

	void strokeRect(int x, int y, int w, int h);
	void rect(int x, int y, int w, int h);

	void strokePolygon(Points& points);
	void polygon(Points& points);

	Argon_Rect image(const char* path, int x, int y);
	Argon_Rect image(const char* path, int x, int y, int w, int h);
	Argon_Rect image(const char* path, int sx, int sy, int sw, int sh, int dx, int dy,int dw,int dh);

	void setBackground(Argon_Color color);
	void setBackground(int r, int g, int b, int a = 255);

	void setStroke(Argon_Color color);
	void setStroke(int r, int g, int b, int a = 255);

	void setFill(Argon_Color color);
	void setFill(int r, int g, int b, int a = 255);

	void wait(int ms);

	void messageBox(const char* title,const char* message, uint32_t flags = ARGON_MESSAGE_INFO);
};


//Friend Classes

class CachedImage {
public:
	const char* path;
	SDL_Surface* img;
	Argon& argon;

	CachedImage(Argon& a, const char* path);
	~CachedImage();
	Argon_Rect draw(int sx, int sy, int sw, int sh, int dx, int dy,int dw, int dh);
	void remove();
};

#endif
