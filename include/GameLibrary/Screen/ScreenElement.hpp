
#pragma once

#include <mutex>
#include <GameLibrary/Application/ApplicationData.hpp>
#include <GameLibrary/Graphics/Graphics.hpp>
#include <GameLibrary/Input/Mouse.hpp>
#include <GameLibrary/Input/Multitouch.hpp>
#include <GameLibrary/Utilities/ArrayList.hpp>
#include "AutoLayoutManager.hpp"

namespace fgl
{
	class Screen;

	/*! A stackable element that can be added to a Screen */
	class ScreenElement
	{
		friend class Screen;
		friend class ScreenManager;
	public:
		/*! default constructor*/
		ScreenElement();
		/*! Constructs an element with a given frame (bounding box).
			\param frame the bounding box of the element*/
		explicit ScreenElement(const RectangleD& frame);
		/*! virtual destructor*/
		virtual ~ScreenElement();
		
		
		ScreenElement(const ScreenElement&) = delete;
		ScreenElement& operator=(const ScreenElement&) = delete;
		
		
		/*! Updates any properties of the element, and updates all the child elements.
			\param appData specifies information about the Application updating the element, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData*/
		virtual void update(ApplicationData appData);
		/*! Draws the element and all of its child elements. This function calls drawBackground, drawMain, and drawElements respectively.
			\param appData specifies information about the Application drawing the element, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the element*/
		virtual void draw(ApplicationData appData, Graphics graphics) const;
		
		
		/*! Called to layout child elements inside this element. */
		void layoutChildElements();
		/*! Called to layout child elements inside this element if needed */
		void layoutChildElementsIfNeeded();
		/*! Called to flag that this element needs its children's layout */
		void setNeedsLayout();
		/*! Sets the actual frame (bounding box) of the element inside of its parent.
			\param frame a RectangleD object representing the element's bounding box within its parent*/
		void setFrame(const RectangleD& frame);
		/*! Gets the actual frame (bounding box) of the element inside of its parent.
			\returns a RectangleD object representing the element's bounding box within its parent*/
		RectangleD getFrame() const;
		/*! Gets the frame of the element reduced on all sides by the border width.
			\returns a RectangleD object representing the frame the element inside of its border*/
		RectangleD getBorderPaddedFrame() const;
		/*! Gets the frame of the element inside the given parent element */
		RectangleD getFrameInsideParent(ScreenElement* parent) const;
		/*! Moves the frame to have it's center coordinates at the given point.
			\param center the center coordinates to set */
		void setCenter(const Vector2d& center);
		/*! Gets the center coordinates of the frame.
			\returns a Vector2d point*/
		Vector2d getCenter() const;
		
		
		/*! Adds a child element to this element.
			\param element a ScreenElement pointer
			\throws fgl::IllegalArgumentException if element is null, or if the element is already a child of another ScreenElement*/
		void addChildElement(ScreenElement* element);
		/*! Inserts a child element in this element at a given index.
			\param index the index in the child elements array to insert the element
			\param element a ScreenElement pointer
			\throws fgl::IllegalArgumentException if the element is null or if the element is already a child of another ScreenElement*/
		void addChildElement(size_t index, ScreenElement* element);
		/*! Removes this element from its parent element, or returns if it has no parent element.
			\throws fgl::IllegalStateException if this element is not stored within its parent element*/
		void removeFromParentElement();
		/*! Brings a child element to the front of the element list.
			\param element a ScreenElement pointer
			\throws fgl::IllegalArgumentException if element is null, or if the element is not a child of this element
			\throws fgl::IllegalStateException if this element is not stored within its parent element*/
		void bringChildElementToFront(ScreenElement* element);
		/*! Sends a child element to the back of the element list.
			\param element a ScreenElement pointer
			\throws fgl::IllegalArgumentException if element is null, or if the element is not a child of this element
			\throws fgl::IllegalStateException if this element is not stored within its parent element*/
		void sendChildElementToBack(ScreenElement* element);
		
		
		/*! Gets the list of child elements of this element.
			\returns a const ArrayList reference of ScreenElement pointers*/
		const ArrayList<ScreenElement*>& getChildElements() const;
		/*! Gets the parent element of this element.
			\returns a ScreenElement pointer*/
		ScreenElement* getParentElement() const;
		
		
		/*! Sets a layout rule to automatically adjust the frame whenever the parent element's frame changes.
		If this element is the child of another element, then the frame will adjust itself accordingly when this function is called.
			\param ruleType the type of layout rule \see fgl::LayoutRuleType
			\param value the value of the rule given, in the specified units
			\param valueType the units of the value parameter \see fgl::LayoutValueType*/
		void setLayoutRule(const LayoutRuleType& ruleType, double value, const LayoutValueType& valueType=LAYOUTVALUE_PIXEL);
		/*! Clears all layout rules and sets them from a given dictionary of layout rules
			\param rules a dictionary of layout rules
			\example
				 <key>LAYOUTRULE_LEFT</key>
				 <dict>
					<key>value</key>
					<real>0.4</real>
					<key>valueType</key>
					<string>LAYOUTVALUE_RATIO</string>
				 </dict>*/
		void setLayoutRules(const Dictionary& rules);
		/*! Tells whether any layout rules have been set for this element.
			\returns true if this element has set layout rules, or false if otherwise*/
		bool hasLayoutRules() const;
		/*! Removes any set layout rules from this element.*/
		void removeAllLayoutRules();
		/*! Gets the layout manager for this element.
			\returns a const AutoLayoutManager reference*/
		const AutoLayoutManager& getAutoLayoutManager() const;
		
		
		/*! Sets the background color of this element. If the color is equal to Colors::TRANSPARENT, the background will not be drawn.
			\param color a Color value*/
		void setBackgroundColor(const Color& color);
		/*! Gets the background of this element.
			\returns a const Color value reference*/
		const Color& getBackgroundColor() const;


