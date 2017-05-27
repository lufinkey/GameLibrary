
#pragma once

#include <functional>
#include <GameLibrary/Utilities/ArrayList.hpp>
#include "ScreenElement.hpp"
#include "Transition/Transition.hpp"

namespace fgl
{
	class ScreenManager;

	/*! A drawable entity that can be used to separate different menus or screens of an Application. This class is non-copyable.*/
	class Screen
	{
		friend class ScreenManager;
	public:
		/*! The default Transition to use for presenting or dismissing Screen objects*/
		static const Transition* const defaultPresentationTransition;
		
		/*! default constructor*/
		Screen();
		/*! Constructs a Screen to be the root Screen of a Window. Only one screen in any Screen stack should be delegated as the root Screen.
			\param window the Window that the Screen will be contained within*/
		explicit Screen(Window* window);
		/*! virtual destructor*/
		virtual ~Screen();
		
		
		Screen(const Screen&) = delete;
		Screen& operator=(const Screen&) = delete;
		
		
		/*! Updates the properties of the Screen. This should NOT be overridden except for creating a custom Screen container.
		If overridden, the overriding function should first call the base class's update function, and then check if the Screen contains a child Screen before updating.
			\param appData specifies information about the Application updating the Screen, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData*/
		virtual void update(ApplicationData appData);
		/*! Draws the Screen and all of its children.
			\param appData specifies information about the Application drawing the Screen, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Screen*/
		virtual void draw(ApplicationData appData, Graphics graphics) const final;
		
		
		/*! Gets the size of the Screen inside the Window.
			\returns a Vector2d object representing the Screen's size in pixels*/
		Vector2d getSize() const;
		
		
		//TODO add events for navigation from ScreenManager?
		
		
		/*! Called when the Screen's size has changed, possibly due to frame resizing, or window resizing, or a mobile device orientation change.
			\param oldSize the previous size
			\param newSize the new size*/
		virtual void onSizeChange(const Vector2d& oldSize, const Vector2d& newSize);
		
		
		/*! Called when the Screen is about to appear at the top level of the Screen stack.
			\param transition the Transition used to display the Screen, or null if no Transition was used*/
		virtual void onWillAppear(const Transition* transition);
		/*! Called when the Screen has just appeared at the top level of the Screen stack.
			\param transition the Transition used to display the Screen, or null if no Transition was used*/
		virtual void onDidAppear(const Transition* transition);
		/*! Called when the Screen is about to disappear off the top level of the Screen stack.
			\param transition the Transition used to hide the Screen, or null if no Transition was used*/
		virtual void onWillDisappear(const Transition* transition);
		/*! Called when the Screen has just disappeared off the top level of the Screen stack.
			\param transition the Transition used to hide the Screen, or null if no Transition was used*/
		virtual void onDidDisappear(const Transition* transition);
		
		
		/*! Gives the default transition to use when being presented by another Screen
			\returns a Transition pointer, or null to use no transition */
		virtual const Transition* getDefaultPresentationTransition() const;
		/*! Gives the default length of time to take when presenting this Screen on another Screen
			\returns a length of time, in milliseconds, that the presentation will last */
		virtual long long getDefaultPresentationDuration() const;
		/*! Gives the default transition to use when being dismissed from another Screen.
			\note this transition is played backwards, to allow for using the same transition for presenting and dismissing
			\returns a Transition pointer, or null to use no transition */
		virtual const Transition* getDefaultDismissalTransition() const;
		/*! Gives the default length of time to take when dismissing this Screen from another Screen
			\returns a length of time, in milliseconds, that the dismissal will last */
		virtual long long getDefaultDismissalDuration() const;
		
		
		/*! Presents another Screen on top of this Screen. Only one child Screen can be presented to a Screen at a time, but the top level Screen can always present another Screen.
			\param screen the Screen to present
			\param transition a Transition to use to present the Screen, or null to present the Screen without a transition
			\param duration a length of time, in milliseconds, that the transition will last
			\param oncompletion a callback to call when the Screen finishes the transition
			\param ondismissal a callback to call when the Screen is dismissed after the transition finishes
			\throws fgl::IllegalArgumentException if:
				1.) the Screen being presented is null
				2.) the Screen being presented is already presented on another Screen,
				3.) the Screen being presented is a root Screen \see fgl::Screen::Screen(fgl::Window*),
				4.) the Screen being presented is already held within a ScreenManager,
				5.) duration is negative
			\throws fgl::ScreenNavigationException if a Screen is already in the process of being presented on this Screen*/
		void presentScreen(Screen* screen, const Transition* transition, long long duration, const std::function<void()>& oncompletion=nullptr, const std::function<void()>& ondismissal=nullptr);
		/*! Presents another Screen on top of this Screen. Only one child Screen can be presented to a Screen at a time, but the top level Screen can always present another Screen. This function uses the presented Screen's getDefaultPresentationTransition function for the transition, and getDefaultPresentationDuration for the duration.
			\param screen the Screen to present
			\param oncompletion a callback to call when the Screen finishes the transition
			\param ondismissal a callback to call when the Screen is dismissed after the transition finishes
			\throws fgl::IllegalArgumentException if:
				1.) the Screen being presented is null
				2.) the Screen being presented is already presented on another Screen,
				3.) the Screen being presented is a root Screen \see fgl::Screen::Screen(fgl::Window*),
				4.) the Screen being presented is already held within a ScreenManager,
				5.) duration is negative
			\throws fgl::ScreenNavigationException if a Screen is already in the process of being presented on this Screen*/
		void presentScreen(Screen* screen, const std::function<void()>& oncompletion=nullptr, const std::function<void()>& ondismissal=nullptr);
		/*! Dismisses the child Screen that is presented on top of this Screen
			\param transition a Transition to use to dismiss the Screen
			\param duration a length of time, in milliseconds, that the transition will last
			\param oncompletion a callback to call when the Screen finishes the transition */
		void dismissPresentedScreen(const Transition* transition, long long duration, const std::function<void()>& oncompletion=nullptr);
		/*! Dismisses the child Screen that is presented on top of this Screen. This function uses the presented Screen's getDefaultDismissalTransition function for the transition, and getDefaultDismissalDuration for the duration
			\param oncompletion a callback to call when the Screen finishes the transition */
		void dismissPresentedScreen(const std::function<void()>& oncompletion=nullptr);
		
		
		/*! Gets the root ScreenElement.
			\returns a ScreenElement pointer*/
		ScreenElement* getElement() const;
		/*! Gets the ScreenManager that the Screen is contained within.
			\returns a ScreenManager pointer, or null if the Screen is not contained within a ScreenManager*/
		ScreenManager* getScreenManager() const;
		/*! Gets the Screen that is presenting this Screen.
			\returns a Screen pointer, or null if the Screen is not being presented on another Screen*/
		Screen* getPresentingScreen() const;
		/*! Gets the Screen that this Screen is presenting.
			\returns a Screen pointer, or null if this Screen is not presenting another Screen*/
		Screen* getPresentedScreen() const;
		/*! Gets the parent Screen to this child Screen, or null if this Screen has not been added to a parent Screen
			\returns a Screen pointer, or null if this Screen is not the child of another Screen*/
		Screen* getParentScreen() const;
		/*! Gets an ArrayList of child Screens
			\returns a list of Screens that are children of this Screen*/
		ArrayList<Screen*> getChildScreens() const;
		
		
		/*! Gets the top level Screen in this Screen's child stack.
			\returns a Screen pointer*/
		Screen* getTopScreenInHeirarchy() const;
		/*! Gets the bottom level Screen in this Screen's child stack.
			\returns a Screen pointer*/
		Screen* getBottomScreenInHeirarchy() const;
		/*! Tells whether the Screen is visible in the Screen heirarchy, or if another Screen is covering it. If the bottom Screen is not the root Screen of a Window, this will always return false.
			\returns true if the Screen is visible in the Screen heirarchy, or false if otherwise*/
		bool isOnTop();
		/*! Tells whether this Screen is in the process of presenting a Screen.
			\returns true if a transition to present a Screen is occurring, or false if otherwise*/
		bool isPresenting() const;
		/*! Tells whether this Screen is in the process of dismissing a presented Screen.
			\returns true if a transition to dismiss a presented Screen is occurring, or false if otherwise*/
		bool isDismissing() const;
		
		
		/*! Enables/disables drawing of the presenting Screen behind this Screen
			\param enabled true to enable drawing the parent Screen, or false to disable drawing the parent Screen */
		void setParentScreenDrawingEnabled(bool enabled);
		/*! Tells whether drawing of the presenting Screen behind this Screen is enabled
			\returns true if parent drawing is enabled, or false if it is disabled */
		bool isParentScreenDrawingEnabled() const;
		/*! Enables/disables updating of the presenting Screen behind this Screen
			\param enabled true to enable updating the parent Screen, or false to disable updating the parent Screen */
		void setParentScreenUpdatingEnabled(bool enabled);
		/*! Tells whether updating of the presenting Screen behind this Screen is enabled
			\returns true if parent updating is enabled, or false if it is disabled */
		bool isParentScreenUpdatingEnabled() const;
		
		
		/*! Gets the Window of the root Screen of this Screen stack
			\returns a Window pointer, or null if this Screen is not on a Screen stack owned by a Window */
		Window* getWindow() const;
		
