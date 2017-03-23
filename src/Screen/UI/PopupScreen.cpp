
#include <GameLibrary/Screen/UI/PopupScreen.hpp>
#include <GameLibrary/Screen/Transition/FadeZoomTransition.hpp>

namespace fgl
{
	const Transition* const PopupScreen::defaultPresentationTransition = new FadeZoomTransition(1.4, 1.0);

	PopupScreen::PopupScreen(ScreenElement* popupElement, bool autoDeleteElement)
		: popupElement(popupElement), autoDeletePopupElement(autoDeleteElement)
	{
		setParentScreenDrawingEnabled(true);

		getElement()->setBackgroundColor(fgl::Color(0, 0, 0, 128));

		popupElement->setLayoutRule(fgl::LAYOUTRULE_CENTER_X, 0.5, fgl::LAYOUTVALUE_RATIO);
		popupElement->setLayoutRule(fgl::LAYOUTRULE_CENTER_Y, 0.5, fgl::LAYOUTVALUE_RATIO);

		getElement()->addChildElement(popupElement);
	}

	PopupScreen::~PopupScreen()
	{
		if(autoDeletePopupElement)
		{
			delete popupElement;
		}
	}

	const Transition* PopupScreen::getDefaultPresentationTransition() const
	{
		return defaultPresentationTransition;
	}

	const Transition* PopupScreen::getDefaultDismissalTransition() const
	{
		return defaultPresentationTransition;
	}
}
