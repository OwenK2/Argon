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

//KeyCode Flags
#define Argon_UNKNOWN 0
#define Argon_BACKSPACE 8
#define Argon_TAB 9
#define Argon_RETURN 13
#define Argon_ESCAPE 27
#define Argon_SPACE 32
#define Argon_EXCLAIM 33
#define Argon_QUOTEDBL 34
#define Argon_HASH 35
#define Argon_DOLLAR 36
#define Argon_PERCENT 37
#define Argon_AMPERSAND 38
#define Argon_QUOTE 39
#define Argon_LEFTPAREN 40
#define Argon_RIGHTPAREN 41
#define Argon_ASTERISK 42
#define Argon_PLUS 43
#define Argon_COMMA 44
#define Argon_MINUS 45
#define Argon_PERIOD 46
#define Argon_SLASH 47
#define Argon_0 48
#define Argon_1 49
#define Argon_2 50
#define Argon_3 51
#define Argon_4 52
#define Argon_5 53
#define Argon_6 54
#define Argon_7 55
#define Argon_8 56
#define Argon_9 57
#define Argon_COLON 58
#define Argon_SEMICOLON 59
#define Argon_LESS 60
#define Argon_EQUALS 61
#define Argon_GREATER 62
#define Argon_QUESTION 63
#define Argon_AT 64
#define Argon_LEFTBRACKET 91
#define Argon_BACKSLASH 92
#define Argon_RIGHTBRACKET 93
#define Argon_CARET 94
#define Argon_UNDERSCORE 95
#define Argon_BACKQUOTE 96
#define Argon_a 97
#define Argon_b 98
#define Argon_c 99
#define Argon_d 100
#define Argon_e 101
#define Argon_f 102
#define Argon_g 103
#define Argon_h 104
#define Argon_i 105
#define Argon_j 106
#define Argon_k 107
#define Argon_l 108
#define Argon_m 109
#define Argon_n 110
#define Argon_o 111
#define Argon_p 112
#define Argon_q 113
#define Argon_r 114
#define Argon_s 115
#define Argon_t 116
#define Argon_u 117
#define Argon_v 118
#define Argon_w 119
#define Argon_x 120
#define Argon_y 121
#define Argon_z 122
#define Argon_DELETE 127
#define Argon_CAPSLOCK 1073741881
#define Argon_F1 1073741882
#define Argon_F2 1073741883
#define Argon_F3 1073741884
#define Argon_F4 1073741885
#define Argon_F5 1073741886
#define Argon_F6 1073741887
#define Argon_F7 1073741888
#define Argon_F8 1073741889
#define Argon_F9 1073741890
#define Argon_F10 1073741891
#define Argon_F11 1073741892
#define Argon_F12 1073741893
#define Argon_PRINTSCREEN 1073741894
#define Argon_SCROLLLOCK 1073741895
#define Argon_PAUSE 1073741896
#define Argon_INSERT 1073741897
#define Argon_HOME 1073741898
#define Argon_PAGEUP 1073741899
#define Argon_END 1073741901
#define Argon_PAGEDOWN 1073741902
#define Argon_RIGHT 1073741903
#define Argon_LEFT 1073741904
#define Argon_DOWN 1073741905
#define Argon_UP 1073741906
#define Argon_NUMLOCKCLEAR 1073741907
#define Argon_KP_DIVIDE 1073741908
#define Argon_KP_MULTIPLY 1073741909
#define Argon_KP_MINUS 1073741910
#define Argon_KP_PLUS 1073741911
#define Argon_KP_ENTER 1073741912
#define Argon_KP_1 1073741913
#define Argon_KP_2 1073741914
#define Argon_KP_3 1073741915
#define Argon_KP_4 1073741916
#define Argon_KP_5 1073741917
#define Argon_KP_6 1073741918
#define Argon_KP_7 1073741919
#define Argon_KP_8 1073741920
#define Argon_KP_9 1073741921
#define Argon_KP_0 1073741922
#define Argon_KP_PERIOD 1073741923
#define Argon_APPLICATION 1073741925
#define Argon_POWER 1073741926
#define Argon_KP_EQUALS 1073741927
#define Argon_F13 1073741928
#define Argon_F14 1073741929
#define Argon_F15 1073741930
#define Argon_F16 1073741931
#define Argon_F17 1073741932
#define Argon_F18 1073741933
#define Argon_F19 1073741934
#define Argon_F20 1073741935
#define Argon_F21 1073741936
#define Argon_F22 1073741937
#define Argon_F23 1073741938
#define Argon_F24 1073741939
#define Argon_EXECUTE 1073741940
#define Argon_HELP 1073741941
#define Argon_MENU 1073741942
#define Argon_SELECT 1073741943
#define Argon_STOP 1073741944
#define Argon_AGAIN 1073741945
#define Argon_UNDO 1073741946
#define Argon_CUT 1073741947
#define Argon_COPY 1073741948
#define Argon_PASTE 1073741949
#define Argon_FIND 1073741950
#define Argon_MUTE 1073741951
#define Argon_VOLUMEUP 1073741952
#define Argon_VOLUMEDOWN 1073741953
#define Argon_KP_COMMA 1073741957
#define Argon_KP_EQUALSAS400 1073741958
#define Argon_ALTERASE 1073741977
#define Argon_SYSREQ 1073741978
#define Argon_CANCEL 1073741979
#define Argon_CLEAR 1073741980
#define Argon_PRIOR 1073741981
#define Argon_RETURN2 1073741982
#define Argon_SEPARATOR 1073741983
#define Argon_OUT 1073741984
#define Argon_OPER 1073741985
#define Argon_CLEARAGAIN 1073741986
#define Argon_CRSEL 1073741987
#define Argon_EXSEL 1073741988
#define Argon_KP_00 1073742000
#define Argon_KP_000 1073742001
#define Argon_THOUSANDSSEPARATOR 1073742002
#define Argon_DECIMALSEPARATOR 1073742003
#define Argon_CURRENCYUNIT 1073742004
#define Argon_CURRENCYSUBUNIT 1073742005
#define Argon_KP_LEFTPAREN 1073742006
#define Argon_KP_RIGHTPAREN 1073742007
#define Argon_KP_LEFTBRACE 1073742008
#define Argon_KP_RIGHTBRACE 1073742009
#define Argon_KP_TAB 1073742010
#define Argon_KP_BACKSPACE 1073742011
#define Argon_KP_A 1073742012
#define Argon_KP_B 1073742013
#define Argon_KP_C 1073742014
#define Argon_KP_D 1073742015
#define Argon_KP_E 1073742016
#define Argon_KP_F 1073742017
#define Argon_KP_XOR 1073742018
#define Argon_KP_POWER 1073742019
#define Argon_KP_PERCENT 1073742020
#define Argon_KP_LESS 1073742021
#define Argon_KP_GREATER 1073742022
#define Argon_KP_AMPERSAND 1073742023
#define Argon_KP_DBLAMPERSAND 1073742024
#define Argon_KP_VERTICALBAR 1073742025
#define Argon_KP_DBLVERTICALBAR 1073742026
#define Argon_KP_COLON 1073742027
#define Argon_KP_HASH 1073742028
#define Argon_KP_SPACE 1073742029
#define Argon_KP_AT 1073742030
#define Argon_KP_EXCLAM 1073742031
#define Argon_KP_MEMSTORE 1073742032
#define Argon_KP_MEMRECALL 1073742033
#define Argon_KP_MEMCLEAR 1073742034
#define Argon_KP_MEMADD 1073742035
#define Argon_KP_MEMSUBTRACT 1073742036
#define Argon_KP_MEMMULTIPLY 1073742037
#define Argon_KP_MEMDIVIDE 1073742038
#define Argon_KP_PLUSMINUS 1073742039
#define Argon_KP_CLEAR 1073742040
#define Argon_KP_CLEARENTRY 1073742041
#define Argon_KP_BINARY 1073742042
#define Argon_KP_OCTAL 1073742043
#define Argon_KP_DECIMAL 1073742044
#define Argon_KP_HEXADECIMAL 1073742045
#define Argon_LCTRL 1073742048
#define Argon_LSHIFT 1073742049
#define Argon_LALT 1073742050
#define Argon_LGUI 1073742051
#define Argon_RCTRL 1073742052
#define Argon_RSHIFT 1073742053
#define Argon_RALT 1073742054
#define Argon_RGUI 1073742055
#define Argon_MODE 1073742081
#define Argon_AUDIONEXT 1073742082
#define Argon_AUDIOPREV 1073742083
#define Argon_AUDIOSTOP 1073742084
#define Argon_AUDIOPLAY 1073742085
#define Argon_AUDIOMUTE 1073742086
#define Argon_MEDIASELECT 1073742087
#define Argon_WWW 1073742088
#define Argon_MAIL 1073742089
#define Argon_CALCULATOR 1073742090
#define Argon_COMPUTER 1073742091
#define Argon_AC_SEARCH 1073742092
#define Argon_AC_HOME 1073742093
#define Argon_AC_BACK 1073742094
#define Argon_AC_FORWARD 1073742095
#define Argon_AC_STOP 1073742096
#define Argon_AC_REFRESH 1073742097
#define Argon_AC_BOOKMARKS 1073742098
#define Argon_BRIGHTNESSDOWN 1073742099
#define Argon_BRIGHTNESSUP 1073742100
#define Argon_DISPLAYSWITCH 1073742101
#define Argon_KBDILLUMTOGGLE 1073742102
#define Argon_KBDILLUMDOWN 1073742103
#define Argon_KBDILLUMUP 1073742104
#define Argon_EJECT 1073742105
#define Argon_SLEEP 1073742106
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
	Argon_Color getPixel(int x, int y);
};


//Friend Classes

class CachedImage {
public:
	bool loaded;
	const char* path;
	SDL_Surface* img;
	Argon& argon;

	CachedImage(Argon& a, const char* path);
	~CachedImage();
	Argon_Rect draw(int sx, int sy, int sw, int sh, int dx, int dy,int dw, int dh);
	void remove();
};

#endif
