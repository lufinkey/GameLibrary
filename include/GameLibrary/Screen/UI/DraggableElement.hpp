
#pragma once

#include "TouchElement.hpp"

namespace fgl
{
	class DraggableElement : public TouchElement
	{
	public:
		/*! DraggableElement constructor; */
		DraggableElement();
		/*! \copydoc fgl::ScreenElement::ScreenElement(const RectangleD&frame)*/
		explicit DraggableElement(const RectangleD& frame);
		
		bool isDragPointConstrainedToParentBounds() const;
		void setDragPointConstrainedToParentBounds(bool constrained);
		
		bool isDragFrameConstrainedToParentBounds() const;
		void setDragFrameConstrainedToParentBounds(bool constrained);
		
		bool isDragging() const;
		
	protected:
		virtual void onTouchDown(const TouchEvent& touchEvent) override;
		virtual bool onTouchMove(const TouchEvent& touchEvent) override;
		virtual void onTouchUpInside(const TouchEvent& touchEvent) override;
		virtual void onTouchUpOutside(const TouchEvent& touchEvent) override;
		virtual void onTouchCancel(const TouchEvent& touchEvent) override;
		
		virtual void onDragStart(const TouchEvent& touchEvent);
		virtual void onDragMove(const TouchEvent& touchEvent);
		virtual void onDragStop(const TouchEvent& touchEvent);
		
	private:
		void startDrag(const TouchEvent& touchEvent);
		void moveDrag(const TouchEvent& touchEvent);
		void stopDrag(const TouchEvent& touchEvent);
		
		bool draggable;
		bool constrainDragPointToParentBounds;
		bool constrainDragFrameToParentBounds;
		bool dragging;
		unsigned int dragTouchId;
		fgl::Vector2d dragTouchOffset;
	};
}
