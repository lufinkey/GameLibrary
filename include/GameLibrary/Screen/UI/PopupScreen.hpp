
#pragma once

#include <GameLibrary/Screen/Screen.hpp>

namespace fgl
{
	class PopupScreen : public Screen
	{
	public:
		static const Transition* const defaultPresentationTransition;

		PopupScreen(ScreenElement* popupElement, bool autoDeleteElement=true);
		virtual ~PopupScreen();

		virtual const Transition* getDefaultPresentationTransition() const override;
		virtual const Transition* getDefaultDismissalTransition() const override;

	private:
		ScreenElement* popupElement;
		bool autoDeletePopupElement;
	};
}
