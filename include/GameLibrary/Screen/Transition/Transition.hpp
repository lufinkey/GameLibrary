
#pragma once

#include <GameLibrary/Application/ApplicationData.hpp>
#include <GameLibrary/Graphics/Graphics.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	class Screen;
	
	/*! An interface class for creating a custom transition between two Screens */
	class Transition
	{
	public:
		/*! The default transition duration, in milliseconds (250 milliseconds)*/
		static const long long defaultDuration = 250ULL;
		
		/*! default constructor*/
		Transition(){}
		/*! virtual destructor*/
		virtual ~Transition(){}
		
		/*! Draws the two Screens in their transition state. For best results, the two Screens should have the same size.
			\param appData specifies information about the Application drawing the Transition, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Transition
			\param progress the current Transition progress, from 0 to 1; Making this a value other than 0 through 1 causes undefined behavior
			\param screen1 the first Screen being transitioned
			\param screen2 the second Screen being transitioned*/
		virtual void draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const = 0;
	};
}

REGISTER_TYPE(fgl, Transition)
