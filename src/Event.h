#ifndef EVENT_H
#define EVENT_H
#include <SDL2/SDL.h>

enum EventType {
	NO_EVENT,
	LOAD,
	QUIT,
	SHOW_WINDOW,
	HIDE_WINDOW,
	EXPOSE_WINDOW,
	MOVE_WINDOW,
	RESIZE_WINDOW,
	WINDOW_SIZE_CHANGED,
	MINIMIZE_WINDOW,
	MAXIMIZE_WINDOW,
	RESTORE_WINDOW,
	MOUSEENTER,
	MOUSELEAVE,
	GAIN_FOCUS,
	LOSE_FOCUS,
	CLOSE_WINDOW,
	TAKE_FOCUS,
	HIT_TEST,
	KEYDOWN,
	KEYUP,
	MOUSEMOVE,
	MOUSEDOWN,
	MOUSEUP,
	MOUSEWHEEL,
	DROPFILE,
	DROPTEXT,
	DROPBEGIN,
	DROPCOMPLETE
};

class Argon;
class Event;
typedef void (*EventHandler)(Argon&, Event);

struct Event {
public:
	EventType type = NO_EVENT;
	uint32_t timestamp;
	uint32_t windowID;

	union {
		struct { //Window Event involving position
			int32_t wx;
			int32_t wy;
		};
		struct { //Window Event involving width and height
			int32_t width;
			int32_t height;
		};
		struct { //Keyboard Event
			bool isKeyRepeat; //if key is held and triggers multiple presses
			bool isPressed; // if key is now down or up
			uint16_t keycode;
			uint16_t scancode;
			const char* key;
			bool lshift;
			bool rshift;
			bool shift;
			bool lctrl;
			bool rctrl;
			bool ctrl;
			bool lalt;
			bool ralt;
			bool alt;
			bool lmeta;
			bool rmeta;
			bool meta;
			bool numlock;
			bool capslock;
			bool altgr;
		};
		struct { //Mouse Events
			int8_t which;
			int32_t x;
			int32_t y;
			int32_t relx;
			int32_t rely;
			uint32_t clicks;
			bool down;
			bool isWheelInverted = false; // will only be set for wheel events
		};
		struct { //File Drop Events
			char* file; // could also be text
		};
	};

};


#endif