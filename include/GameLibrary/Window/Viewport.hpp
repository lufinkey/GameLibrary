
#pragma once

#include <GameLibrary/Utilities/Geometry/Vector2.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	class Viewport
	{
		friend class Graphics;
	public:
		Viewport();
		explicit Viewport(const Vector2d& size);
		Viewport(double width, double height);
		~Viewport();
		
		void setSize(double width, double height);
		void setSize(const Vector2d& size);
		const Vector2d& getSize() const;

		void setZoom(double zoom);
		double getZoom() const;

		void setMatchesWindow(bool toggle);
		bool matchesWindow() const;

		void setLetterboxed(bool toggle);
		bool isLetterboxed() const;
		
		void setMaintainsResolution(bool toggle);
		bool maintainsResolution() const;
	private:
		Vector2d size;
		double zoom;
		bool matchWindow;
		bool letterboxed;
		bool maintainResolution;
	};
}

REGISTER_TYPE(fgl, Viewport)
