
#pragma once

#include <GameLibrary/Utilities/ArrayList.hpp>
#include "Screen.hpp"

namespace fgl
{
	/*! Screen container that handles navigating through list of Screen objects*/
	class ScreenManager : public Screen
	{
		friend class Screen;
	public:
		/*! The default Transition to use for pushing or popping Screen objects*/
		static const Transition* const defaultPushTransition;
		
		/*! Constructs a ScreenManager with a Screen at the beginning of the contained list.
			\param rootScreen the Screen to be contained in the root of the container
			\throws fgl::IllegalArgumentException if the given screen:\n
				1.) is null\n
				2.) is already contained within another ScreenManager\n
				3.) is presented on another Screen\n
				4.) already belongs to another Window*/
		explicit ScreenManager(Screen*rootScreen);
		/*! Constructs a ScreenManager with a Screen at the beginning of the contained list, and an optional Window for being the root screen of a Window.
			\param window a Window pointer, or null if this ScreenManager is not the root Screen of the Window
			\param rootScreen the Screen to be contained in the root of the container
			\throws fgl::IllegalArgumentException if the given screen:\n
				1.) is null\n
				2.) is already contained within another ScreenManager\n
				3.) is presented on another Screen\n
				4.) already belongs to another Window*/
		ScreenManager(Window*window, Screen*rootScreen);
		/*! virtual destructor*/
		virtual ~ScreenManager();
		
		
		ScreenManager(const ScreenManager&) = delete;
		ScreenManager& operator=(const ScreenManager&) = delete;
		
		
		/*! \copydoc fgl::Screen::update(fgl::ApplicationData)*/
		virtual void update(ApplicationData appData) override;
		/*! \copydoc fgl::Screen::draw(fgl::ApplicationData,fgl::Graphics)const*/
		virtual void draw(ApplicationData appData, Graphics graphics) const override;
		
		
		/*! \copydoc fgl::Screen::onWillAppear(const fgl::Transition*transition)*/
		virtual void onWillAppear(const Transition*transition) override;
		/*! \copydoc fgl::Screen::onDidAppear(const fgl::Transition*transition)*/
		virtual void onDidAppear(const Transition*transition) override;
		/*! \copydoc fgl::Screen::onWillDisappear(const fgl::Transition*transition)*/
		virtual void onWillDisappear(const Transition*transition) override;
		/*! \copydoc fgl::Screen::onDidDisappear(const fgl::Transition*transition)*/
		virtual void onDidDisappear(const Transition*transition) override;
		
		
		/*! Sets the container's Screen list.
		If the top screen on the list is the same as the top screen in the container, then no transition is performed.
		If the top screen on the list is a screen that already exists within the container, a pop animation is performed.
		If the top screen on the list does not exist already within the container, a push animation is performed.
			\param screens a list of Screen pointers to set
			\param transition a Transition to use to set the new list
			\param duration a length of time, in milliseconds, that the transition will last
			\param completion a callback to call when the Screen finishes the transition
			\throws fgl::IllegalArgumentException if the given list of Screen pointers is:\n
				1.) is empty\n
				2.) contains a null Screen\n
				3.) contains a Screen that is already presented on another Screen\n
				4.) contains a Screen that is already inside another Screen container\n
				5.) contains a Screen that already belongs to another Window\n
			\throws fgl::ScreenNavigationException if this container is already in the process of pushing, popping, or setting items on the list*/
		void set(const ArrayList<Screen*>& screens, const Transition*transition=defaultPushTransition, unsigned long long duration=Transition::defaultDuration, CompletionCallback completion=nullptr);
		/*! Pushes a Screen to the container's list of Screen pointers.
			\param screen a Screen pointer to push
			\param transition a Transition to use to push the Screen
			\param duration a length of time, in milliseconds, that the transition will last
			\param completion a callback to call when the Screen finishes the transition
			\throws fgl::IllegalArgumentException if the given Screen:\n
				1.) is null\n
				2.) is already presented on another Screen\n
				3.) is already inside a Screen container\n
				4.) already belongs to another Window\n
			\throws fgl::ScreenNavigationException if:\n
				1.) this container is already in the process of pushing, popping, or setting items on the list*/
		void push(Screen*screen, const Transition*transition=defaultPushTransition, unsigned long long duration=Transition::defaultDuration, CompletionCallback completion=nullptr);
		/*! Pushes a list of Screen pointers onto the container's list of Screen pointers.
			\param screens a list of Screen pointers to push
			\param transition a Transition to use to push the list
			\param duration a length of time, in milliseconds, that the transition will last
			\param completion a callback to call when the Screen finishes the transition
			\throws fgl::IllegalArgumentException if the given list of Screen pointers is:\n
				1.) is empty\n
				2.) contains a null Screen\n
				3.) contains a Screen that is already presented on another Screen\n
				4.) contains a Screen that is already inside a Screen container\n
				5.) contains a Screen that already belongs to another Window\n
			\throws fgl::ScreenNavigationException if this container is already in the process of pushing, popping, or setting items on the list*/
		void push(const ArrayList<Screen*>& screens, const Transition*transition=defaultPushTransition, unsigned long long duration=Transition::defaultDuration, CompletionCallback completion=nullptr);
		/*! Pops the top Screen off of the container's list of Screen pointers.
			\param transition a Transition to use to pop the Screen
			\param duration a length of time, in milliseconds, that the transition will last
			\param completion a callback to call when the Screen finishes the transition
			\returns the Screen pointer that was popped from the list
			\throws fgl::ScreenNavigationException if:\n
				1.) this container is already in the process of pushing, popping, or setting items on the list
				2.) this container only contains 1 Screen*/
		Screen* pop(const Transition*transition=defaultPushTransition, unsigned long long duration=Transition::defaultDuration, CompletionCallback completion=nullptr);
		/*! Pops Screen pointers off of the container's list to index of the given Screen pointer.
			\param screen the screen to pop the list to; This Screen will become the new top Screen
			\param transition a Transition to use to pop the Screen pointers
			\param duration a length of time, in milliseconds, that the transition will last
			\param completion a callback to call when the Screen finishes the transition
			\returns a list of Screen pointers that were popped from the list
			\throws fgl::IllegalArgumentException if the given Screen is not contained within this Screen container
			\throws fgl::ScreenNavigationException if this container is already in the process of pushing, popping, or setting items on the list*/
		ArrayList<Screen*> popTo(Screen*screen, const Transition*transition=defaultPushTransition, unsigned long long duration=Transition::defaultDuration, CompletionCallback completion=nullptr);
		/*! Pops Screen objects off of the container's list to the first Screen in the list.
			\param transition a Transition to use to pop the Screen pointers
			\param duration a length of time, in milliseconds, that the transition will last
			\param completion a callback to call when the Screen finishes the transition
			\returns a list of Screen pointers that were popped from the list
			\throws fgl::ScreenNavigationException if this container is already in the process of pushing, popping, or setting items on the list*/
		ArrayList<Screen*> popToRoot(const Transition*transition=defaultPushTransition, unsigned long long duration=Transition::defaultDuration, CompletionCallback completion=nullptr);
		
	protected:
		/*! Draws all contained Screen pointers. This function is automatically called from within ScreenManager::draw.
			\param appData specifies information about the Application drawing the contained Screen pointers, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the contained Screen pointers*/
		void drawScreens(ApplicationData appData, Graphics graphics) const;
		
	private:
		TransitionData pushpopData;

		ArrayList<Screen*> screens;

		virtual void setWindow(Window*window) override;
	};
}
