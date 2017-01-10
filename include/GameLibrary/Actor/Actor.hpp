
#pragma once

#include <GameLibrary/Screen/UpdateDrawable.hpp>
#include <GameLibrary/Input/Mouse.hpp>

namespace fgl
{
	class ActorMouseEvent
	{
		friend class Actor;
	public:
		enum EventType
		{
			EVENTTYPE_NONE,
			EVENTTYPE_MOUSEENTER,
			EVENTTYPE_MOUSELEAVE,
			EVENTTYPE_MOUSEPRESS,
			EVENTTYPE_MOUSERELEASE
		};
		typedef enum EventType EventType;
		
		/*! copy constructor*/
		ActorMouseEvent(const ActorMouseEvent&);
		/*! assignment operator*/
		ActorMouseEvent& operator=(const ActorMouseEvent&);
		/*! default constructor*/
		ActorMouseEvent();
		/*! Constructs an ActorMouseEvent object*/
		ActorMouseEvent(Actor* target, const EventType& eventType, const ApplicationData& appData, unsigned int mouseIndex, bool multitouch);
		
		
		/*! Gives the target of the mouse event. The target is the Actor which the event was performed on.
			\returns an Actor pointer*/
		Actor* getTarget() const;
		/*! Tells the type of mouse event that occured.
			\returns an event type constant*/
		const EventType& getEventType() const;
		/*! Gives information about the Application updating the Actor, such as the Window object, the View transform, etc. \see fgl::ApplicationData
			\returns a const ApplicationData object reference*/
		const ApplicationData& getApplicationData() const;
		/*! Gives the touchID or mouseIndex of the input. If Multitouch::isAvailable() returns true, touchID represents a multitouch id. Otherwise, touchID represents a mouse index. \see fgl::Mouse \see fgl::Multitouch
			\returns an unsigned int representing an index or id*/
		unsigned int getMouseIndex() const;
		/*! Tells whether this event is a Mouse event or a Multitouch event
			\returns true if Multitouch event, false if Mouse event*/
		bool isMultitouchEvent() const;
		
	private:
		Actor* target;
		EventType eventType;
		ApplicationData appData;
		unsigned int mouseIndex;
		//TODO add support for button attribute, representing the button changed during this event
		bool multitouch;
	};
	
	/*! An overrideable implementation of fgl::UpdateDrawable that provides the necessities to create a dynamic, movable, scalable, rotatable object that can be updated and drawn.*/
	class Actor : public UpdateDrawable
	{
		friend class SpriteActor;
		friend class TextActor;
		friend class WireframeActor;
	public:
		/*! x coordinate of the Actor. The x coordinate is 0 by default.*/
		double x;
		/*! y coordinate of the Actor. The x coordinate is 0 by default.*/
		double y;
		
		
		/*! default constructor*/
		Actor();
		/*! virtual destructor*/
		virtual ~Actor();
		
		
		/*! Updates certain properties of the Actor, such as mouse state, and calls Actor events.
			\param appData specifies information about the Application updating the Actor, such as the Window object, the View transform, etc. \see fgl::ApplicationData*/
		virtual void update(ApplicationData appData) override;
		/*! Draws the Actor to the screen using the specified Graphics object
			\param appData specifies information about the Application drawing the Actor, such as the Window object, the View transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Actor*/
		virtual void draw(ApplicationData appData, Graphics graphics) const override;
		
		
		/*! Gets the actual bounding box of the Actor. The bounding box resizes based on rotation, scaling, and other transformations.
			\returns a RectangleD object representing the Actor's bounding box*/
		virtual RectangleD getFrame() const override;
		
		
		/*! Scales and repositions the Actor to fit within the specified container
			\param container the frame to fit the Actor into.
			\note this function will not work correctly if the Actor's scale value is set to 0*/
		virtual void scaleToFit(const RectangleD& container);
		/*! Scales the Actor to fit within the specified size
			\param size the width and height to fit the Actor within
			\note this function will not work correctly if the Actor's scale value is set to 0*/
		void scaleToFit(const Vector2d& size);
		
		
		/*! Gets the width of the Actor object. This value is affected by scale, but is not affected by rotation.
			\returns a double representing the width of the Actor
			\note this is NOT the same as getting the width from Actor::getFrame()*/
		double getWidth() const;
		/*! Gets the height of the Actor object This value is affected by scale, but is not affected by rotation.
			\returns a double representing the width of the Actor
			\note this is NOT the same as getting the width from Actor::getFrame()*/
		double getHeight() const;
		
		
		/*! Gets the x value of the Actor in the previous frame.
			\returns a double value*/
		double getPreviousX() const;
		/*! Gets the y value of the Actor in the previous frame.
			\returns a double value*/
		double getPreviousY() const;

		
		
