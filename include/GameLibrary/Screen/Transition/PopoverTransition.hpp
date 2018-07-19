
#pragma once

#include "Transition.hpp"

namespace fgl
{
	/*! A custom Transition that slides one Screen on top of the other, with an optional fade*/
	class PopoverTransition : public Transition
	{
	public:
		/*! The direction of the popover*/
		typedef enum
		{
			/*! The second Screen slides upward*/
			POPOVER_UP,
			/*! The second Screen slides to the right*/
			POPOVER_RIGHT,
			/*! The second Screen slides downward*/
			POPOVER_DOWN,
			/*! The second Screen slides to the left*/
			POPOVER_LEFT
		} PopoverDirection;
		
		/*! Constructs a transition that slides one Screen on top of the other Screen, with an optional fade.
			\param direction the direction of the popover
			\param fade true to fade in as the Screen slides in, or false to just have the Screen slide in without fade*/
		explicit PopoverTransition(PopoverDirection direction, bool fade=false);
		/*! virtual destructor*/
		virtual ~PopoverTransition();
		
		/*! \copydoc fgl::Transition::draw(ApplicationData,Graphics,double,Screen*,Screen*)const*/
		virtual void draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const override;
		
	protected:
		/*! the direction of the popover*/
		PopoverDirection direction;
		/*! Tells whether the transition should fade or not; Value is true to fade in as the Screen slides in, or false to just have the Screen slide in without fade*/
		bool fade;
	};
}
