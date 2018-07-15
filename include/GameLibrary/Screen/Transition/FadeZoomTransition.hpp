
#pragma once

#include "Transition.hpp"

namespace fgl
{
	/*! A custom Transition that fades and zooms between two Screens*/
	class FadeZoomTransition : public Transition
	{
	public:
		/*! Constructs a transition that fades and zooms between two Screens
			\param startZoom the zoom ratio at the beginning of the transition
			\param endZoom the zoom ratio at the end of the transition; Default value is 1*/
		explicit FadeZoomTransition(double startZoom, double endZoom=1.0f);
		/*! virtual destructor*/
		virtual ~FadeZoomTransition();
		
		/*! \copydoc fgl::Transition::draw(ApplicationData,Graphics,double,Screen*,Screen*)const*/
		virtual void draw(ApplicationData appData, Graphics graphics, double progress, Screen* screen1, Screen* screen2) const override;
		
	protected:
		/*! the zoom ratio at the beginning of the transition*/
		double startZoom;
		/*! the zoom ratio at the end of the transition; Default value is 1*/
		double endZoom;
	};
}

REGISTER_TYPE(fgl, FadeZoomTransition, fgl::Transition)