	protected:
		typedef enum
		{
			TRANSITION_NONE,
			TRANSITION_SHOW,
			TRANSITION_HIDE
		} TransitionAction;

		typedef struct
		{
			String name;
			float zLayer;
			Screen* screen;
			Screen* transitionScreen;
			TransitionAction action;
			bool requiresInitializing;
			long long startTime;
			const Transition*transition;
			double progress;
			long long duration;
			std::function<void()> completion;
		} TransitionData;

		/*! Updates the properties of the Screen. This function is called from within the update function, and should not manually be called.
		This function is safe to override with custom behavior.
			\param appData specifies information about the Application updating the Screen, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData*/
		virtual void onUpdate(const ApplicationData& appData);
		/*! Draws the Screen and any contents to the Window. This function is called from within the draw function, and should not be manually called.
		This function is safe to override with custom behavior.
			\param appData specifies information about the Application updating the Screen, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to Draw the Screen contents*/
		virtual void onDraw(const ApplicationData& appData, Graphics graphics) const;
		
		
		/*! Draws the elements of the Screen. Should only be called within Screen::draw \see fgl::Screen::drawingOverlayTransition for an example of how to call this
			\param appData specifies information about the Application drawing the Screen, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Screen*/
		virtual void drawElements(const ApplicationData& appData, Graphics graphics) const;
		/*! Draws a Screen transition
			\param transitionData the transition data for the transitioning Screen
			\param appData specifies information about the Application drawing the Screen, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Screen*/
		virtual void drawTransition(const TransitionData* transitionData, const ApplicationData& appData, Graphics graphics) const;
		/*! Draws a child Screen
			\param screen the child screen to draw
			\param appData specifies information about the Application drawing the Screen, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Screen*/
		virtual void drawChildScreen(Screen* childScreen, const ApplicationData& appData, Graphics graphics) const;


