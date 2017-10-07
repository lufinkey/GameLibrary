
#include <GameLibrary/Window/Viewport.hpp>

namespace fgl
{
	Viewport::Viewport() : Viewport(640,480)
	{
		//
	}

	Viewport::Viewport(const Vector2d& size) : Viewport(size.x,size.y)
	{
		//
	}

	Viewport::Viewport(double width, double height)
	{
		size.x = width;
		size.y = height;
		zoom = 1;
		matchWindow = false;
		letterboxed = true;
		maintainResolution = false;
	}

	Viewport::~Viewport()
	{
		//
	}
	
	void Viewport::setSize(double width, double height)
	{
		size.x = width;
		size.y = height;
	}

	void Viewport::setSize(const Vector2d& size)
	{
		setSize(size.x, size.y);
	}

	const Vector2d& Viewport::getSize() const
	{
		return size;
	}

	void Viewport::setZoom(double z)
	{
		zoom = z;
	}

	double Viewport::getZoom() const
	{
		return zoom;
	}

	void Viewport::setMatchesWindow(bool toggle)
	{
		matchWindow = toggle;
	}

	bool Viewport::matchesWindow() const
	{
		return matchWindow;
	}

	void Viewport::setLetterboxed(bool toggle)
	{
		letterboxed = toggle;
	}

	bool Viewport::isLetterboxed() const
	{
		return letterboxed;
	}
	
	void Viewport::setMaintainsResolution(bool toggle)
	{
		maintainResolution = toggle;
	}
	
	bool Viewport::maintainsResolution() const
	{
		return maintainResolution;
	}
}
