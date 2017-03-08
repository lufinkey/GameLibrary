
#include <GameLibrary/Actor/AnimationPlayer.hpp>

namespace fgl
{
	AnimationPlayer::AnimationPlayer() : AnimationPlayer(nullptr)
	{
		//
	}
	
	AnimationPlayer::AnimationPlayer(Animation* animation_arg, const Animation::Direction& direction_arg)
		: animation(animation_arg),
		direction(direction_arg),
		frameIndex(0),
		prevFrameTime(0),
		prevUpdateTime(0),
		firstUpdate(true)
	{
		if(direction == Animation::NO_CHANGE)
		{
			direction = Animation::FORWARD;
		}
		if(animation!=nullptr)
		{
			if(direction==Animation::BACKWARD)
			{
				frameIndex = animation->getTotalFrames() - 1;
			}
		}
	}
	
	void AnimationPlayer::update(const ApplicationData& appData, const std::function<void(AnimationEvent)>& eventHandler)
	{
		prevUpdateTime = appData.getTime().getMilliseconds();
		if(firstUpdate)
		{
			if(animation!=nullptr)
			{
				prevFrameTime = prevUpdateTime;
			}
			firstUpdate = false;
		}
		
		bool frameChanged = false;
		bool animationFinished = false;
		
		//update animation loop
		if(direction == Animation::STOPPED)
		{
			prevFrameTime = prevUpdateTime;
		}
		else if(animation!=nullptr)
		{
			float fps = animation->getFPS();
			if(fps!=0)
			{
				long long waitTime = (long long)(1000.0f/fps);
				long long finishTime = prevFrameTime + waitTime;
				if(finishTime <= prevUpdateTime)
				{
					prevFrameTime = prevUpdateTime;
					if(direction == Animation::FORWARD)
					{
						frameIndex++;
						size_t totalFrames = animation->getTotalFrames();
						if(frameIndex >= totalFrames)
						{
							frameIndex = 0;
							animationFinished = true;
						}
						frameChanged = true;
					}
					else if(direction == Animation::BACKWARD)
					{
						if(frameIndex == 0)
						{
							size_t totalFrames = animation->getTotalFrames();
							if(totalFrames > 0)
							{
								frameIndex = totalFrames-1;
							}
							animationFinished = true;
						}
						else
						{
							frameIndex--;
						}
						frameChanged = true;
					}
				}
			}
		}
		
		if(eventHandler)
		{
			if(frameChanged)
			{
				eventHandler(ANIMATIONEVENT_FRAMECHANGED);
			}
			if(animationFinished)
			{
				eventHandler(ANIMATIONEVENT_FINISHED);
			}
		}
	}
	
	void AnimationPlayer::draw(Graphics& graphics) const
	{
		if(animation!=nullptr && animation->getTotalFrames()>0)
		{
			animation->drawFrame(graphics, frameIndex);
		}
	}
	
	void AnimationPlayer::draw(Graphics& graphics, const RectangleD& dstRect) const
	{
		if(animation!=nullptr && animation->getTotalFrames()>0)
		{
			animation->drawFrame(graphics, frameIndex, dstRect);
		}
	}
	
	void AnimationPlayer::setAnimation(Animation* animation_arg, const Animation::Direction& direction_arg)
	{
		animation = animation_arg;
		if(animation==nullptr)
		{
			frameIndex = 0;
			prevFrameTime = prevUpdateTime;
			direction = direction_arg;
		}
		else
		{
			switch(direction_arg)
			{
				case Animation::FORWARD:
				case Animation::STOPPED:
				{
					frameIndex = 0;
					prevFrameTime = prevUpdateTime;
					direction = direction_arg;
				}
				break;
					
				case Animation::BACKWARD:
				{
					size_t totalFrames = animation->getTotalFrames();
					if(totalFrames>0)
					{
						frameIndex = (totalFrames-1);
					}
					else
					{
						frameIndex = 0;
					}
					prevFrameTime = prevUpdateTime;
					direction = direction_arg;
				}
				break;
					
				case Animation::NO_CHANGE:
				{
					switch(direction)
					{
						default:
						case Animation::NO_CHANGE:
						case Animation::FORWARD:
						case Animation::STOPPED:
						{
							if(frameIndex >= animation->getTotalFrames())
							{
								frameIndex = 0;
							}
						}
						break;
							
						case Animation::BACKWARD:
						{
							size_t totalFrames = animation->getTotalFrames();
							if(frameIndex >= totalFrames)
							{
								if(totalFrames>0)
								{
									frameIndex = (totalFrames-1);
								}
								else
								{
									frameIndex = 0;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
	
	void AnimationPlayer::setDirection(const Animation::Direction& direction_arg)
	{
		if(direction_arg != Animation::NO_CHANGE)
		{
			direction = direction_arg;
		}
	}
	
	void AnimationPlayer::setFrameIndex(size_t frameIndex_arg)
	{
		frameIndex = frameIndex_arg;
	}
	
	Animation* AnimationPlayer::getAnimation() const
	{
		return animation;
	}
	
	const Animation::Direction& AnimationPlayer::getDirection() const
	{
		return direction;
	}
	
	size_t AnimationPlayer::getFrameIndex() const
	{
		return frameIndex;
	}
}
