
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
	
	AnimationElement::AnimationElement(const RectangleD& frame, Animation* animation, const Animation::Direction& direction)
		: ScreenElement(frame),
		animation(animation),
		animation_direction(direction),
		animation_frame(0),
		animation_prevFrameTime(0),
		prevUpdateTime(0),
		firstUpdate(true),
		imageElement(nullptr)
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
		imageElement = new ImageElement(RectangleD(0, 0, frame.width, frame.height));
		updateAnimationImage();
		addChildElement(imageElement);
	}
	
	AnimationElement::AnimationElement(Animation* animation, const Animation::Direction& direction)
		: AnimationElement(RectangleD(0,0,0,0), animation, direction)
	{
		//
	}
	
	AnimationElement::~AnimationElement()
	{
		delete imageElement;
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

					updateAnimationImage();
				}
			}
		}
	}

	void AnimationElement::setFrame(const RectangleD& frame)
	{
		ScreenElement::setFrame(frame);
		imageElement->setFrame(RectangleD(0, 0, frame.width, frame.height));
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

		updateAnimationImage();
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
		updateAnimationImage();
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

	ImageElement* AnimationElement::getImageElement() const
	{
		return imageElement;
	}

	void AnimationElement::updateAnimationImage()
	{
		if(animation!=nullptr)
		{
			if(animation->getTotalFrames()>0)
			{
				imageElement->setImage(animation->getImage(animation_frame));
				imageElement->setImageSourceRect(animation->getImageSourceRect(animation_frame));
			}
			else
			{
				imageElement->setImage(nullptr);
				imageElement->setImageSourceRect(ImageElement::DEFAULT_SRC_RECT);
			}
		}
		else
		{
			imageElement->setImage(nullptr);
			imageElement->setImageSourceRect(ImageElement::DEFAULT_SRC_RECT);
		}
	}
}