		/*! Sets whether the Actor is visible when drawn or not. This does not affect whether the mouse can detect the Actor. Actor is visible by default.
			\param visible true makes the actor visible, false makes the actor invisible*/
		void setVisible(bool visible);
		/*! Sets the color of the Actor. This has a slightly different effect on different subclasses of Actor. Color is white {r=255; g=255; b=255; a=255} by default.
			\link fgl::SpriteActor - composites the color on the sprite
			\link fgl::TextActor - sets the text color
			\link fgl::WireframeActor - sets the wireframe color
			\see fgl::Color
			\param color the color to set the Actor*/
		void setColor(const Color& color);
		/*! Rotates the actor by the specified amount of degrees with the origin at its (x,y) position. Rotation is 0 by default.
			\see fgl::Actor::x
			\see fgl::Actor::y
			\param degrees the rotation, in degrees, to rotate the Actor*/
		void rotate(double degrees);
		/*! Sets the rotation of the Actor to the specified amount of degrees with the origin at its (x,y) position. Rotation does affect mouse states. Rotation is 0 by default.
			\see fgl::Actor::x
			\see fgl::Actor::y
			\param degrees the rotation, in degrees, to set the Actor's rotation*/
		void setRotation(double degrees);
		/*! Sets the transparency level of the Actor. This does not affect whether the mouse can detect the Actor. Alpha is 1 by default (fully visible).
			\param alpha the alpha level, from 0 (fully invisible) to 1 (fully visible)*/
		void setAlpha(double alpha);
		/*! Sets the scale of the Actor with the origin at its (x,y) position. This does affect mouse states. Scale is 1 by default.
			\see fgl::Actor::x
			\see fgl::Actor::y
			\param scale the ratio to scale the Actor from its default size*/
		void setScale(double scale);
		/*! Sets the frame (bounding box) and the border of the Actor to be drawn on top of the Actor when fgl::Actor::draw is called. This is good for testing. Frame is not visible by default.
			\param visible true makes the frame and border visible, false makes the frame and border invisible*/
		void setFrameVisible(bool visible);
		/*! Sets the color of the frame (bounding box) and the border of the Actor. Frame and border color are green {r=0; g=255; b=0; a=255} by default.
			\see fgl::Actor::setFrameVisible(bool)
			\param color the color to draw the frame and border*/
		void setFrameColor(const Color& color);
		/*! Sets whether the Actor is mirrored horizontally or not. If the Actor is mirrored, it will be drawn inverted horizontally. This does affect mouse states. Horizontal mirroring is false by default.
			\param mirror true makes the Actor drawn inverted horizontally, and false makes the Actor drawn correctly horizontally*/
		void setMirroredHorizontal(bool mirror);
		/*! Sets whether the Actor is mirrored vertically or not. If the Actor is mirrored, it will be drawn inverted vertically. This does affect mouse states. Vertical mirroring is false by default.
			\param mirror true makes the Actor drawn inverted horizontally, and false makes the Actor drawn correctly horizontally*/
		void setMirroredVertical(bool mirror);
		/*! Sets whether mouse events are enabled or disabled. Mouse events are enabled by default, but can slow down a game if a large amount of actors are being updated.
			Mouses states remain unchanged while events are disabled.
			\param enabled true enables mouse events and false disables them*/
		void setMouseEventsEnabled(bool enabled);
		
		
		/*! Tells whether the Actor has been set visible. Visibility is true by default.
			\see fgl::Actor::setVisible(bool)
			\returns true means the Actor has been set visible, false means the Actor has been set invisible*/
		bool isVisible() const;
		/*! Gets the color of the Actor.  Color is white {r=255; g=255; b=255; a=255} by default.
			\see fgl::Actor::setColor(const Color&)
			\returns a const reference to a Color object representing the Actor's color*/
		const Color& getColor() const;
		/*! Gets the Actor's rotation, in degrees, on its (x,y) origin. Rotation is 0 by default.
			\see fgl::Actor::setRotation(double)
			\returns a double value representing the Actor's rotation in degrees*/
		double getRotation() const;
		/*! Gets the Actor's alpha value. Alpha is 1 by default (fully visible).
			\see fgl::Actor::setAlpha(double)
			\returns a double representing the alpha level, from 0 (fully invisible) to 1 (fully visible)*/
		double getAlpha() const;
		/*! Gets the Actor's scale ratio. Scale is 1 by default.
			\see fgl::Actor::setScale(double)
			\returns a double representing the ratio the Actor has been scaled*/
		double getScale() const;
		/*! Tells whether the Actor's frame (bounding box) and border have been set visible. Frame and border visibility is false by default.
			\see fgl::Actor::setFrameVisible(bool)
			\returns true if the Actor's frame and border are visible, and false if the frame and border are invisible*/
		bool isFrameVisible() const;
		/*! Gets the color of the Actor's frame (bounding box) and border. Frame and border color are green {r=0; g=255; b=0; a=255} by default.
			\see fgl::Actor::setFrameColor(const Color&)
			\returns a const reference to a Color object representing the Actor's frame and border color*/
		const Color& getFrameColor() const;
		/*! Tells whether the Actor is drawn horizontally inverted. Horizontal mirroring is false by default.
			\see fgl::Actor::setMirroredHorizontal(bool)
			\returns true if the Actor is mirrored horizontally, false if otherwise.*/
		bool isMirroredHorizontal() const;
		/*! Tells whether the Actor is drawn vertically inverted. Vertical mirroring is false by default.
			\see fgl::Actor::setMirroredVertical(bool)
			\returns true if the Actor is mirrored vertically, false if otherwise.*/
		bool isMirroredVertical() const;
		/*! Tells whether mouse events are enabled.
			\return true if mouse events are enabled, or false if they are disabled*/
		bool areMouseEventsEnabled() const;
		
		
		/*! Tells whether the Mouse or a Multitouch input was hovering over the Actor in the most recent update call. This function does not recheck the mouse. The Actor's mouse state gets updated every time fgl::Actor::update is called.
			\see fgl::Actor::update(ApplicationData)
			\returns true if a Mouse or Multitouch input was hovering over the Actor in the most recent update call, and false if otherwise.*/
		bool isMouseOver() const;
		/*! Tells whether the Mouse or a Multitouch input was hovering over the Actor in the previous (before the most recent) update call. This function does not recheck the mouse. The Actor's mouse state gets updated every time fgl::Actor::update is called.
			\see fgl::Actor::update(ApplicationData)
			\returns true if a Mouse or Multitouch input was hovering over the Actor in the previous update call, and false if otherwise.*/
		bool wasMouseOver() const;
		/*! Tells whether the Mouse or a Multitouch input was pressing the Actor in the most recent update call. This function does not recheck the mouse. The Actor's mouse state gets updated every time fgl::Actor::update is called.
			\see fgl::Actor::update(ApplicationData)
			\returns true if a Mouse or Multitouch input was pressing the Actor in the most recent update call, and false if otherwise.*/
		bool isMousePressed() const;
		/*! Tells whether the Mouse or a Multitouch input was pressing the Actor in the previous (before the most recent) call. This function does not recheck the mouse. The Actor's mouse state gets updated every time fgl::Actor::update is called.
			\see fgl::Actor::update(ApplicationData)
			\returns true if a Mouse or Multitouch input was pressing the Actor in the previous update call, and false if otherwise.*/
		bool wasMousePressed() const;
		/*! Tells whether the Actor was just pressed in the most recent update call. This function does not recheck the mouse. The Actor's mouse state gets updated every time fgl::Actor::update is called.
			\see fgl::Actor::update(ApplicationData)
			\returns true if a Mouse or Multitouch input just pressed the Actor in the most recent update call, and false if otherwise.*/
		bool didMousePress() const;
		/*! Tells whether the Actor was just released in the most recent update call. This function does not recheck the mouse. The Actor's mouse state gets updated every time fgl::Actor::update is called.
			\see fgl::Actor::update(ApplicationData)
			\returns true if a Mouse or Multitouch input just released the Actor in the most recent update call, and false if otherwise.
			\note the Actor may still be pressed. This function just checks to see if any touchID or mouseIndex released the Actor.*/
		bool didMouseRelease() const;
		
		
		/*! Clears all the stored mouse and multitouch data from the Actor.
			\note This function does NOT set any of the mouse states (isMousePressed, isMouseReleased, etc.) to false.*/
		void clearMouseState();
		
		
		/*! Called when a Mouse or Multitouch input presses the Actor. This function is called within the Actor's update function. \see fgl::Actor::update(ApplicationData)
			\param evt information about the event that was performed*/
		virtual void onMousePress(const ActorMouseEvent& evt);
		/*! Called when a Mouse or Multitouch input releases the Actor. This function is called within the Actor's update function. \see fgl::Actor::update(ApplicationData)
			\param evt information about the event that was performed*/
		virtual void onMouseRelease(const ActorMouseEvent& evt);
		/*! Called when a Mouse or Multitouch input enters (hovers over) the Actor. This function is called within the Actor's update function. \see fgl::Actor::update(ApplicationData)
			\param evt information about the event that was performed*/
		virtual void onMouseEnter(const ActorMouseEvent& evt);
		/*! Called when a Mouse or Multitouch input leaves (stops hovering over) the Actor. This function is called within the Actor's update function. \see fgl::Actor::update(ApplicationData)
			\param evt information about the event that was performed*/
		virtual void onMouseLeave(const ActorMouseEvent& evt);
		
		
		/*! Updates the Actor's size, based on its properties (scale, rotation, etc.) and stores it. Called by the Actor when new properties have been set through their function in Actor.*/
		virtual void updateSize();
		/*! Checks whether a specified point is colliding with the Actor.
			\param point an (x,y) coordinate to check.
			\returns true if the point collides with the Actor, false if otherwise.
			\note this function is inefficient in loops. It should be used for checking single pixels, and not for pixel level collisions.*/
		virtual bool checkPointCollision(const Vector2d& point);
		
