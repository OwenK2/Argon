#ifndef COLOR_H
#define COLOR_H
#include <cstring>

#define TRANSPARENT 0,0,0,0
#define WHITE_TRANSPARENT 255,255,255,0
#define BLACK_TRANSPARENT 0,0,0,0
#define ALICEBLUE 240,248,255
#define ANTIQUEWHITE 250,235,215
#define AQUA 0,255,255
#define AQUAMARINE 127,255,212
#define AZURE 240,255,255
#define BEIGE 245,245,220
#define BISQUE 255,228,196
#define BLACK 0,0,0
#define BLANCHEDALMOND 255,235,205
#define BLUE 0,0,255
#define BLUEVIOLET 138,43,226
#define BROWN 165,42,42
#define BURLYWOOD 222,184,135
#define CADETBLUE 95,158,160
#define CHARTREUSE 127,255,0
#define CHOCOLATE 210,105,30
#define CORAL 255,127,80
#define CORNFLOWERBLUE 100,149,237
#define CORNSILK 255,248,220
#define CRIMSON 220,20,60
#define CYAN 0,255,255
#define DARKBLUE 0,0,139
#define DARKCYAN 0,139,139
#define DARKGOLDENROD 184,134,11
#define DARKGRAY 169,169,169
#define DARKGREEN 0,100,0
#define DARKKHAKI 189,183,107
#define DARKMAGENTA 139,0,139
#define DARKOLIVEGREEN 85,107,47
#define DARKORANGE 255,140,0
#define DARKORCHID 153,50,204
#define DARKRED 139,0,0
#define DARKSALMON 233,150,122
#define DARKSEAGREEN 143,188,143
#define DARKSLATEBLUE 72,61,139
#define DARKSLATEGRAY 47,79,79
#define DARKTURQUOISE 0,206,209
#define DARKVIOLET 148,0,211
#define DEEPPINK 255,20,147
#define DEEPSKYBLUE 0,191,255
#define DIMGRAY 105,105,105
#define DODGERBLUE 30,144,255
#define FIREBRICK 178,34,34
#define FLORALWHITE 255,250,240
#define FORESTGREEN 34,139,34
#define FUCHSIA 255,0,255
#define GAINSBORO 220,220,220
#define GHOSTWHITE 248,248,255
#define GOLD 255,215,0
#define GOLDENROD 218,165,32
#define GRAY 128,128,128
#define GREEN 0,128,0
#define GREENYELLOW 173,255,47
#define HONEYDEW 240,255,240
#define HOTPINK 255,105,180
#define INDIANRED 205,92,92
#define INDIGO 75,0,130
#define IVORY 255,255,240
#define KHAKI 240,230,140
#define LAVENDER 230,230,250
#define LAVENDERBLUSH 255,240,245
#define LAWNGREEN 124,252,0
#define LEMONCHIFFON 255,250,205
#define LIGHTBLUE 173,216,230
#define LIGHTCORAL 240,128,128
#define LIGHTCYAN 224,255,255
#define LIGHTGOLDENRODYELLOW 250,250,210
#define LIGHTGREEN 144,238,144
#define LIGHTGREY 211,211,211
#define LIGHTPINK 255,182,193
#define LIGHTSALMON 255,160,122
#define LIGHTSEAGREEN 32,178,170
#define LIGHTSKYBLUE 135,206,250
#define LIGHTSLATEGRAY 119,136,153
#define LIGHTSTEELBLUE 176,196,222
#define LIGHTYELLOW 255,255,224
#define LIME 0,255,0
#define LIMEGREEN 50,205,50
#define LINEN 250,240,230
#define MAGENTA 255,0,255
#define MAROON 128,0,0
#define MEDIUMAUQAMARINE 102,205,170
#define MEDIUMBLUE 0,0,205
#define MEDIUMORCHID 186,85,211
#define MEDIUMPURPLE 147,112,216
#define MEDIUMSEAGREEN 60,179,113
#define MEDIUMSLATEBLUE 123,104,238
#define MEDIUMSPRINGGREEN 0,250,154
#define MEDIUMTURQUOISE 72,209,204
#define MEDIUMVIOLETRED 199,21,133
#define MIDNIGHTBLUE 25,25,112
#define MINTCREAM 245,255,250
#define MISTYROSE 255,228,225
#define MOCCASIN 255,228,181
#define NAVAJOWHITE 255,222,173
#define NAVY 0,0,128
#define OLDLACE 253,245,230
#define OLIVE 128,128,0
#define OLIVEDRAB 104,142,35
#define ORANGE 255,165,0
#define ORANGERED 255,69,0
#define ORCHID 218,112,214
#define PALEGOLDENROD 238,232,170
#define PALEGREEN 152,251,152
#define PALETURQUOISE 175,238,238
#define PALEVIOLETRED 216,112,147
#define PAPAYAWHIP 255,239,213
#define PEACHPUFF 255,218,185
#define PERU 205,133,63
#define PINK 255,192,203
#define PLUM 221,160,221
#define POWDERBLUE 176,224,230
#define PURPLE 128,0,128
#define RED 255,0,0
#define ROSYBROWN 188,143,143
#define ROYALBLUE 65,105,225
#define SADDLEBROWN 139,69,19
#define SALMON 250,128,114
#define SANDYBROWN 244,164,96
#define SEAGREEN 46,139,87
#define SEASHELL 255,245,238
#define SIENNA 160,82,45
#define SILVER 192,192,192
#define SKYBLUE 135,206,235
#define SLATEBLUE 106,90,205
#define SLATEGRAY 112,128,144
#define SNOW 255,250,250
#define SPRINGGREEN 0,255,127
#define STEELBLUE 70,130,180
#define TAN 210,180,140
#define TEAL 0,128,128
#define THISTLE 216,191,216
#define TOMATO 255,99,71
#define TURQUOISE 64,224,208
#define VIOLET 238,130,238
#define WHEAT 245,222,179
#define WHITE 255,255,255
#define WHITESMOKE 245,245,245
#define YELLOW 255,255,0
#define YELLOWGREEN 154,205,50

struct Color {
	uint8_t r = 255;
	uint8_t g = 255;
	uint8_t b = 255;
	uint8_t a = 255;

	Color() {}
	Color(uint8_t c) : r(c), g(c), b(c) {};
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {};
	Color(const char* hex) {
		uint32_t value = 0;
		uint8_t len = strlen(hex);
		for(uint8_t i = hex[0] == '#' ? 1 : 0;i < len;++i) {
			value = (value << 4) | (hex[i] <= '9' ? hex[i] - '0' : (hex[i] <= 'F' ? hex[i] - 'A' + 10 : hex[i] - 'a' + 10));
		}
		if(len < 8) {
			value = (value << 8) | 0xFF;
		}
		r = (value >> 24) & 0xFF;
		g = (value >> 16) & 0xFF;
		b = (value >> 8) & 0xFF;
		a = value & 0xFF;
	};
	Color(const Color& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}

	~Color() {};

	Color& operator=(const Color& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
		return *this;
	}

};

#endif