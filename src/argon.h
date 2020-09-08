#ifndef ARGON_H
#include <stdexcept>
#include <unordered_map>
#include "Event.h"
#include "Color.h"

#define FULLSCREEN 1
#define FULLSCREEN_DESKTOP 2
#define HIDDEN 4
#define BORDERLESS 8
#define FIXED_SIZE 16
#define MINIMIZED 32
#define MAXIMIZED 64
#define ALWAYS_ON_TOP 128

typedef void (*ArgonLooper)(Argon&, void*);

class Argon {
private:
	bool running = false;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	//Window Properties
	const char* title = "Argon";
	int32_t x = (uint16_t)SDL_WINDOWPOS_CENTERED;
	int32_t y = (uint16_t)SDL_WINDOWPOS_CENTERED;
	uint16_t height = 500;
	uint16_t width = 500;

	//Timing 
	bool useVsync = true;
	uint16_t spf = 0;
	uint8_t fps = 60;
	uint32_t nextStep = SDL_GetTicks();

	//Events
	void eventWatcher(SDL_Event* event);
	static int resizeWatcher(void* data, SDL_Event* event);
	std::unordered_map<EventType, EventHandler> eventMap;
	int8_t getMouseButton(int32_t* mx = NULL, int32_t* my = NULL);

	//Argon Loop
	void* loopData = NULL;
	ArgonLooper gameStep = NULL;

	//Drawing Properties
	uint16_t strokeWidth = 1;
	Color fill;
	Color stroke;

public:
	//Constructors
	Argon(uint32_t flags = 0);
	Argon(const char* title, uint32_t flags = 0);
	Argon(const char* title, int32_t x, int32_t y, uint16_t w, uint16_t h, uint32_t flags = 0);
	Argon(const char* title, uint8_t fps, int32_t x, int32_t y, uint16_t w, uint16_t h, uint32_t flags = 0);

	//Destructor
	~Argon();

	

	//Loop Handling
	void start();
	void setLoop(ArgonLooper loop, void* data = NULL);
	void removeLoop();

	//Event Handling
	bool triggerEvent(Event event);
	void setEventHandler(EventType type, EventHandler handler);
	void removeEventHandler(EventType type);

	//Drawing Functions
	void drawRect(int32_t x, int32_t y, uint16_t w, uint16_t h);


	//Settings
	bool setRelativeMouse(bool val);

	//Setters
	void setWindowSize(uint16_t width, uint16_t height);
	void setWindowPosition(int32_t x, int32_t y);
	void setFill(Color& c);
	void setFill(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void setStroke(Color& c);
	void setStroke(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	void setStrokeWidth(uint16_t w);

	//Getters
	uint8_t getFps();
	int32_t getX();
	int32_t getY();
	uint16_t getWidth();
	uint16_t getHeight();
	Color& getFill();
	Color& getStroke();
	uint16_t getStrokeWidth();
};





#endif