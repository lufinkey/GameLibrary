
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
		lastFrameTime(0),
		animationChanged(false)
	{
		if(direction == Animation::NO_CHANGE)
		{
			direction = Animation::FORWARD;
		}
		if(animation!=nullptr)
		{
			animationChanged = true;
			if(direction==Animation::BACKWARD)
			{
				frameIndex = animation->getTotalFrames() - 1;
			}
		}
	}
	
	void AnimationPlayer::update(const ApplicationData& appData, const std::function<void(AnimationEvent)>& eventHandler)
	{
		long long currentTimeMillis = appData.getTime().getMilliseconds();
		
		//handle animation
		bool frameChanged = false;
		do
		{
			frameChanged = false;
			if(animationChanged)
			{
				lastFrameTime = currentTimeMillis;
				animationChanged = false;
			}
			if(animation!=nullptr && animation->getTotalFrames() > 0)
			{
				long double fps = (long double)animation->getFPS();
				long long frameTime = (long long)(1000.0L/fps);
				if(frameTime==0)
				{
					frameTime = 1;
				}
				long long nextFrameTime = lastFrameTime + frameTime;
				if(nextFrameTime<=currentTimeMillis)
				{
					lastFrameTime = nextFrameTime;
					if(direction!=Animation::STOPPED)
					{
						frameChanged = true;
						if(direction==Animation::FORWARD)
						{
							frameIndex++;
							if(frameIndex >= animation->getTotalFrames())
							{
								frameIndex = 0;
								if(eventHandler)
								{
									eventHandler(ANIMATIONEVENT_FINISHED);
									if(!animationChanged)
									{
										eventHandler(ANIMATIONEVENT_FRAMECHANGED);
									}
								}
							}
							else
							{
								if(eventHandler)
								{
									eventHandler(ANIMATIONEVENT_FRAMECHANGED);
								}
							}
						}
						else if(direction==Animation::BACKWARD)
						{
							frameIndex--;
							if(frameIndex == -1)
							{
								frameIndex = animation->getTotalFrames()-1;
								if(eventHandler)
								{
									eventHandler(ANIMATIONEVENT_FINISHED);
									if(!animationChanged)
									{
										eventHandler(ANIMATIONEVENT_FRAMECHANGED);
									}
								}
							}
							else
							{
								if(eventHandler)
								{
									eventHandler(ANIMATIONEVENT_FRAMECHANGED);
								}
							}
						}
					}
				}
			}
		} while(animationChanged || frameChanged);
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
			lastFrameTime = 0;
			animationChanged = true;
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
					lastFrameTime = 0;
					animationChanged = true;
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
					lastFrameTime = 0;
					animationChanged = true;
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
