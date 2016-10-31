
#pragma once

#include <GameLibrary/Screen/ScreenElement.h>
#include <GameLibrary/Actor/Animation.h>

namespace fgl
{
	/*! Displays an Animation object.*/
	class AnimationElement : public ScreenElement
	{
	public:
		/*! The method of displaying the animation within the element.*/
		typedef enum
		{
			/*! Stretch the animation to fill the frame*/
			DISPLAY_STRETCH,
			/*! Scale the animation to be contained within the frame, but maintain the aspect ratio of the animation.*/
			DISPLAY_FIT,
			/*! Scale the animation to fill the frame, but don't stretch the animation. If a part of the animation is outside the frame, it is clipped.*/
			DISPLAY_ZOOM,
			/*! Repeat the animation to fill the frame*/
			DISPLAY_REPEAT
		} DisplayMode;
		
		/*! default constructor*/
		AnimationElement();
		/*! \copydoc fgl::ScreenElement::ScreenElement(const RectangleD&frame)*/
		explicit AnimationElement(const RectangleD& frame);
		/*! Constructs a screen element with a frame and an animation to display with a given display mode
			\param frame the frame (bounding box) of the element
			\param animation the animation to display inside the element
			\param direction the direction that frames should iterate for the Animation when animating
			\param displayMode the method to use for displaying the animation; Default value is DISPLAY_STRETCH*/
		AnimationElement(const RectangleD& frame, Animation* animation, const Animation::Direction& direction = Animation::FORWARD, const DisplayMode& displayMode = DISPLAY_STRETCH);
		/*! Constructs a screen element with an animation to display with a given display mode. The default frame is (0,0,0,0)
			\param animation the animation to display inside the element
			\param displayMode the method to use for displaying the animation; Default value is DISPLAY_STRETCH*/
		AnimationElement(Animation* animation, const Animation::Direction& direction = Animation::FORWARD, const DisplayMode&displayMode = DISPLAY_STRETCH);
		/*! virtual destructor*/
		virtual ~AnimationElement();
		
		
		/*! \copydoc fgl::ScreenElement::update(fgl::ApplicationData)*/
		virtual void update(ApplicationData appData) override;
		
		
		/*! Sets the Animation for the element to display.
			\param animation the Animation to display, or null to not display an animation
			\param direction the direction that frames should iterate for the Animation when animating. If NO_CHANGE is used, the current frame index and direction are preserved through the change*/
		void setAnimation(Animation* animation, const Animation::Direction& direction = Animation::FORWARD);
		/*! Sets the direction that frames should iterate for the Animation when animating
			\param direction a constant representing the animation direction*/
		void setAnimationDirection(const Animation::Direction& direction);
		/*! Sets the current frame of animation.
			\param frame the index of the frame*/
		void setAnimationFrame(size_t frameIndex);
		/*! Sets the display method to display the Animation within the element.
			\param mode a constant that dictates how to display the animation \see fgl::AnimationElement::DisplayMode*/
		void setDisplayMode(const DisplayMode& mode);
		
		/*! Gets the current Animation being displayed.
			\returns an Animation pointer, or null if no animation is being displayed*/
		Animation* getAnimation() const;
		/*! Gets the direction of Animation
			\returns a constant representing the direction that frames are iterating through the Animation*/
		const Animation::Direction& getAnimationDirection() const;
		/*! Returns the index of the current frame of animation.
			\returns an unsigned integer representing the current frame index*/
		size_t getAnimationFrame() const;
		/*! Gets the current display method for the Animation within the element.
			\returns a AnimationElement::DisplayMode constant*/
		const DisplayMode& getDisplayMode() const;
		
	protected:
		/*! \copydoc fgl::ScreenElement::drawMain(fgl::ApplicationData,fgl::Graphics)const*/
		virtual void drawMain(ApplicationData appData, Graphics graphics) const override;
		
	private:
		Animation* animation;
		Animation::Direction animation_direction;
		size_t animation_frame;
		long long animation_prevFrameTime;
		long long prevUpdateTime;
		DisplayMode displayMode;
		bool firstUpdate;
	};
}
