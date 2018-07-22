
#pragma once

#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/Dictionary.hpp>

namespace fgl
{
	#define RGBA(r,g,b,a) (a##U | (b<<8) | (g<<16) | (r<<24))
	#define RGBX(r,g,b) RGBA(r,g,b,255)
	
	/*! Contains an RGBA color value*/
	class Color
	{
	public:
		/*! alpha value. default value is 255*/
		Uint8 a;
		/*! blue value. default value is 255*/
		Uint8 b;
		/*! green value. default value is 255*/
		Uint8 g;
		/*! red value. default value is 255*/
		Uint8 r;
		
		/*! default constructor*/
		constexpr Color()
			: a(255), b(255), g(255), r(255)
		{
			//
		}
		/*! Constructs a Color object from an RGBA32 constant
			\param rgba an RGBA 32 bit constant constant
			\param includeAlpha indicates whether to include the alpha bit (true), or to default to an alpha of 255 (false) */
		constexpr Color(Uint32 rgba, bool includeAlpha=true)
			: a(includeAlpha ? (rgba & 0xff) : 255), b((rgba >> 8) & 0xff), g((rgba >> 16) & 0xff), r((rgba >> 24) & 0xff)
		{
			//
		}
		/*! Constructs a Color object with an rgba value
			\param r the red value
			\param g the green value
			\param b the blue value
			\param a the alpha value*/
		constexpr Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255)
			: a(a), b(b), g(g), r(r)
		{
			//
		}
		/*! Constructs a Color object with a given Dictionary of r,g,b,a values
			\param colorValues a Dictionary of r,g,b,a values for this color
			\example
				<key>r</key>
				<integer>0</integer>
				<key>g</key>
				<integer>0</integer>
				<key>b</key>
				<integer>0</integer> */
		explicit Color(const Dictionary& colorValues);
		
		
		/*! assignment operator*/
		Color&operator=(Uint32 rgba);
		/*! equality operator*/
		bool operator==(const Color&) const;
		/*! inequality operator*/
		bool operator!=(const Color&) const;
		
		
		/*! Creates a 32 bit RGBA integer
			\returns an unsigned 32 bit integer*/
		Uint32 getRGBA() const;
		/*! Creates a 32 bit ARGB integer
			\returns an unsigned 32 bit integer*/
		Uint32 getARGB() const;
		/*! Creates a 32 bit ABGR integer
			\returns an unsigned 32 bit integer*/
		Uint32 getABGR() const;
		/*! Creates a 32 bit BGRA integer
			\returns an unsigned 32 bit integer*/
		Uint32 getBGRA() const;
		
		
		/*! Compares another Color to check equality.
			\param color the color to compare
			\returns true if the Color objects are equal, and false if they are not equal*/
		bool equals(const Color& color) const;
		
		/*! Creates a Color object by compositing a Color on top of the current Color. this->r *= (color.r/255);
			\param comp the Color to composite on top of the current Color
			\returns a Color object with the given color composited on top of it.*/
		Color composite(const Color& comp) const;

		/*! Inverts the color to its negative
			\returns a negative Color*/
		Color negative(bool alpha=false) const;
		
		/*! Gives the color, but with the given alpha
			\param alpha the alpha byte
			\returns a Color object with the given alpha value */
		Color withAlpha(Uint8 alpha) const;
		/*! Gives the color, but with the given alpha from 0.0 (fully transparent) to 1.0 (fully visible)
			\param alpha the alpha value from 0 to 1
			\returns a Color object with the given alpha value */
		Color withAlpha(double alpha) const;
		/*! Gives the color, but with the given alpha from 0.0 (fully transparent) to 1.0 (fully visible)
			\param alpha the alpha value from 0 to 1
			\returns a Color object with the given alpha value */
		Color withAlpha(float alpha) const;
		
		/*! Gives the color, but darkened by the given amount
			\param amount a value between 0 (no color change) and 1 (completely black)
			\returns a Color object darkened by the given amount */
		Color darkened(double amount) const;
		
		/*! Generates a random Color value.
			\param alpha true to randomize the alpha value, false to only randomize the RGB values
			\returns a random Color value*/
		static Color random(bool alpha=false);
		
