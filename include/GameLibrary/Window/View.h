
#pragma once

#include <GameLibrary/Utilities/Geometry/Vector2.h>

namespace GameLibrary
{
	class View
	{
		friend class Graphics;
	public:
		View();
		View(const View&);
		explicit View(const Vector2d& size);
		View(double width, double height);
		~View();
		
		void setSize(double width, double height);
		void setSize(const Vector2d& size);
		const Vector2d& getSize() const;

		void setZoom(double zoom);
		double getZoom() const;

		void setMatchesWindow(bool toggle);
		bool matchesWindow() const;

		void setLetterboxed(bool toggle);
		bool isLetterboxed() const;
		
		void setMaintainResolution(bool toggle);
		bool maintainsResolution() const;
	private:
		Vector2d size;
		double zoom;
		bool matchWindow;
		bool letterboxed;
		bool maintainResolution;
	};
}
