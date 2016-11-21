
#pragma once

#include <GameLibrary/Types.h>

namespace fgl
{
	#define RGBA(r,g,b,a) (a | (b<<8) | (g<<16) | (r<<24))
	#define RGBX(r,g,b) RGBA(r,g,b,255)
	
	/*! Contains an RGBA color value*/
	class Color
	{
	public:
		/*! 32 bit RGBA constants*/
		typedef enum
		{
			BLACK = RGBX(0, 0, 0),
			BLUE = RGBX(0, 0, 255),
			CYAN = RGBX(0, 255, 255),
			DARKGRAY = RGBX(64, 64, 64),
			GRAY = RGBX(128, 128, 128),
			GREEN = RGBX(0, 255, 0),
			LIGHTGRAY = RGBX(192, 192, 192),
			MAGENTA = RGBX(255, 0, 255),
			ORANGE = RGBX(255, 200, 0),
			PINK = RGBX(255, 175, 175),
			RED = RGBX(255, 0, 0),
			WHITE = RGBX(255, 255, 255),
			YELLOW = RGBX(255, 255, 0),
			TRANSPARENT = RGBA(0, 0, 0, 0),
			
			ACIDGREEN = RGBX(176, 191, 26),
			ALICEBLUE = RGBX(240, 248, 255),
			ALMOND = RGBX(239, 222, 205),
			AMARANTH = RGBX(229, 43, 80),
			ANTIQUEWHITE = RGBX(250, 235, 215),
			AQUA = RGBX(0, 255, 255),
			AQUAMARINE = RGBX(127, 255, 212),
			AZURE = RGBX(240, 255, 255),
			BAKERSCHOCOLATE = RGBX(92, 51, 23),
			BEIGE = RGBX(245, 245, 220),
			BISQUE = RGBX(255, 228, 196),
			BLANCHEDALMOND = RGBX(255, 255, 205),
			BLUEVIOLET = RGBX(138, 43, 226),
			BRASS = RGBX(181, 166, 66),
			BROWN = RGBX(165, 42, 42),
			BURLYWOOD = RGBX(222, 184, 135),
			BURNTUMBER = RGBX(138, 51, 36),
			CADETBLUE = RGBX(95, 158, 160),
			CHARTREUSE = RGBX(127, 255, 0),
			CHOCOLATE = RGBX(210, 105, 30),
			COBALT = RGBX(61, 89, 171),
			COBALTGREEN = RGBX(61, 145, 64),
			COOLCOPPER = RGBX(217, 135, 25),
			COPPER = RGBX(184, 115, 51),
			CORAL = RGBX(255, 127, 80),
			CORNFLOWERBLUE = RGBX(100, 149, 237),
			CORNSILK = RGBX(255, 248, 220),
			CRIMSON = RGBX(220, 20, 60),
			DARKBLUE = RGBX(0, 0, 139),
			DARKCYAN = RGBX(0, 139, 139),
			DARKGOLDENROD = RGBX(184, 134, 11),
			DARKGREEN = RGBX(0, 100, 0),
			DARKKHAKI = RGBX(189, 183, 107),
			DARKMAGENTA = RGBX(139, 0, 139),
			DARKOLIVEGREEN = RGBX(85, 107, 47),
			DARKORANGE = RGBX(255, 140, 0),
			DARKORCHID = RGBX(153, 50, 204),
			DARKPURPLE = RGBX(135, 31, 120),
			DARKRED = RGBX(139, 0, 0),
			DARKSALMON = RGBX(233, 150, 122),
			DARKSEAGREEN = RGBX(143,188,143),
			DARKSLATEBLUE = RGBX(72, 61, 139),
			DARKSLATEGRAY = RGBX(40, 79, 79),
			DARKTURQUOISE = RGBX(0, 206, 209),
			DARKVIOLET = RGBX(148, 0, 211),
			DEEPPINK = RGBX(255, 20, 147),
			DEEPSKYBLUE = RGBX(0, 191, 255),
			DIMGRAY = RGBX(105, 105, 105),
			DODGERBLUE = RGBX(30, 144, 255),
			EBONY = RGBX(85, 93, 80),
			EGGPLANT = RGBX(97, 64, 81),
			FIREBRICK = RGBX(178, 34, 34),
			FLORALWHITE = RGBX(255, 250, 240),
			FORESTGREEN = RGBX(34, 139, 34),
			FUSCHIA = RGBX(255, 0, 255),
			GAINSBORO = RGBX(220, 220, 220),
			GHOSTWHITE = RGBX(248, 248, 255),
			GOLD = RGBX(255, 215, 0),
			GOLDENROD = RGBX(218, 165, 32),
			GREENYELLOW = RGBX(173, 255, 47),
			HONEYDEW = RGBX(240, 255, 240),
			HOTPINK = RGBX(255, 105, 180),
			INDIANRED = RGBX(205, 92, 92),
			INDIGO = RGBX(75, 0, 130),
			IRISBLUE = RGBX(3, 180, 200),
			IVORY = RGBX(255, 240, 240),
			KHAKI = RGBX(240, 230, 140),
			LAVENDER = RGBX(230, 230, 250),
			LAVENDERBLUSH = RGBX(255, 240, 245),
			LAWNGREEN = RGBX(124, 252, 0),
			LEMONCHIFFON = RGBX(255, 250, 205),
			LIGHTBLUE = RGBX(173, 216, 230),
			LIGHTCORAL = RGBX(240, 128, 128),
			LIGHTCYAN = RGBX(224, 255, 255),
			LIGHTGOLDENRODYELLOW = RGBX(250, 250, 210),
			LIGHTGREEN = RGBX(144, 238, 144),
			LIGHTPINK = RGBX(255, 182, 193),
			LIGHTSALMON = RGBX(255, 160, 122),
			LIGHTSEAGREEN = RGBX(32, 178, 170),
			LIGHTSKYBLUE = RGBX(135, 206, 250),
			LIGHTSLATEGRAY = RGBX(119, 136, 153),
			LIGHTSTEELBLUE = RGBX(176, 196, 222),
			LIGHTYELLOW = RGBX(255, 255, 224),
			LIME = RGBX(0, 255, 0),
			LIMEGREEN = RGBX(50, 205, 50),
			LINEN = RGBX(250, 240, 230),
			MAROON = RGBX(128, 0, 0),
			MEDIUMAQUAMARINE = RGBX(102, 205, 170),
			MEDIUMBLUE = RGBX(102, 205, 170),
			MEDIUMORCHID = RGBX(186, 85, 211),
			MEDIUMPURPLE = RGBX(147, 112, 219),
			MEDIUMSEAGREEN = RGBX(60, 179, 113),
			MEDIUMSLATEBLUE = RGBX(123, 104, 238),
			MEDIUMSPRINGGREEN = RGBX(0, 250, 154),
			MEDIUMTURQUOISE = RGBX(72, 209, 204),
			MEDIUMVIOLETRED = RGBX(199, 21, 112),
			MIDNIGHTBLUE = RGBX(25, 25, 112),
			MINTCREAM = RGBX(245, 255, 250),
			MISTYROSE = RGBX(255, 228, 225),
			MOCCASIN = RGBX(255, 228, 181),
			NAVAJOWHITE = RGBX(255, 222, 173),
			NAVY = RGBX(0, 0, 128),
			NEONBLUE = RGBX(77, 77, 255),
			NEONGREEN = RGBX(57, 255, 20),
			OLDLACE = RGBX(253, 245, 230),
			OLIVE = RGBX(128, 128, 0),
			OLIVEDRAB = RGBX(107, 142, 45),
			ORANGERED = RGBX(255, 69, 0),
			ORCHID = RGBX(218, 112, 214),
			PALEGOLDENROD = RGBX(238, 232, 170),
			PALEGREEN = RGBX(152, 251, 152),
			PALETURQUOISE = RGBX(175, 238, 238),
			PALEVIOLETRED = RGBX(219, 112, 147),
			PAPAYAWHIP = RGBX(255, 239, 213),
			PEACHPUFF = RGBX(255, 218, 155),
			PERU = RGBX(205, 133, 63),
			PLUM = RGBX(221, 160, 221),
			POWDERBLUE = RGBX(176, 224, 230),
			PURPLE = RGBX(128, 128, 128),
			QUARTZ = RGBX(217, 217, 243),
			RASPBERRY = RGBX(135, 38, 87),
			ROSYBROWN = RGBX(188, 143, 143),
			ROYALBLUE = RGBX(65, 105, 255),
			SADDLEBROWN = RGBX(139, 69, 19),
			SAPGREEN = RGBX(48, 128, 20),
			SALMON = RGBX(250, 128, 114),
			SANDYBROWN = RGBX(244, 264, 96),
			SCARLET = RGBX(140, 23, 23),
			SEAGREEN = RGBX(46,	139, 87),
			SEASHELL = RGBX(255, 245, 238),
			SEMISWEETCHOCOLATE = RGBX(107, 66, 38),
			SEPIA = RGBX(94, 38, 18),
			SIENNA = RGBX(160, 82, 45),
			SILVER = RGBX(192, 192, 192),
			SKYBLUE = RGBX(135, 206, 235),
			SLATEBLUE = RGBX(106, 90, 205),
			SLATEGRAY = RGBX(112, 128, 144),
			SNOW = RGBX(255, 250, 250),
			SPICYPINK = RGBX(255, 28, 174),
			SPRINGGREEN = RGBX(0, 255, 127),
			STEELBLUE = RGBX(70, 130, 180),
			SUMMERSKYBLUE = RGBX(56, 176, 222),
			TAN = RGBX(210, 180, 140),
			TEAL = RGBX(0, 128, 128),
			THISTLE = RGBX(216, 191, 216),
			TOMATO = RGBX(253, 99, 71),
			TURQUOISE = RGBX(64, 224, 208),
			VIOLET = RGBX(238, 130, 238),
			VIOLETRED = RGBX(208, 32, 144),
			WARMGRAY = RGBX(128, 128, 105),
			WHEAT = RGBX(245, 222, 179),
			WHITESMOKE = RGBX(245, 245, 245),
			YELLOWGREEN = RGBX(154, 205, 50),
		} RGBA32;
		