		/*! Sets the border width of this element. If the width is 0, the border will not be drawn.
			\param borderWidth the width of the border, in pixels */
		void setBorderWidth(float borderWidth);
		/*! Gets the width of the border on this element.
			\returns the border width, in pixels */
		float getBorderWidth() const;
		/*! Sets the border color of this element. If the color is equal to Colors::TRANSPARENT, the background color will not be drawn.
			\param borderColor the color to draw the border */
		void setBorderColor(const Color& borderColor);
		/*! Gets the border color of this element.
			\returns the border color */
		const Color& getBorderColor() const;


		/*! Sets the transparency level of the element
			\param alpha a value between 0 and 1, with 1 being fully visible and 0 being fully invisible */
		void setAlpha(float alpha);
		/*! Gets the transparency level of the element
			\returns a value between 0 and 1, with 1 being fully visible and 0 being fully invisible */
		float getAlpha() const;
		
		
		/*! Sets this element's visibility. If an element is set to not be visible, it will not be drawn, and neither will its children.
			\param toggle true to make this element visible, or false to make this element invisible*/
		void setVisible(bool toggle);
		/*! Tells whether this element's visibility is enabled. This does not tell if all parents are visible.
			\returns true if this element is visible, or false if otherwise*/
		bool isVisible() const;
		/*! Tells whether this element, and all parent elements in the heirarchy, are visible
			\returns true if this element and all parents are visible, or false if this element or a parent is not visible*/
		bool isVisibleInHeirarchy() const;
		
		/*! Sets whether this element clips its child elements to its frame.
			\param toggle true to make this element clip its child elements to its frame, or false to display its child elements normally*/
		void setClippedToFrame(bool toggle);
		/*! Tells whether this element clips its child elements to its frame.
			\returns true if clipping is enabled, or false if clipping is not enabled*/
		bool isClippedToFrame() const;

		
		//! Represents a touch event on a ScreenElement
		class TouchEvent
		{
			friend class Screen;
		public:
			typedef enum
			{
				EVENTTYPE_TOUCHDOWN,
				EVENTTYPE_TOUCHUP,
				EVENTTYPE_TOUCHMOVE,
				EVENTTYPE_TOUCHCANCEL
			} EventType;
			
			static fgl::String EventType_toString(EventType eventType);
			
