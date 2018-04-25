
#pragma once

#include "ImageElement.hpp"
#include <GameLibrary/Screen/ScreenElement.hpp>
#include <GameLibrary/Actor/AnimationPlayer.hpp>

namespace fgl
{
	/*! Displays an Animation object.*/
	class AnimationElement : public ScreenElement
	{
	public:
		/*! default constructor*/
		AnimationElement();
		/*! \copydoc fgl::ScreenElement::ScreenElement(const RectangleD&frame)*/
		explicit AnimationElement(const RectangleD& frame);
		/*! Constructs a screen element with a frame and an animation to display with a given display mode
			\param frame the frame (bounding box) of the element
			\param animation the animation to display inside the element
			\param direction the direction that frames should iterate for the Animation when animating */
		AnimationElement(const RectangleD& frame, Animation* animation, const Animation::Direction& direction = Animation::Direction::FORWARD);
		/*! Constructs a screen element with an animation to display with a given display mode. The default frame is (0,0,0,0)
			\param animation the animation to display inside the element */
		AnimationElement(Animation* animation, const Animation::Direction& direction = Animation::Direction::FORWARD);
		/*! virtual destructor*/
		virtual ~AnimationElement();
		
		
		/*! \copydoc fgl::ScreenElement::update(fgl::ApplicationData)*/
		virtual void update(ApplicationData appData) override;
		/*! \copydoc fgl::ScreenElement::setFrame(const fgl::RectangleD&)*/
		virtual void layoutChildElements() override;
		
		
		/*! Sets the Animation for the element to display.
			\param animation the Animation to display, or null to not display an animation
			\param direction the direction that frames should iterate for the Animation when animating. If NO_CHANGE is used, the current frame index and direction are preserved through the change*/
		void setAnimation(Animation* animation, const Animation::Direction& direction = Animation::Direction::FORWARD);
		/*! Sets the direction that frames should iterate for the Animation when animating
			\param direction a constant representing the animation direction*/
		void setAnimationDirection(const Animation::Direction& direction);
		/*! Sets the current frame of animation.
			\param frameIndex the index of the frame*/
		void setAnimationFrameIndex(size_t frameIndex);
		
		/*! Gets the current Animation being displayed.
			\returns an Animation pointer, or null if no animation is being displayed*/
		Animation* getAnimation() const;
		/*! Gets the direction of Animation
			\returns a constant representing the direction that frames are iterating through the Animation*/
		const Animation::Direction& getAnimationDirection() const;
		/*! Returns the index of the current frame of animation.
			\returns an unsigned integer representing the current frame index*/
		size_t getAnimationFrameIndex() const;

		/*! Gets the image element being used to display the animation.
			\returns a pointer to an ImageElement instance */
		ImageElement* getImageElement() const;
		
	private:
		void updateAnimationImage();

		AnimationPlayer animationPlayer;
		ImageElement* imageElement;
	};
}