		/*! alpha value. default value is 255*/
		byte a;
		/*! blue value. default value is 255*/
		byte b;
		/*! green value. default value is 255*/
		byte g;
		/*! red value. default value is 255*/
		byte r;
		
		/*! default constructor*/
		Color();
		/*! copy constructor*/
		Color(const Color&);
		/*! Constructs a Color object from an RGBA32 constant
			\param rgba an RGBA 32 bit constant constant*/
		Color(const fgl::Color::RGBA32&rgba);
		/*! Constructs a Color object with an rgba value
			\param r the red value
			\param g the green value
			\param b the blue value
			\param a the alpha value*/
		Color(byte r, byte g, byte b, byte a=255);
		
		
		/*! assignment operator*/
		Color&operator=(const Color&);
		/*! assignment operator*/
		Color&operator=(const fgl::Color::RGBA32&rgba);
		/*! equality operator*/
		bool operator==(const Color&) const;
		/*! inequality operator*/
		bool operator!=(const Color&) const;
		
		
		/*! Creates a 32 bit RGBA integer
			\returns an unsigned 32 bit integer*/
		fgl::Uint32 getRGBA() const;
		/*! Creates a 32 bit ARGB integer
			\returns an unsigned 32 bit integer*/
		fgl::Uint32 getARGB() const;
		/*! Creates a 32 bit ABGR integer
			\returns an unsigned 32 bit integer*/
		fgl::Uint32 getABGR() const;
		/*! Creates a 32 bit BGRA integer
			\returns an unsigned 32 bit integer*/
		fgl::Uint32 getBGRA() const;
		
		
		/*! Compares another Color to check equality.
			\param color the color to compare
			\returns true if the Color objects are equal, and false if they are not equal*/
		bool equals(const Color&color) const;
		
		/*! Creates a Color object by compositing a Color on top of the current Color. this->r *= (color.r/255);
			\param comp the Color to composite on top of the current Color
			\returns a Color object with the given color composited on top of it.*/
		Color composite(const Color&comp) const;

		/*! Inverts the color to its negative
			\returns a negative Color*/
		Color negative(bool alpha=false) const;
		
		/*! Generates a random Color value.
			\param alpha true to randomize the alpha value, false to only randomize the RGB values
			\returns a random Color value*/
		static Color random(bool alpha=false);
	};
}