#ifndef ARGON_H
#define ARGON_H

#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
#define ARGON_TEXT 528

#define ARGON_BASIC 72

class Argon;
class Argon_Drawable;
class Argon_Image;

enum MSGBoxType {
	MSG_ERROR = SDL_MESSAGEBOX_ERROR,
	MSG_WARN = SDL_MESSAGEBOX_WARNING,
	MSG_INFO = SDL_MESSAGEBOX_INFORMATION
};
enum MSGBoxBtnType {
	NONE = 0,
	BTN_RETURN = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
	BTN_ESC = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
};
struct Argon_Button {
	MSGBoxBtnType flag;
	int id;
	const char* txt;
};
enum Argon_Flip {
	ARGON_FLIP_NONE = SDL_FLIP_NONE,
	ARGON_FLIP_HORIZONTAL = SDL_FLIP_HORIZONTAL,
	ARGON_FLIP_VERTICAL = SDL_FLIP_VERTICAL,
	ARGON_FLIP_BOTH = ARGON_FLIP_VERTICAL|ARGON_FLIP_HORIZONTAL
};
enum EventType {
	LOAD,
	QUIT,
	CLOSE,
	SHOWN,
	HIDDEN,
	EXPOSED,
	MOVED,
	RESIZE,
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
struct WindowEvent : Event {
	int x;
	int y;
	int w;
	int h;
	bool shown;
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
struct Argon_Key {
	const char* value;
	int code;
	Argon_Key(const char* val, int code) : value(val), code(code) {}
	bool operator==(const char* rhs) {return strcmp(rhs,value) == 0;}
	bool operator==(int rhs) {return rhs == code;}
};
class Argon_Key_State {
private:
	const uint8_t* states = NULL;
public:
	bool operator[](int index) {
		return states[SDL_GetScancodeFromKey(index)];
	}
	bool operator[](char c) {
		const char str[2] = {c, '\0'};
		return states[SDL_GetScancodeFromName(str)];
	}
	bool operator[](const char* str) {
		return states[SDL_GetScancodeFromName(str)];
	}
friend class Argon;
};
struct KeyboardEvent : Event {
	Argon_Key key;
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

struct Argon_Color {
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0;
};
struct Argon_Rect {
	int x, y, w, h;
};

typedef std::function<void(Argon&,WindowEvent&)> WindowListener;
typedef std::function<void(Argon&,MouseEvent&)> MouseListener;
typedef std::function<void(Argon&,KeyboardEvent&)> KeyboardListener;
typedef std::function<void(Argon&,WheelEvent&)> WheelListener;
typedef std::function<void(Argon&,FileEvent&)> FileListener;
typedef std::function<void(Argon&)> Task;

class Argon {
private:
	const char* name;
	int sdl_flags = 0;
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Surface* surface;
	bool running = false;
	double frameTime;
	bool quitOnClose = true;
	bool canCountClick = false;
	bool imagesEnabled = false;
	uint16_t dblClickTime = 400;
	uint32_t lastClick = 0;
	Argon_Color bgColor = {0,0,0,0};
	Argon_Color fillColor = {255,255,255,255};
	Argon_Color strokeColor = {255,255,255,255};
	int strokeWeight = 1;
	bool doFill = true;
	bool doStroke = true;

	WindowListener loadListener = NULL;
	WindowListener quitListener = NULL;
	WindowListener closeListener = NULL;
	WindowListener shownListener = NULL;
	WindowListener hiddenListener = NULL;
	WindowListener exposedListener = NULL;
	WindowListener movedListener = NULL;
	WindowListener resizeListener = NULL;
	WindowListener sizeChangedListener = NULL;
	WindowListener minimizedListener = NULL;
	WindowListener maximizedListener = NULL;
	WindowListener restoredListener = NULL;
	WindowListener focusListener = NULL;
	WindowListener blurListener = NULL;
	WindowListener takeFocusListener = NULL;
	WindowListener hitTestListener = NULL;
	MouseListener mouseEnterListener = NULL;
	MouseListener mouseLeaveListener = NULL;
	MouseListener mouseUpListener = NULL;
	MouseListener mouseDownListener = NULL;
	MouseListener mouseMoveListener = NULL;
	MouseListener clickListener = NULL;
	MouseListener dblclickListener = NULL;
	KeyboardListener keyUpListener = NULL;
	KeyboardListener keyDownListener = NULL;
	WheelListener mouseWheelListener = NULL;
	FileListener dropFileListener = NULL;