		/*! Adds a transition to animate between two screens.
			\param name the name to reference the transition by
			\param zLayer priority value to use in ordering the child screen drawing operations. A higher value means the childScreen will be closer to the top of the stack. Anything below 0.5 will be drawn underneath this Screen's elements.
			\param screen the screen to transition from
			\param transitionScreen the screen to transition to
			\param action species whether to show or hide transitionScreen (run transition forward or backward)
			\param transition the transition to animate
			\param duration the length of time to transition in milliseconds
			\param completion a function to run when the transition finishes*/
		void addTransition(const String& name, float zLayer, Screen* screen, Screen* transitionScreen, TransitionAction action, const Transition* transition, long long duration, const std::function<void()>& completion);
		/*! Checks if a transition with a given name is animating
			\param name the name of the transition to look for
			\returns true if the transition is animating, or false if it is not*/
		bool isTransitionAnimating(const String& name) const;


		/*! Adds a child screen to draw on top of this screen
			\param childScreen the child screen to add
			\param zLayer priority value to use in ordering the child screen drawing operations. A higher value means the childScreen will be closer to the top of the stack. Anything below 0.5 will be drawn underneath this Screen's elements*/
		void addChildScreen(Screen* screen, float zLayer, bool visible=true);
		/*! Removes a child screen from the child screen stack
			\param childScreen the child screen to remove*/
		void removeChildScreen(Screen* screen);
		/*! Sets a child screen visible or invisible
			\param screen the child screen to change the visibility for
			\param visible true to make the child screen visible, or false to make it not visible*/
		void setChildScreenVisible(Screen* screen, bool visible);
		
	private:
		static void TransitionData_clear(TransitionData& data);
		static void TransitionData_begin(TransitionData& data, Screen* screen, Screen* transitionScreen, TransitionAction action, const Transition* transition, long long duration, const std::function<void()>& completion=nullptr);
		/*Makes sure the TransitionData is initialized, if it requires it.*/
		static void TransitionData_checkInitialization(ApplicationData& appData, TransitionData& data);
		/*Applies any new progress to the transition.
		If the transition finishes, a constant representing the finished transition is returned. Otherwise, 0 is returned.*/
		static bool TransitionData_applyProgress(ApplicationData& appData, TransitionData& data);
		/*Checks if the transition is finished by calling TransitionData_applyProgress, and stores the objects that need calling if so.*/
		static std::function<void()> TransitionData_checkFinished(ApplicationData& appData, TransitionData& data);

		void handleFirstShowing();
		
		Window* window;
		Vector2d framesize;
		
		fgl::ArrayList<TransitionData> transitions;
		
		ScreenElement* element;

		struct ChildScreenContainer
		{
			Screen* screen;
			float zLayer;
			bool visible;
		};
		fgl::ArrayList<ChildScreenContainer> childScreens;
		Screen* parentScreen;

		Screen* presentedScreen;
		std::function<void()> presentedScreenDismissCallback;
		ScreenManager* screenManager;
		
		bool isshown;
		bool drawsParent;
		bool updatesParent;
		// Used to tell if the Screen is drawing an overlay. This value is only set within the draw function.
		mutable bool drawingOverlayTransition;
		
		void updateFrame(Window*window);
		virtual void setWindow(Window*window);

		//---TouchData---
		
		typedef struct
		{
			unsigned int touchID;
			bool state[4];
			Vector2d pos;
		} MouseTouchData;
		ArrayList<MouseTouchData> currentTouches;
		
		static size_t getTouchDataIndex(ArrayList<MouseTouchData>& touches, unsigned int touchID);
		static ArrayList<unsigned int> getUnlistedTouchIDs(ArrayList<MouseTouchData>& touches, ArrayList<unsigned int>& touchIDs);
		
		void updateElementMouse(ApplicationData appData);
		void updateElementTouch(ApplicationData appData);
	};
}
