#include <functional>
#include <SDL2/SDL.h>
using namespace std;

struct Color {
	char r;
	char g;
	char b;
	char a;
};

class Argon {
private:
	function<void(Argon*)> init;
	function<void(Argon*,SDL_Event)> eventHandler;
	function<void(Argon*)> program;
public:
	int w = 500;
	int h = 500;
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	Argon(const char* _name, int fps, );
	~Argon();
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
