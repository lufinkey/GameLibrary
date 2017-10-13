
#include <GameLibrary/Graphics/Color.hpp>
#include <GameLibrary/Utilities/Math.hpp>
#include <GameLibrary/Utilities/Plist.hpp>

namespace fgl
{
	Color::Color(const Dictionary& colorValues)
		: a(extractNumber(colorValues, "a", 255)),
		b(extractNumber(colorValues, "b", 255)),
		g(extractNumber(colorValues, "g", 255)),
		r(extractNumber(colorValues, "r", 255))
	{
		//
	}

	Color& Color::operator=(Uint32 rgba)
	{
		Uint8* rgbaArray = (Uint8*)(&rgba);
		r = rgbaArray[3];
		g = rgbaArray[2];
		b = rgbaArray[1];
		a = rgbaArray[0];
		return *this;
	}
	
	bool Color::operator==(const Color&color) const
	{
		return equals(color);
	}
	
	bool Color::operator!=(const Color&color) const
	{
		return !equals(color);
	}
	
	Uint32 Color::getRGBA() const
	{
		Uint32 rgba = 0;
		Uint8* arr = (Uint8*)(&rgba);
		arr[3] = r;
		arr[2] = g;
		arr[1] = b;
		arr[0] = a;
		return rgba;
	}

	Uint32 Color::getARGB() const
	{
		Uint32 argb = 0;
		Uint8* arr = (Uint8*)(&argb);
		arr[3] = a;
		arr[2] = r;
		arr[1] = g;
		arr[0] = b;
		return argb;
	}

	Uint32 Color::getABGR() const
	{
		Uint32 abgr = 0;
		Uint8* arr = (Uint8*)(&abgr);
		arr[3] = a;
		arr[2] = b;
		arr[1] = g;
		arr[0] = r;
		return abgr;
	}
	
	Uint32 Color::getBGRA() const
	{
		Uint32 abgr = 0;
		Uint8* arr = (Uint8*)(&abgr);
		arr[3] = b;
		arr[2] = g;
		arr[1] = r;
		arr[0] = a;
		return abgr;
	}
	
	bool Color::equals(const Color&color) const
	{
		if(r==color.r && g==color.g && b==color.b && a==color.a)
		{
			return true;
		}
		return false;
	}

	Uint8 Color_compositeByte(Uint8 orig, Uint8 comp)
	{
		/*double x = (double)orig;
		double n = (double)comp;
		return (Uint8)((-n / 255) * (n - x - 255));*/
		return (Uint8)(orig*((double)comp/255));
	}

	Color Color::composite(const Color& comp) const
	{
		Uint8 cr = Color_compositeByte(r,comp.r);
		Uint8 cg = Color_compositeByte(g,comp.g);
		Uint8 cb = Color_compositeByte(b,comp.b);
		Uint8 ca = Color_compositeByte(a,comp.a);
		return Color(cr,cg,cb,ca);
	}

	Color Color::negative(bool alpha) const
	{
		if(alpha)
		{
			return Color(255-r, 255-g, 255-b, 255-a);
		}
		return Color(255-r, 255-g, 255-b, a);
	}
	
	Color Color::withAlpha(Uint8 alpha) const
	{
		return Color(r, g, b, alpha);
	}

	Color Color::random(bool alpha)
	{
		Color color;
		color.r = (Uint8)(Math::random()*255);
		color.g = (Uint8)(Math::random()*255);
		color.b = (Uint8)(Math::random()*255);
		if(alpha)
		{
			color.a = (Uint8)(Math::random()*255);
		}
		return color;
	}
	
	Color Color::getGrayColor(double amount)
	{
		if(amount > 1.0)
		{
			amount = 1.0;
		}
		else if(amount < 0.0)
		{
			amount = 0.0;
		}
		return Color((Uint8)(255.0*amount), (Uint8)(255.0*amount), (Uint8)(255.0*amount));
	}
}
