
#pragma once

#include <GameLibrary/Screen/Screen.hpp>

namespace fgl
{
	class PopupScreen : public Screen
	{
	public:
		static const Transition* const defaultPresentationTransition;

		PopupScreen(ScreenElement* popupElement);
		virtual ~PopupScreen();

		virtual const Transition* getDefaultPresentationTransition() const override;
		virtual const Transition* getDefaultDismissalTransition() const override;
		
		ScreenElement* getPopupElement() const;

	private:
		ScreenElement* popupElement;
	};
}
