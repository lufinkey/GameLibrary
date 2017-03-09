
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>

namespace fgl
{
	class TouchElement : public ScreenElement
	{
	public:
		/*! default constructor*/
		TouchElement();
		/*! \copydoc fgl::ScreenElement::ScreenElement(const RectangleD&frame)*/
		explicit TouchElement(const RectangleD&frame);
		
		virtual void update(ApplicationData appData) override;
		
		void setTouchEnabled(bool toggle);
		bool isTouchEnabled() const;
		
	protected:
		virtual void onMouseEnter(unsigned int mouseIndex);
		virtual void onMouseLeave(unsigned int mouseIndex);
		
		virtual void onTouchCancel(const TouchEvent& touchEvent);
		
		virtual void onTouchDown(const TouchEvent& touchEvent);
		virtual void onTouchUpInside(const TouchEvent& touchEvent);
		virtual void onTouchUpOutside(const TouchEvent& touchEvent);
		
		virtual bool onTouchMove(const TouchEvent& evt);
		
		virtual bool handleTouchEvent(const TouchEvent& touchEvent) override;
		virtual void otherElementHandledTouchEvent(const TouchEvent& touchEvent) override;
		
	private:
		size_t getTouchIndex(unsigned int touchID, bool mouse) const;
		void addTouch(const TouchEvent& touchEvent);
		void removeTouch(unsigned int touchID, bool mouse);
		void cancelTouch(const TouchEvent& touchEvent);
		bool isPointInsideFrame(const Vector2d& point) const;

		struct TouchData
		{
			TouchEvent lastEvent;
			bool inside;
		};
		ArrayList<TouchData> touches;
		ArrayList<unsigned int> enteredMouses;

		bool touchEnabled;
	};
}