		/*! Generates a greyscale Color value
			\param amount a value between 0 and 1 that represents the amount of white in the image
			\returns a greyscale Color value*/
		static Color getGrayColor(double amount);
	};
	
	
	
	namespace Colors
	{
		static const Color BLACK = Color(0, 0, 0);
		static const Color BLUE = Color(0, 0, 255);
		static const Color CYAN = Color(0, 255, 255);
		static const Color DARKGRAY = Color(64, 64, 64);
		static const Color GRAY = Color(128, 128, 128);
		static const Color GREEN = Color(0, 255, 0);
		static const Color LIGHTGRAY = Color(192, 192, 192);
		static const Color MAGENTA = Color(255, 0, 255);
		static const Color ORANGE = Color(255, 200, 0);
		static const Color PINK = Color(255, 175, 175);
		static const Color RED = Color(255, 0, 0);
		static const Color WHITE = Color(255, 255, 255);
		static const Color YELLOW = Color(255, 255, 0);
		static const Color TRANSPARENT = Color(0, 0, 0, 0);
		
		static const Color ACIDGREEN = Color(176, 191, 26);
		static const Color ALICEBLUE = Color(240, 248, 255);
		static const Color ALMOND = Color(239, 222, 205);
		static const Color AMARANTH = Color(229, 43, 80);
		static const Color ANTIQUEWHITE = Color(250, 235, 215);
		static const Color AQUA = Color(0, 255, 255);
		static const Color AQUAMARINE = Color(127, 255, 212);
		static const Color AZURE = Color(240, 255, 255);
		static const Color BAKERSCHOCOLATE = Color(92, 51, 23);
		static const Color BEIGE = Color(245, 245, 220);
		static const Color BISQUE = Color(255, 228, 196);
		static const Color BLANCHEDALMOND = Color(255, 255, 205);
		static const Color BLUEVIOLET = Color(138, 43, 226);
		static const Color BRASS = Color(181, 166, 66);
		static const Color BROWN = Color(165, 42, 42);
		static const Color BURLYWOOD = Color(222, 184, 135);
		static const Color BURNTUMBER = Color(138, 51, 36);
		static const Color CADETBLUE = Color(95, 158, 160);
		static const Color CHARTREUSE = Color(127, 255, 0);
		static const Color CHOCOLATE = Color(210, 105, 30);
		static const Color COBALT = Color(61, 89, 171);
		static const Color COBALTGREEN = Color(61, 145, 64);
		static const Color COOLCOPPER = Color(217, 135, 25);
		static const Color COPPER = Color(184, 115, 51);
		static const Color CORAL = Color(255, 127, 80);
		static const Color CORNFLOWERBLUE = Color(100, 149, 237);
		static const Color CORNSILK = Color(255, 248, 220);
		static const Color CRIMSON = Color(220, 20, 60);
		static const Color DARKBLUE = Color(0, 0, 139);
		static const Color DARKCYAN = Color(0, 139, 139);
		static const Color DARKGOLDENROD = Color(184, 134, 11);
		static const Color DARKGREEN = Color(0, 100, 0);
		static const Color DARKKHAKI = Color(189, 183, 107);
		static const Color DARKMAGENTA = Color(139, 0, 139);
		static const Color DARKOLIVEGREEN = Color(85, 107, 47);
		static const Color DARKORANGE = Color(255, 140, 0);
		static const Color DARKORCHID = Color(153, 50, 204);
		static const Color DARKPURPLE = Color(135, 31, 120);
		static const Color DARKRED = Color(139, 0, 0);
		static const Color DARKSALMON = Color(233, 150, 122);
		static const Color DARKSEAGREEN = Color(143,188,143);
		static const Color DARKSLATEBLUE = Color(72, 61, 139);
		static const Color DARKSLATEGRAY = Color(40, 79, 79);
		static const Color DARKTURQUOISE = Color(0, 206, 209);
		static const Color DARKVIOLET = Color(148, 0, 211);
		static const Color DEEPPINK = Color(255, 20, 147);
		static const Color DEEPSKYBLUE = Color(0, 191, 255);
		static const Color DIMGRAY = Color(105, 105, 105);
		static const Color DODGERBLUE = Color(30, 144, 255);
		static const Color EBONY = Color(85, 93, 80);
		static const Color EGGPLANT = Color(97, 64, 81);
		static const Color FIREBRICK = Color(178, 34, 34);
		static const Color FLORALWHITE = Color(255, 250, 240);
		static const Color FORESTGREEN = Color(34, 139, 34);
		static const Color FUSCHIA = Color(255, 0, 255);
		static const Color GAINSBORO = Color(220, 220, 220);
		static const Color GHOSTWHITE = Color(248, 248, 255);
		static const Color GOLD = Color(255, 215, 0);
		static const Color GOLDENROD = Color(218, 165, 32);
		static const Color GREENYELLOW = Color(173, 255, 47);
		static const Color HONEYDEW = Color(240, 255, 240);
		static const Color HOTPINK = Color(255, 105, 180);
		static const Color INDIANRED = Color(205, 92, 92);
		static const Color INDIGO = Color(75, 0, 130);
		static const Color IRISBLUE = Color(3, 180, 200);
		static const Color IVORY = Color(255, 240, 240);
		static const Color KHAKI = Color(240, 230, 140);
		static const Color LAVENDER = Color(230, 230, 250);
		static const Color LAVENDERBLUSH = Color(255, 240, 245);
		static const Color LAWNGREEN = Color(124, 252, 0);
		static const Color LEMONCHIFFON = Color(255, 250, 205);
		static const Color LIGHTBLUE = Color(173, 216, 230);
		static const Color LIGHTCORAL = Color(240, 128, 128);
		static const Color LIGHTCYAN = Color(224, 255, 255);
		static const Color LIGHTGOLDENRODYELLOW = Color(250, 250, 210);
		static const Color LIGHTGREEN = Color(144, 238, 144);
		static const Color LIGHTPINK = Color(255, 182, 193);
		static const Color LIGHTSALMON = Color(255, 160, 122);
		static const Color LIGHTSEAGREEN = Color(32, 178, 170);
		static const Color LIGHTSKYBLUE = Color(135, 206, 250);
		static const Color LIGHTSLATEGRAY = Color(119, 136, 153);
		static const Color LIGHTSTEELBLUE = Color(176, 196, 222);
		static const Color LIGHTYELLOW = Color(255, 255, 224);
		static const Color LIME = Color(0, 255, 0);
		static const Color LIMEGREEN = Color(50, 205, 50);
		static const Color LINEN = Color(250, 240, 230);
		static const Color MAROON = Color(128, 0, 0);
		static const Color MEDIUMAQUAMARINE = Color(102, 205, 170);
		static const Color MEDIUMBLUE = Color(102, 205, 170);
		static const Color MEDIUMORCHID = Color(186, 85, 211);
		static const Color MEDIUMPURPLE = Color(147, 112, 219);
		static const Color MEDIUMSEAGREEN = Color(60, 179, 113);
		static const Color MEDIUMSLATEBLUE = Color(123, 104, 238);
		static const Color MEDIUMSPRINGGREEN = Color(0, 250, 154);
		static const Color MEDIUMTURQUOISE = Color(72, 209, 204);
		static const Color MEDIUMVIOLETRED = Color(199, 21, 112);
		static const Color MIDNIGHTBLUE = Color(25, 25, 112);
		static const Color MINTCREAM = Color(245, 255, 250);
		static const Color MISTYROSE = Color(255, 228, 225);
		static const Color MOCCASIN = Color(255, 228, 181);
		static const Color NAVAJOWHITE = Color(255, 222, 173);
		static const Color NAVY = Color(0, 0, 128);
		static const Color NEONBLUE = Color(77, 77, 255);
		static const Color NEONGREEN = Color(57, 255, 20);
		static const Color OLDLACE = Color(253, 245, 230);
		static const Color OLIVE = Color(128, 128, 0);
		static const Color OLIVEDRAB = Color(107, 142, 45);
		static const Color ORANGERED = Color(255, 69, 0);
		static const Color ORCHID = Color(218, 112, 214);
		static const Color PALEGOLDENROD = Color(238, 232, 170);
		static const Color PALEGREEN = Color(152, 251, 152);
		static const Color PALETURQUOISE = Color(175, 238, 238);
		static const Color PALEVIOLETRED = Color(219, 112, 147);
		static const Color PAPAYAWHIP = Color(255, 239, 213);
		static const Color PEACHPUFF = Color(255, 218, 155);
		static const Color PERU = Color(205, 133, 63);
		static const Color PLUM = Color(221, 160, 221);
		static const Color POWDERBLUE = Color(176, 224, 230);
		static const Color PURPLE = Color(128, 128, 128);
		static const Color QUARTZ = Color(217, 217, 243);
		static const Color RASPBERRY = Color(135, 38, 87);
		static const Color ROSYBROWN = Color(188, 143, 143);
		static const Color ROYALBLUE = Color(65, 105, 255);
		static const Color SADDLEBROWN = Color(139, 69, 19);
		static const Color SAPGREEN = Color(48, 128, 20);
		static const Color SALMON = Color(250, 128, 114);
		static const Color SANDYBROWN = Color(244, 164, 96);
		static const Color SCARLET = Color(140, 23, 23);
		static const Color SEAGREEN = Color(46,	139, 87);
		static const Color SEASHELL = Color(255, 245, 238);
		static const Color SEMISWEETCHOCOLATE = Color(107, 66, 38);
		static const Color SEPIA = Color(94, 38, 18);
		static const Color SIENNA = Color(160, 82, 45);
		static const Color SILVER = Color(192, 192, 192);
		static const Color SKYBLUE = Color(135, 206, 235);
		static const Color SLATEBLUE = Color(106, 90, 205);
		static const Color SLATEGRAY = Color(112, 128, 144);
		static const Color SNOW = Color(255, 250, 250);
		static const Color SPICYPINK = Color(255, 28, 174);
		static const Color SPRINGGREEN = Color(0, 255, 127);
		static const Color STEELBLUE = Color(70, 130, 180);
		static const Color SUMMERSKYBLUE = Color(56, 176, 222);
		static const Color TAN = Color(210, 180, 140);
		static const Color TEAL = Color(0, 128, 128);
		static const Color THISTLE = Color(216, 191, 216);
		static const Color TOMATO = Color(253, 99, 71);
		static const Color TURQUOISE = Color(64, 224, 208);
		static const Color VIOLET = Color(238, 130, 238);
		static const Color VIOLETRED = Color(208, 32, 144);
		static const Color WARMGRAY = Color(128, 128, 105);
		static const Color WHEAT = Color(245, 222, 179);
		static const Color WHITESMOKE = Color(245, 245, 245);
		static const Color YELLOWGREEN = Color(154, 205, 50);
	};
}