	private:
		double width;
		double height;
		
		double prevx;
		double prevy;
		bool clicked;
		bool prevclicked;
		bool mouseover;
		bool prevmouseover;
		bool didpress;
		bool didrelease;
		bool visible;
		bool mirroredHorizontal;
		bool mirroredVertical;
		bool frame_visible;
		bool mouseEventsEnabled;
		Color color;
		Color frame_color;
		double rotation;
		double alpha;
		double scale;
		
		TransformD rotationMatrix;
		TransformD inverseRotationMatrix;
		
		typedef struct
		{
			unsigned int touchID;
			bool pressed;
		} MouseTouchData;
		ArrayList<MouseTouchData> currentTouches;
		
		bool isTouchDataActive(unsigned int touchID);
		bool isTouchDataPressed(unsigned int touchID);
		void applyTouchData(unsigned int touchID, bool pressed);
		void removeTouchData(unsigned int touchID);
		MouseTouchData* getTouchData(unsigned int touchID);
		ArrayList<unsigned int> getDifTouchData(const ArrayList<unsigned int>&touchIDs);
		
		void updateMouse(ApplicationData& appData);
		void updateTouch(ApplicationData& appData);
		void callMouseEvents(ApplicationData& appData, const ArrayList<ActorMouseEvent>& eventCallData);
	};
}
