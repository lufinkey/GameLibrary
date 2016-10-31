
#pragma once

#include "Transition.h"

namespace fgl
{
	/*! A custom Transition that fades from one Drawable to a solid color, and then fades to the other Drawable*/
	class FadeColorTransition : public Transition
	{
	public:
		/*! Constructs a transition with an intermediate fade color.
			\param color the solid color to fade between.
			\param frozenPortion the portion of the transition duration when the solid color is displayed without fading; Must be a value between 0 and 1
			\throws fgl::IllegalArgumentException if frozenPortion is not a value between 0 and 1*/
		explicit FadeColorTransition(const Color& color, double frozenPortion = 0);
		/*! virtual destructor*/
		virtual ~FadeColorTransition();
		
		/*! \copydoc fgl::Transition::draw(ApplicationData,Graphics,double,Drawable*,Drawable*)const*/
		virtual void draw(ApplicationData appData, Graphics graphics, double progress, Drawable* element1, Drawable* element2) const override;
		
	protected:
		/*! the solid color to fade between*/
		Color color;
		/*! the portion of the transition duration when the solid color is displayed without fading; Must be a value between 0 and 1*/
		double frozenPortion;
	};
}