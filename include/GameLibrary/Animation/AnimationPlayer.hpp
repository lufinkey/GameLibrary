
#pragma once

#include "Animation.hpp"

namespace fgl
{
	class AnimationPlayer
	{
	public:
		typedef enum
		{
			ANIMATIONEVENT_FRAMECHANGED,
			ANIMATIONEVENT_FINISHED
		} AnimationEvent;
		
		/*! Constructs an AnimationPlayer with no Animation */
		AnimationPlayer();
		/*! Constructs an AnimationPlayer with a given animation and optional direction
			\param animation an Animation pointer, or null for no Animation
			\param direction the direction that frames should iterate when the Animation plays */
		AnimationPlayer(Animation* animation, const Animation::Direction& direction = Animation::Direction::FORWARD);
		
		/*! Updates the AnimationPlayer based on the game time in appData
			\param appData specifies information about the Application updating the Actor, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param eventHandler receives any events that occur during the update */
		void update(const ApplicationData& appData, const std::function<void(AnimationEvent)>& eventHandler=nullptr);
		/*! Draws the animation at (0,0)
			\param graphics the graphics object used to draw the Animation */
		void draw(Graphics& graphics) const;
		/*! Draws the animation
			\param graphics the graphics object used to draw the Animation
			\param dstRect the rectangle that the animation will be drawn within*/
		void draw(Graphics& graphics, const RectangleD& dstRect) const;
		
		/*! Sets the Animation for the player to play.
			\param animation the Animation to play, or null to not display an animation
			\param direction the direction that frames should iterate for the Animation when animating. If NO_CHANGE is used, the current frame index and direction are preserved through the change*/
		void setAnimation(Animation* animation, const Animation::Direction& direction = Animation::Direction::NO_CHANGE);
		/*! Sets the direction that frames should iterate for the Animation when animating
			\param direction a constant representing the animation direction*/
		void setDirection(const Animation::Direction& direction);
		/*! Sets the current frame of animation.
			\param frameIndex the index of the frame*/
		void setFrameIndex(size_t frameIndex);
		
		/*! Gets the current Animation being played.
			\returns an Animation pointer, or null if no animation is being played*/
		Animation* getAnimation() const;
		/*! Gets the direction of the Animation
			\returns a constant representing the direction that frames are iterating through the Animation*/
		const Animation::Direction& getDirection() const;
		/*! Returns the index of the current frame of animation.
			\returns an unsigned integer representing the current frame index*/
		size_t getFrameIndex() const;
		
		/*! Adjusts the last frame time to match the given player's last frame time */
		void synchronizeFrameTime(const AnimationPlayer& cmpPlayer);
		
	private:
		Animation* animation;
		Animation::Direction direction;
		size_t frameIndex;
		long long lastFrameTime;
		bool animationChanged;
	};
}
