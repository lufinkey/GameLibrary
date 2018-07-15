
#pragma once

#include "Transition.hpp"

namespace fgl
{
	/*! A custom Transition that slides and pushes one Screen out of the way of the other.*/
	class SlideTransition : public Transition
	{
	public:
		/*! The direction to slide*/
		typedef enum
		{
			/*! The Screen slides upward*/
			SLIDE_UP,
			/*! The Screen slides to the right*/
			SLIDE_RIGHT,
			/*! The Screen slides downward*/
			SLIDE_DOWN,
			/*! The Screen slides to the left*/
			SLIDE_LEFT
		} SlideDirection;
		
		/*! Constructs a transition that slides in a given direction
			\param direction the direction where the Screen slides*/
		explicit SlideTransition(SlideDirection direction);
		/*! virtual destructor*/
		virtual ~SlideTransition();
		
		/*! \copydoc fgl::Transition::draw(ApplicationData,Graphics,double,Screen*,Screen*)const*/
		virtual void draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const override;
		
	protected:
		/*! The direction where the Screen slides*/
		SlideDirection direction;
	};
}

REGISTER_TYPE(fgl::SlideTransition, fgl::Transition)
