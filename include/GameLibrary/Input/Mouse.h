
#pragma once

#include <GameLibrary/Window/Window.h>

namespace fgl
{
	class MouseEventListener;
	
	/*! Represents the Mouse and all Mouse button states on the Mouse*/
	class Mouse
	{
		friend class EventManager;
	public:
		/*! Mouse button on a Mouse*/
		typedef enum
		{
			/*! Unknown or unsupported mouse button*/
			UNKNOWN_BUTTON=0,
			/*! Left mouse button*/
			BUTTON_LEFT=1,
			/*! Right mouse button*/
			BUTTON_RIGHT=2,
			/*! Middle mouse button*/
			BUTTON_MIDDLE=3
		} Button;
		
		
		/*! Gets the total number of mouse instances in a given Window.
			\returns an unsigned int representing the total number of mouse instances*/
		static unsigned int getTotalMouseInstances(Window* window);
		
		
		/*! Checks if a given button on a given mouse instance is pressed.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\param button a constant representing the mouse button to check
			\param returns true if the mouse button is currently in a pressed state, or false if otherwise*/
		static bool isButtonPressed(Window* window, unsigned int mouseIndex, Button button);
		/*! Gets the current position of a mouse instance inside of a given window.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\returns the current frame's mouse coordinates, relative to the given Window*/
		static Vector2d getPosition(Window* window, unsigned int mouseIndex);
		/*! Gets the current x position of a mouse instance inside of a given window.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\returns the current frame's mouse x coordinate, relative to the givem Window*/
		static double getX(Window* window, unsigned int mouseIndex);
		/*! Gets the current y position of a mouse instance inside of a given window.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\returns the current frame's mouse y coordinate, relative to the givem Window*/
		static double getY(Window* window, unsigned int mouseIndex);
		/*! Sets the position of the mouse instance, relative to the given window
			\param window the window to move the mouse in, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\param pos the coordinates to move the mouse pointer to*/
		static void setPosition(Window* window, unsigned int mouseIndex, const Vector2d& pos);
		/*! Sets the position of the mouse instance, relative to the entire screen.
			\note this function currently has no implementation
			\param mouseIndex the mouse instance index
			\param pos the coordinates to move the mouse pointer*/
		static void setPosition(unsigned int mouseIndex, const Vector2d& pos);
		
		
		/*! Checks if a given button on a given mouse instance was in a pressed state in the previous frame
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\param button a constant representing the mouse button to check
			\param returns true if the mouse button was in a pressed state in the previous frame, or false if otherwise*/
		static bool wasButtonPressed(Window* window, unsigned int mouseIndex, Button button);
		/*! Gets the previous frame's position of a mouse instance inside of a given window.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\returns the previous frame's mouse coordinates, relative to the given Window*/
		static Vector2d getPreviousPosition(Window* window, unsigned int mouseIndex);
		/*! Gets the previous frame's x position of a mouse instance inside of a given window.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\returns the previous frame's mouse x coordinate, relative to the givem Window*/
		static double getPreviousX(Window* window, unsigned int mouseIndex);
		/*! Gets the previous frame's y position of a mouse instance inside of a given window.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\returns the previous frame's mouse y coordinate, relative to the givem Window*/
		static double getPreviousY(Window* window, unsigned int mouseIndex);
		
		
		/*! Checks if a mouse button was just pressed.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\param button a constant representing a mouse button to check
			\returns true if Button was just pressed (released state in the previous frame, pressed state in the current frame), or false if otherwise*/
		static bool didButtonPress(Window* window, unsigned int mouseIndex, Button button);
		/*! Checks if a mouse button was just released.
			\param window the window containing the mouse instance, or null for the currently mouse-focused window
			\param mouseIndex the mouse instance index
			\param button a constant representing a mouse button to check
			\returns true if Button was just released (pressed state in the previous frame, released state in the current frame), or false if otherwise*/
		static bool didButtonRelease(Window* window, unsigned int mouseIndex, Button button);
		
		
		/*! Adds a MouseEventListener to handle Mouse events.
			\param eventListener the listener pointer*/
		static void addEventListener(MouseEventListener* eventListener);
		/*! Removes a previously added MouseEventListener.
			\param eventListener the listener pointer*/
		static void removeEventListener(MouseEventListener* eventListener);
		
	private:
		/*! Handles Mouse movement events sent from EventManager.
			\param window the window inside which the mouse event occurred
			\param mouseIndex the mouse instance index
			\param pos the current position of the mouse instance, relative to the window
			\param dif the change in the position of the mouse from the previous position*/
		static void handleMouseMovement(Window* window, unsigned int mouseIndex, const Vector2d& pos, const Vector2d& dif);
		/*! Handles button press events sent from EventManager.
			\param window the window inside which the mouse event occurred
			\param mouseIndex the mouse instance index
			\param button a constant representing the mouse button that was pressed
			\param pos the current position of the mouse instance, relative to the window*/
		static void handleButtonPress(Window* window, unsigned int mouseIndex, Mouse::Button button, const Vector2d& pos);
		/*! Handles button release events sent from EventManager.
			\param window the window inside which the mouse event occurred
			\param mouseIndex the mouse instance index
			\param button a constant representing the mouse button that was released
			\param pos the current position of the mouse instance, relative to the window*/
		static void handleButtonRelease(Window* window, unsigned int mouseIndex, Mouse::Button button, const Vector2d& pos);
		
		
		/*! Removes a Window from the Window list stored in Mouse
			\param window the window pointer*/
		static void removeWindow(Window* window);
		
		
		/*! Updates all button states. Called once per frame by EventManager from the Application in the main thread.*/
		static void update();
	};
	
	
	/*! Event listener that handles Mouse events*/
	class MouseEventListener
	{
	public:
		/*! virtual destructor*/
		virtual ~MouseEventListener(){}
		/*! Called when a Mouse::Button state changes to pressed.
			\param window the window that has mouse focus; May be null if no window has mouse focus
			\param mouseIndex the mouse instance index
			\param button a constant that represents the pressed mouse button
			\param mousepos the mouse instance coordinates, relative to the window*/
		virtual void onMouseButtonPress(Window* window, unsigned int mouseIndex, Mouse::Button button, const Vector2d& mousepos){}
		/*! Called when a Mouse::Button state changes to released.
			\param window the window that has mouse focus; May be null if no window has mouse focus
			\param mouseIndex the mouse instance index
			\param button the constant that represents the mouse button
			\param mousepos the mouse instance coordinates, relative to the window*/
		virtual void onMouseButtonRelease(Window* window, unsigned int mouseIndex, Mouse::Button button, const Vector2d& mousepos){}
		/*! Called when a Mouse instance changes position.
			\param window the window that has mouse focus; May be null if no window has mouse focus
			\param mouseIndex the mouse instance index
			\param mousepos the mouse instance coordinates, relative to the window
			\param mousedif the change in the position of the mouse from the previous position*/
		virtual void onMouseMove(Window* window, unsigned int mouseIndex, const Vector2d& mousepos, const Vector2d& mousedif){}
	};
}