
#include <GameLibrary/Screen/UI/DraggableElement.hpp>

namespace fgl
{
	DraggableElement::DraggableElement()
		: DraggableElement(fgl::RectangleD(0, 0, 0, 0)) {
		//
	}
	
	DraggableElement::DraggableElement(const fgl::RectangleD& frame)
		: TouchElement(frame),
		constrainDragPointToParentBounds(true),
		constrainDragFrameToParentBounds(true),
		dragging(false),
		dragTouchId(-1),
		dragTouchOffset(0,0) {
		//
	}
	
	bool DraggableElement::isDragPointConstrainedToParentBounds() const {
		return constrainDragPointToParentBounds;
	}
	
	void DraggableElement::setDragPointConstrainedToParentBounds(bool constrained) {
		constrainDragPointToParentBounds = constrained;
	}
	
	bool DraggableElement::isDragFrameConstrainedToParentBounds() const {
		return constrainDragFrameToParentBounds;
	}
	
	void DraggableElement::setDragFrameConstrainedToParentBounds(bool constrained) {
		constrainDragFrameToParentBounds = constrained;
	}
	
	bool DraggableElement::isDragging() const {
		return dragging;
	}
	
	
	
	void DraggableElement::onTouchDown(const TouchEvent& touchEvent) {
		if(!dragging) {
			startDrag(touchEvent);
		}
	}
	
	bool DraggableElement::onTouchMove(const TouchEvent& touchEvent) {
		if(dragging && touchEvent.getTouchID() == dragTouchId) {
			moveDrag(touchEvent);
		}
		return true;
	}
	
	void DraggableElement::onTouchUpInside(const TouchEvent& touchEvent) {
		if(dragging && touchEvent.getTouchID() == dragTouchId) {
			stopDrag(touchEvent);
		}
	}
	
	void DraggableElement::onTouchUpOutside(const TouchEvent& touchEvent) {
		if(dragging && touchEvent.getTouchID() == dragTouchId) {
			stopDrag(touchEvent);
		}
	}
	
	void DraggableElement::onTouchCancel(const TouchEvent& touchEvent) {
		if(dragging && touchEvent.getTouchID() == dragTouchId) {
			stopDrag(touchEvent);
		}
	}
	
	
	
	void DraggableElement::onDragStart(const TouchEvent& touchEvent) {
		// open for implementation
	}
	
	void DraggableElement::onDragMove(const TouchEvent& touchEvent) {
		// open for implementation
	}
	
	void DraggableElement::onDragStop(const TouchEvent& touchEvent) {
		// open for implementation
	}
	
	
	
	void DraggableElement::startDrag(const TouchEvent& touchEvent) {
		dragging = true;
		dragTouchId = touchEvent.getTouchID();
		dragTouchOffset = touchEvent.getPosition() - getFrame().getTopLeft();
		onDragStart(touchEvent);
	}
	
	void DraggableElement::moveDrag(const TouchEvent& touchEvent) {
		auto touchPosition = touchEvent.getPosition();
		auto frame = getFrame();
		auto parentElement = getParentElement();
		
		// constrain drag point
		if(parentElement != nullptr && constrainDragPointToParentBounds) {
			auto parentFrame = parentElement->getFrame();
			if(touchPosition.x < 0) {
				touchPosition.x = 0;
			}
			else if(touchPosition.x > parentFrame.width) {
				touchPosition.x = parentFrame.width;
			}
			if(touchPosition.y < 0) {
				touchPosition.y = 0;
			}
			else if(touchPosition.y > parentFrame.height) {
				touchPosition.y = parentFrame.height;
			}
		}
		
		auto framePos = touchPosition - dragTouchOffset;
		frame.x = framePos.x;
		frame.y = framePos.y;
		
		// constrain element frame
		if(parentElement != nullptr && constrainDragFrameToParentBounds) {
			auto parentFrame = parentElement->getFrame();
			if(frame.getRight() > parentFrame.width) {
				auto offsetX = frame.getRight() - parentFrame.width;
				frame.x -= offsetX;
			}
			if(frame.x < 0) {
				frame.x = 0;
			}
			if(frame.getBottom() > parentFrame.height) {
				frame.y -= (frame.getBottom() - parentFrame.height);
			}
			if(frame.y < 0) {
				frame.y = 0;
			}
		}
		
		setFrame(frame);
		onDragMove(touchEvent);
	}
	
	void DraggableElement::stopDrag(const TouchEvent& touchEvent) {
		dragging = false;
		dragTouchId = -1;
		dragTouchOffset = { 0, 0 };
		onDragStop(touchEvent);
	}
}
