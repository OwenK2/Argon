#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define ARGON_FULLSCREEN 1
#define ARGON_SHOWN 4
#define ARGON_HIDDEN 8
#define ARGON_BORDERLESS 16
#define ARGON_RESIZEABLE 32
#define ARGON_MINIMIZED 64
#define ARGON_MAXIMIZED 128
#define ARGON_HIGHDPI 8192
#define ARGON_BASIC 8196

struct Color {
	char r;
	char g;
	char b;
	char a;
};
struct Mouse {
	int x;
	int y;
	uint8_t which;
	bool down;
};
struct Keyboard {
	int key;
	bool alt;
	bool meta;
	bool shift;
	bool ctrl;
};
struct Wheel {
	int x;
	int y;
	int dir;
};
enum EventType {
	LOAD = 0,
	SHOWN = 1,
	HIDDEN = 2,
	EXPOSED = 3,
	MOVED = 4,
	RESIZED = 5,
	SIZE_CHANGED = 6,
	MINIMIZED = 7,
	MAXIMIZED = 8,
	MOUSEENTER = 9,
	MOUSELEAVE = 10,
	CLOSE = 11,
	MOUSEUP = 12,
	MOUSEDOWN = 13,
	MOUSEMOVE = 14,
	MOUSEWHEEL = 15,
	CLICK = 16,
	KEYUP = 17,
	KEYDOWN = 18,
	QUIT = 19
};
struct Event {
	EventType type;
	Mouse mouse;
	Keyboard keyboard;
	Wheel wheel;
};


class Argon {
private:
	const char* name;
	int frameTime;				//Precalculated to save time, time per frame
	SDL_Window* win;
	SDL_Renderer* ren;
	std::function<void(Argon*,Event)> eventHandler;
	std::function<void(Argon*)> renderLoop;
	void start(int flags);
	void loop();
	static int eventWatcher(void* data, SDL_Event* e);
public:
	bool running = false;
	int fps = 60;
	int w = 500;
	int h = 500;
	int dw = 500;
	int dh = 500;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	Mouse mouse = {0,0,0,false};
	Keyboard keyboard = {-1,false,false,false};
	Argon(const char* _name, int _fps, uint32_t _flags,void _event(Argon* a, Event e),void _loop(Argon* a));
	Argon(const char* _name, int _w, int _h, int _fps, uint32_t _flags, void _event(Argon* a, Event e),void _loop(Argon* a));
	Argon(const char* _name, int _x, int _y, int _w, int _h, int _fps, uint32_t _flags, void _event(Argon* a, Event e),void _loop(Argon* a));
	~Argon();

	void quit();
};

// Color fillStyle;
// Color strokeStyle;
// LineCap lineCap;
// char globalAlpha;
// //STORE FONT FAMILY HERE
// int lineWidth;
// //RECTS
// void rect(int x, int y, int w, int h);
// void fillRect(int x, int y, int w, int h);
// void strokeRect(int x, int y, int w, int h);
// //CLEAR
// void clearRect(int x, int y, int w, int h);
// void clear();
// //DRAWING CALCULATIONS
// void fill();			//Fill last created figure or figure created with begin path
// void stroke();		//same rules as fill but for stroke
// //PATHS
// void beginPath();
// void moveTo(int x, int y);
// void closePath(); 		//Close current figure by connecting to start point
// void lineTo(int x, int y);
// void quadraticCurveTo(int cpx,int cpy,int x,int y);
// void bezierCurveTo(int cp1x,int cp1y,int cp2x,int cp2y,int x,int y);
// void arc(int x,int y,int r,float sAngle,float eAngle,bool counterclockwise);
// void arcTo(int x1,int y1,int x2,int y2,int r);
// void isPointInPath(int x, int y);			//Check if point is in current shape
// //TRANSFORMATIONS (on current shape)
// void scale(int sx, int sy);
// void rotate(float angle);
// void translate(int x, int y);
// void setTransform(); //replace current transform matrix with new one
// //TEXT
// void fillText(char* text,int x, int y);
// void strokeText(char* text,int x, int y);
// int measureText(char* text);
// //IMAGE
// void drawImage(char* img,int sx,int sy,int swidth,int sheight,int x,int y,int width,int height);
// void drawImage(char* img,int x,int y,int width,int height);
// void drawImage(char* img,int x,int y);
// //DATA
// ImageData getImageData();
// void putImageData(ImageData data);
// //FUNCTIONS
// void save();		//Save current context settings
// void restore();	//restore saved context settings
