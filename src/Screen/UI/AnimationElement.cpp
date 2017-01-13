
#include <GameLibrary/Screen/UI/AnimationElement.hpp>

namespace fgl
{
	AnimationElement::AnimationElement() : AnimationElement(RectangleD(0,0,0,0), nullptr)
	{
		//
	}
	
	AnimationElement::AnimationElement(const RectangleD& frame) : AnimationElement(frame, nullptr)
	{
		//
	}
	
	AnimationElement::AnimationElement(const RectangleD& frame, Animation* animation, const Animation::Direction& direction, const DisplayMode& displayMode)
		: ScreenElement(frame),
		animation(animation),
		animation_direction(direction),
		animation_frame(0),
		animation_prevFrameTime(0),
		prevUpdateTime(0),
		displayMode(displayMode),
		firstUpdate(true)
	{
		if(animation_direction==Animation::NO_CHANGE)
		{
			animation_direction = Animation::FORWARD;
		}
		if(animation!=nullptr)
		{
			if(animation_direction==Animation::BACKWARD)
			{
				animation_frame = animation->getTotalFrames() - 1;
			}
		}
	}
	
	AnimationElement::AnimationElement(Animation* animation, const Animation::Direction& direction, const DisplayMode& displayMode)
		: AnimationElement(RectangleD(0,0,0,0), animation, direction, displayMode)
	{
		//
	}
	
	AnimationElement::~AnimationElement()
	{
		//
	}
	
	void AnimationElement::update(ApplicationData appData)
	{
		prevUpdateTime = appData.getTime().getMilliseconds();
		if(firstUpdate)
		{
			if(animation!=nullptr)
			{
				animation_prevFrameTime = prevUpdateTime;
			}
			firstUpdate = false;
		}
		
		ScreenElement::update(appData);
		
		//update animation loop
		if(animation_direction == Animation::STOPPED)
		{
			animation_prevFrameTime = prevUpdateTime;
		}
		else if(animation!=nullptr)
		{
			float fps = animation->getFPS();
			if(fps!=0)
			{
				long long waitTime = (long long)(1000.0f/fps);
				long long finishTime = animation_prevFrameTime + waitTime;
				if(finishTime <= prevUpdateTime)
				{
					animation_prevFrameTime = prevUpdateTime;
					if(animation_direction == Animation::FORWARD)
					{
						animation_frame++;
						size_t totalFrames = animation->getTotalFrames();
						if(animation_frame >= totalFrames)
						{
							animation_frame = 0;
							//TODO on animation finish
						}
					}
					else if(animation_direction == Animation::BACKWARD)
					{
						if(animation_frame == 0)
						{
							size_t totalFrames = animation->getTotalFrames();
							if(totalFrames > 0)
							{
								animation_frame = totalFrames-1;
							}
							//TODO on animation finish
						}
						else
						{
							animation_frame--;
						}
					}
				}
			}
		}
	}
	
	void AnimationElement::drawMain(ApplicationData appData, Graphics graphics) const
	{
		if(animation!=nullptr && animation->getTotalFrames()>0)
		{
			size_t anim_frame = animation_frame;
			if(anim_frame >= animation->getTotalFrames())
			{
				switch(animation_direction)
				{
					default:
					case Animation::NO_CHANGE:
					case Animation::FORWARD:
					case Animation::STOPPED:
					anim_frame = 0;
					break;

					case Animation::BACKWARD:
					anim_frame = animation->getTotalFrames() - 1;
					break;
				}
			}
			RectangleD animRect = animation->getRect(anim_frame);
			switch(displayMode)
			{
				default:
				case DISPLAY_STRETCH:
				{
					animation->drawFrame(graphics, anim_frame, getFrame());
				}
				break;

				case DISPLAY_FIT:
				{
					RectangleD frame = getFrame();
					if(animRect.width!=0 && animRect.height!=0 && frame.width!=0 && frame.height!=0)
					{
						animRect.scaleToFit(frame);
						animation->drawFrame(graphics, anim_frame, animRect);
					}
				}
				break;

				case DISPLAY_ZOOM:
				{
					RectangleD frame = getFrame();
					if(animRect.width!=0 && animRect.height!=0 && frame.width!=0 && frame.height!=0)
					{
						animRect.scaleToFill(frame);
						graphics.clip(frame);
						animation->drawFrame(graphics, anim_frame, animRect);
					}
				}
				break;

				case DISPLAY_REPEAT:
				{
					RectangleD frame = getFrame();
					graphics.clip(frame);
					unsigned int imageTimesX = (unsigned int)Math::ceil(frame.width/animRect.width);
					unsigned int imageTimesY = (unsigned int)Math::ceil(frame.height/animRect.height);
					for(unsigned int y=0; y<imageTimesY; y++)
					{
						for(unsigned int x=0; x<imageTimesX; x++)
						{
							double animX = frame.x + (animRect.width*((double)x));
							double animY = frame.y + (animRect.height*((double)y));
							animation->drawFrame(graphics, anim_frame, RectangleD(animX, animY, animRect.width, animRect.height));
						}
					}
				}
				break;
			}
		}
	}
	
	void AnimationElement::setAnimation(Animation* anim, const Animation::Direction& direction)
	{
		animation = anim;
		switch(direction)
		{
			case Animation::FORWARD:
			case Animation::STOPPED:
			{
				animation_frame = 0;
				animation_prevFrameTime = prevUpdateTime;
				animation_direction = direction;
			}
			break;

			case Animation::BACKWARD:
			{
				size_t totalFrames = animation->getTotalFrames();
				if(totalFrames>0)
				{
					animation_frame = (totalFrames-1);
				}
				else
				{
					animation_frame = 0;
				}
				animation_prevFrameTime = prevUpdateTime;
				animation_direction = direction;
			}
			break;

			case Animation::NO_CHANGE:
			{
				switch(animation_direction)
				{
					default:
					case Animation::NO_CHANGE:
					case Animation::FORWARD:
					case Animation::STOPPED:
					{
						if(animation_frame >= animation->getTotalFrames())
						{
							animation_frame = 0;
						}
					}
					break;

					case Animation::BACKWARD:
					{
						size_t totalFrames = animation->getTotalFrames();
						if(animation_frame >= totalFrames)
						{
							if(totalFrames>0)
							{
								animation_frame = (totalFrames-1);
							}
							else
							{
								animation_frame = 0;
							}
						}
					}
					break;
				}
			}
		}
	}
	
	void AnimationElement::setAnimationDirection(const Animation::Direction& direction)
	{
		if(direction != Animation::NO_CHANGE)
		{
			animation_direction = direction;
		}
	}
	
	void AnimationElement::setAnimationFrame(size_t frameIndex)
	{
		animation_frame = frameIndex;
	}
	
	void AnimationElement::setDisplayMode(const DisplayMode& mode)
	{
		displayMode = mode;
	}
	
	Animation* AnimationElement::getAnimation() const
	{
		return animation;
	}
	
	const Animation::Direction& AnimationElement::getAnimationDirection() const
	{
		return animation_direction;
	}
	
	size_t AnimationElement::getAnimationFrame() const
	{
		return animation_frame;
	}
	
	const AnimationElement::DisplayMode& AnimationElement::getDisplayMode() const
	{
		return displayMode;
	}
}