	Task mainLoop = NULL;

	void gameLoop();
	void eventWatcher(SDL_Event* e);
	static int resizeWatcher(void* data, SDL_Event* e);

public:
	int x;
	int y;
	int w;
	int h;
	bool shown;
	Mouse mouse;
	Argon_Key_State keys;

	Argon();
	Argon(const char* name);
	Argon(const char* name, int flags);
	Argon(const char* name, int fps, int flags);
	Argon(const char* name, int w, int h, int flags);
	Argon(const char* name, int w, int h, int fps, int flags);
	Argon(const char* name, int x, int y, int w, int h, int fps, int flags);
	~Argon();

	//Window API
	void start();
	void resume();
	void close();
	void quit();
	void grabMouse(bool grab);
	void notification(const char* title, const char* msg, MSGBoxType type);
	int prompt(const char* title, const char* msg, int numBtns, Argon_Button* buttons, MSGBoxType type);

	//Loop API
	void setLoop(Task loop);
	void removeLoop();
	void setFPS(int fps);

	//Event API
	void setListener(EventType type, WindowListener listner);
	void setListener(EventType type, MouseListener listner);
	void setListener(EventType type, KeyboardListener listner);
	void setListener(EventType type, WheelListener listner);
	void setListener(EventType type, FileListener listner);
	void removeListener(EventType type);
	void setDblClickDebounce(uint16_t time);

	//Drawing API
	void clear();
	void point(int x, int y);
	void line(int x1, int y1, int x2, int y2);
	void circle(int cx, int cy, int r);
	Argon_Color getPixel(int x, int y);
	void putPixel(int x, int y, Argon_Color& color);
	void getPixelData(uint8_t* data, Argon_Rect* rect = NULL);
	void putPixelData(uint8_t* data, Argon_Rect* rect = NULL);
	void getPixelData(uint8_t* data, int x, int y, int w, int h);
	void putPixelData(uint8_t* data, int x, int y, int w, int h);

	//Drawing Settings API
	void fill();
	void noFill();
	void stroke();
	void noStroke();
	void setFill(uint8_t r,uint8_t g,uint8_t b,uint8_t a = 255);
	void setFill(Argon_Color& color);
	void setStroke(uint8_t r,uint8_t g,uint8_t b,uint8_t a = 255);
	void setStroke(Argon_Color& color);
	void setBackground(uint8_t r,uint8_t g,uint8_t b,uint8_t a = 255);
	void setBackground(Argon_Color& color);
	void setStrokeWeight(int weight);

	//Color modifier functions go here
	//- hue rotate
	//- brightness setter
	//- saturation setter

	//Helper API
	static const char* getEventTypeName(EventType type);

friend class Argon_Drawable;
friend class Argon_Image;
};

class Argon_Drawable {
protected:
	int w;
	int h;
	uint8_t alpha = 255;
	Argon_Flip flip = ARGON_FLIP_NONE;
	double angle = 0;
	SDL_Rect crop_rect;
	SDL_Rect* crop_ptr = NULL;
	SDL_Point center;
	SDL_Point* center_ptr = NULL;
	

	Argon& context;
	SDL_Texture* texture;

public:
	Argon_Drawable(Argon& context, SDL_Texture* texture);
	virtual ~Argon_Drawable();

	void render(Argon_Rect* rect = NULL);
	void render(int x, int y);
	void render(int x, int y, int w, int h);
	void render(int& x, int& y, int& w, int& h);
	void renderEx(double angle, Argon_Flip flip = ARGON_FLIP_NONE);
	void renderEx(Argon_Rect* rect = NULL, double angle = 0, Argon_Flip flip = ARGON_FLIP_NONE);
	void setAlpha(uint8_t alpha);
	uint8_t getAlpha();
	void modifyColor(uint8_t r, uint8_t g, uint8_t b);
	void setAngle(double angle);
	double getAngle();
	void setFlip(Argon_Flip flip);
	Argon_Flip getFlip();
	void crop(Argon_Rect* rect = NULL);
	void crop(int x, int y, int w, int h);
	void resetCrop();
	void setRotateCenter(int cx, int cy);
	void resetRotateCenter();
};

class Argon_Image : public Argon_Drawable {
private:
	const char* path;
public:
	Argon_Image(Argon& context, const char* path);

	const char* getFileName();
	void queryImage(int* w, int* h);
};


#endif