			TouchEvent withAppData(const ApplicationData& appData) const;

			const EventType& getEventType() const;
			unsigned int getTouchID() const;
			const ApplicationData& getApplicationData() const;
			Vector2d getPosition() const;
			bool isMouseEvent() const;
			
			fgl::String toString() const;

		private:
			TouchEvent(const EventType& eventType, unsigned int touchID, ApplicationData appData, const Vector2d& realPosition, bool isMouse);
			
			EventType eventType;
			unsigned int touchID;
			ApplicationData appData;
			Vector2d realPosition;
			TransformD inverseTransform;
			bool mouse;
		};
		
	protected:
		/*! Lays out child elements in parent frame */
		virtual void onLayoutChildElements();
		/*! Updates all the child elements of this element. This function is automatically called from ScreenElement::update.
			\param appData specifies information about the Application drawing the element, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData*/
		virtual void updateElements(ApplicationData appData);
		/*! Draws the background color of the element. This function is automatically called from ScreenElement::draw.
			\param appData specifies information about the Application drawing the element, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the element*/
		virtual void drawBackground(ApplicationData appData, Graphics graphics) const;
		/*! Draws the main content of the element. This does nothing by default, and is intended to be overridden. This function is automatically called from ScreenElement::draw.
			\param appData specifies information about the Application drawing the element, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the element*/
		virtual void drawMain(ApplicationData appData, Graphics graphics) const;
		/*! Draws the border on top of the element. This function is called from ScreenElement::draw.
			\param appData specifies information about the Application drawing the element, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the element*/
		virtual void drawBorder(ApplicationData appData, Graphics graphics) const;
		/*! Draws all the child elements of this element. This function is automatically called from ScreenElement::draw.
			\param appData specifies information about the Application drawing the element, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the element*/
		virtual void drawElements(ApplicationData appData, Graphics graphics) const;
		
		
		/*! Called to apply any necessary changes to the ApplicationData before it's passed to the child elements
			\param appData the ApplicationData object to change
			\returns the modified ApplicationData */
		virtual ApplicationData getChildrenApplicationData(ApplicationData appData) const;
		/*! Called to apply any necessary changes to the Graphics before it's passed to the child elements
			\param graphics the Graphics object to change
			\returns the modified Graphics */
		virtual Graphics getChildrenGraphics(Graphics graphics) const;
		
		
		/*! Called when this element is added to an element that is stored in a Screen, which is presented on, or is itself, a root screen.
			\param window the Window pointer*/
		virtual void onAddToWindow(Window* window);
		/*! Called when this element is removed from an element that is stored in a Screen, which is presented on, or is itself, a root screen.
			\param window the Window pointer*/
		virtual void onRemoveFromWindow(Window* window);
		
		
		/*! Called when this element is added to a parent element.
			\param parent the parent element*/
		virtual void onAddToScreenElement(ScreenElement* parent);
		/*! Called when this element is removed from a parent element.
			\param parent the parent element*/
		virtual void onRemoveFromScreenElement(ScreenElement* parent);


		/*! Handles or ignores a given touch event. Override this method to provide custom touch behavior.
			\param touchEvent the event that occured
			\returns true if the element handles the event, false if the element ignores the event */
		virtual bool handleTouchEvent(const TouchEvent& touchEvent);
		/*! Called when a sibling higher in the element stack or a parent of this element handles a touch event.
			\param touchEvent the event that occured */
		virtual void otherElementHandledTouchEvent(const TouchEvent& touchEvent);
		
	private:
		bool sendTouchEvent(const TouchEvent& touchEvent);
		void sendHandledTouchEvent(const TouchEvent& touchEvent);
		
		void handleAddToWindow(Window* window);
		void handleRemoveFromWindow(Window* window);

		void autoLayoutFrame();

		RectangleD frame;
		ScreenElement* parentElement;
		ArrayList<ScreenElement*> childElements;
		
		AutoLayoutManager autoLayoutMgr;
		
		Color backgroundColor;
		float borderWidth;
		Color borderColor;
		float alpha;

		bool visible;
		bool clipsToFrame;
		bool needsLayout;
	};
}
