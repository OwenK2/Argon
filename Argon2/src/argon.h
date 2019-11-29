#ifndef ARGON_H
#define ARGON_H

#include <cmath>
#include <algorithm>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "geo.h"

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

//KeyCode Flags
#define ARGON_UNKNOWN 0
#define ARGON_BACKSPACE 8
#define ARGON_TAB 9
#define ARGON_RETURN 13
#define ARGON_ESCAPE 27
#define ARGON_SPACE 32
#define ARGON_EXCLAIM 33
#define ARGON_QUOTEDBL 34
#define ARGON_HASH 35
#define ARGON_DOLLAR 36
#define ARGON_PERCENT 37
#define ARGON_AMPERSAND 38
#define ARGON_QUOTE 39
#define ARGON_LEFTPAREN 40
#define ARGON_RIGHTPAREN 41
#define ARGON_ASTERISK 42
#define ARGON_PLUS 43
#define ARGON_COMMA 44
#define ARGON_MINUS 45
#define ARGON_PERIOD 46
#define ARGON_SLASH 47
#define ARGON_0 48
#define ARGON_1 49
#define ARGON_2 50
#define ARGON_3 51
#define ARGON_4 52
#define ARGON_6 54
#define ARGON_7 55
#define ARGON_8 56
#define ARGON_9 57
#define ARGON_COLON 58
#define ARGON_SEMICOLON 59
#define ARGON_LESS 60
#define ARGON_EQUALS 61
#define ARGON_GREATER 62
#define ARGON_QUESTION 63
#define ARGON_AT 64
#define ARGON_LEFTBRACKET 91
#define ARGON_BACKSLASH 92
#define ARGON_RIGHTBRACKET 93
#define ARGON_CARET 94
#define ARGON_UNDERSCORE 95
#define ARGON_BACKQUOTE 96
#define ARGON_a 97
#define ARGON_b 98
#define ARGON_c 99
#define ARGON_d 100
#define ARGON_e 101
#define ARGON_f 102
#define ARGON_g 103
#define ARGON_h 104
#define ARGON_i 105
#define ARGON_j 106
#define ARGON_k 107
#define ARGON_l 108
#define ARGON_m 109
#define ARGON_n 110
#define ARGON_o 111
#define ARGON_p 112
#define ARGON_q 113
#define ARGON_r 114
#define ARGON_s 115
#define ARGON_t 116
#define ARGON_u 117
#define ARGON_v 118
#define ARGON_w 119
#define ARGON_x 120
#define ARGON_y 121
#define ARGON_z 122
#define ARGON_DELETE 127
#define ARGON_CAPSLOCK 1073741881
#define ARGON_F1 1073741882
#define ARGON_F2 1073741883
#define ARGON_F3 1073741884
#define ARGON_F4 1073741885
#define ARGON_F5 1073741886
#define ARGON_F6 1073741887
#define ARGON_F7 1073741888
#define ARGON_F8 1073741889
#define ARGON_F9 1073741890
#define ARGON_F10 1073741891
#define ARGON_F11 1073741892
#define ARGON_F12 1073741893
#define ARGON_PRINTSCREEN 1073741894
#define ARGON_SCROLLLOCK 1073741895
#define ARGON_PAUSE 1073741896
#define ARGON_INSERT 1073741897
#define ARGON_HOME 1073741898
#define ARGON_PAGEUP 1073741899
#define ARGON_END 1073741901
#define ARGON_PAGEDOWN 1073741902
#define ARGON_RIGHT 1073741903
#define ARGON_LEFT 1073741904
#define ARGON_DOWN 1073741905
#define ARGON_UP 1073741906
#define ARGON_NUMLOCKCLEAR 1073741907
#define ARGON_KP_DIVIDE 1073741908
#define ARGON_KP_MULTIPLY 1073741909
#define ARGON_KP_MINUS 1073741910
#define ARGON_KP_PLUS 1073741911
#define ARGON_KP_ENTER 1073741912
#define ARGON_KP_1 1073741913
#define ARGON_KP_2 1073741914
#define ARGON_KP_3 1073741915
#define ARGON_KP_4 1073741916
#define ARGON_KP_5 1073741917
#define ARGON_KP_6 1073741918
#define ARGON_KP_7 1073741919
#define ARGON_KP_8 1073741920
#define ARGON_KP_9 1073741921
#define ARGON_KP_0 1073741922
#define ARGON_KP_PERIOD 1073741923
#define ARGON_APPLICATION 1073741925
#define ARGON_POWER 1073741926
#define ARGON_KP_EQUALS 1073741927
#define ARGON_F13 1073741928
#define ARGON_F14 1073741929
#define ARGON_F15 1073741930
#define ARGON_F16 1073741931
#define ARGON_F17 1073741932
#define ARGON_F18 1073741933
#define ARGON_F19 1073741934
#define ARGON_F20 1073741935
#define ARGON_F21 1073741936
#define ARGON_F22 1073741937
#define ARGON_F23 1073741938
#define ARGON_F24 1073741939
#define ARGON_EXECUTE 1073741940
#define ARGON_HELP 1073741941
#define ARGON_MENU 1073741942
#define ARGON_SELECT 1073741943
#define ARGON_STOP 1073741944
#define ARGON_AGAIN 1073741945
#define ARGON_UNDO 1073741946
#define ARGON_CUT 1073741947
#define ARGON_COPY 1073741948
#define ARGON_PASTE 1073741949
#define ARGON_FIND 1073741950
#define ARGON_MUTE 1073741951
#define ARGON_VOLUMEUP 1073741952
#define ARGON_VOLUMEDOWN 1073741953
#define ARGON_KP_COMMA 1073741957
#define ARGON_KP_EQUALSAS400 1073741958
#define ARGON_ALTERASE 1073741977
#define ARGON_SYSREQ 1073741978
#define ARGON_CANCEL 1073741979
#define ARGON_CLEAR 1073741980
#define ARGON_PRIOR 1073741981
#define ARGON_RETURN2 1073741982
#define ARGON_SEPARATOR 1073741983
#define ARGON_OUT 1073741984
#define ARGON_OPER 1073741985
#define ARGON_CLEARAGAIN 1073741986
#define ARGON_CRSEL 1073741987
#define ARGON_EXSEL 1073741988
#define ARGON_KP_00 1073742000
#define ARGON_KP_000 1073742001
#define ARGON_THOUSANDSSEPARATOR 1073742002
#define ARGON_DECIMALSEPARATOR 1073742003
#define ARGON_CURRENCYUNIT 1073742004
#define ARGON_CURRENCYSUBUNIT 1073742005
#define ARGON_KP_LEFTPAREN 1073742006
#define ARGON_KP_RIGHTPAREN 1073742007
#define ARGON_KP_LEFTBRACE 1073742008
#define ARGON_KP_RIGHTBRACE 1073742009
#define ARGON_KP_TAB 1073742010
#define ARGON_KP_BACKSPACE 1073742011
#define ARGON_KP_A 1073742012
#define ARGON_KP_B 1073742013
#define ARGON_KP_C 1073742014
#define ARGON_KP_D 1073742015
#define ARGON_KP_E 1073742016
#define ARGON_KP_F 1073742017
#define ARGON_KP_XOR 1073742018
#define ARGON_KP_POWER 1073742019
#define ARGON_KP_PERCENT 1073742020
#define ARGON_KP_LESS 1073742021
#define ARGON_KP_GREATER 1073742022
#define ARGON_KP_AMPERSAND 1073742023
#define ARGON_KP_DBLAMPERSAND 1073742024
#define ARGON_KP_VERTICALBAR 1073742025
#define ARGON_KP_DBLVERTICALBAR 1073742026
#define ARGON_KP_COLON 1073742027
#define ARGON_KP_HASH 1073742028
#define ARGON_KP_SPACE 1073742029
#define ARGON_KP_AT 1073742030
#define ARGON_KP_EXCLAM 1073742031
#define ARGON_KP_MEMSTORE 1073742032
#define ARGON_KP_MEMRECALL 1073742033
#define ARGON_KP_MEMCLEAR 1073742034
#define ARGON_KP_MEMADD 1073742035
#define ARGON_KP_MEMSUBTRACT 1073742036
#define ARGON_KP_MEMMULTIPLY 1073742037
#define ARGON_KP_MEMDIVIDE 1073742038
#define ARGON_KP_PLUSMINUS 1073742039
#define ARGON_KP_CLEAR 1073742040
#define ARGON_KP_CLEARENTRY 1073742041
#define ARGON_KP_BINARY 1073742042
#define ARGON_KP_OCTAL 1073742043
#define ARGON_KP_DECIMAL 1073742044
#define ARGON_KP_HEXADECIMAL 1073742045
#define ARGON_LCTRL 1073742048
#define ARGON_LSHIFT 1073742049
#define ARGON_LALT 1073742050
#define ARGON_LGUI 1073742051
#define ARGON_RCTRL 1073742052
#define ARGON_RSHIFT 1073742053
#define ARGON_RALT 1073742054
#define ARGON_RGUI 1073742055
#define ARGON_MODE 1073742081
#define ARGON_AUDIONEXT 1073742082
#define ARGON_AUDIOPREV 1073742083
#define ARGON_AUDIOSTOP 1073742084
#define ARGON_AUDIOPLAY 1073742085
#define ARGON_AUDIOMUTE 1073742086
#define ARGON_MEDIASELECT 1073742087
#define ARGON_WWW 1073742088
#define ARGON_MAIL 1073742089
#define ARGON_CALCULATOR 1073742090
#define ARGON_COMPUTER 1073742091
#define ARGON_AC_SEARCH 1073742092
#define ARGON_AC_HOME 1073742093
#define ARGON_AC_BACK 1073742094
#define ARGON_AC_FORWARD 1073742095
#define ARGON_AC_STOP 1073742096
#define ARGON_AC_REFRESH 1073742097
#define ARGON_AC_BOOKMARKS 1073742098
#define ARGON_BRIGHTNESSDOWN 1073742099
#define ARGON_BRIGHTNESSUP 1073742100
#define ARGON_DISPLAYSWITCH 1073742101
#define ARGON_KBDILLUMTOGGLE 1073742102
#define ARGON_KBDILLUMDOWN 1073742103
#define ARGON_KBDILLUMUP 1073742104
#define ARGON_EJECT 1073742105
#define ARGON_SLEEP 1073742106
#define SDL_Keycode Constant Decimal Value

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

	void quadraticBezier(int x1, int y1, int cpx, int cpy, int x2, int y2);
	void cubicBezier(int x1, int y1, int cpx1, int cpy1, int cpx2, int cpy2, int x2, int y2);
	void nicBezier(Points& pts);

	void strokeCircle(int x, int y, int r);
	void circle(int x, int y, int r);

	void strokeTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void scanLineTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void halfSpaceTriangle(int x1, int y1, int x2, int y2, int x3, int y3); //not yet implemented
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
	Argon_Color getPixel(int x, int y);
};


//Friend Classes

class CachedImage {
public:
	bool loaded;
	int w;
	int h;
	const char* path;
	// SDL_Surface* img;
	SDL_Texture* img;
	Argon& argon;

	CachedImage(Argon& a, const char* path);
	~CachedImage();
	Argon_Rect draw(int sx, int sy, int sw, int sh, int dx, int dy,int dw, int dh);
	void remove();
};

#endif
