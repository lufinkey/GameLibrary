
#pragma once

#include <GameLibrary/Application/ApplicationData.h>
#include <GameLibrary/Graphics/Graphics.h>

namespace fgl
{
	/*! An interface class representing an entity that can be drawn to a Window using a Graphics object*/
	class Drawable
	{
	public:
		/*! virtual destructor*/
		virtual ~Drawable(){};
		
		/*! Draws the entity to the Window.
			\param appData specifies information about the Application drawing the Drawable, such as the Window object, the View transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Drawable*/
		virtual void draw(ApplicationData appData, Graphics graphics) const = 0;
		
		
		/*! Gets the actual frame (bounding box) of the Drawable inside of the Window.
			\returns a RectangleD object representing the Drawable's bounding box*/
		virtual RectangleD getFrame() const = 0;
	};
}
