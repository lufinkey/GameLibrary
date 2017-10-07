
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
		animationPlayer(animation, direction),
		imageElement(nullptr)
	{
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
		animationPlayer.update(appData);
		updateAnimationImage();
		ScreenElement::update(appData);
	}

	void AnimationElement::layoutChildElements()
	{
		ScreenElement::layoutChildElements();
		RectangleD frame = getFrame();
		imageElement->setFrame(RectangleD(0, 0, frame.width, frame.height));
	}
	
	void AnimationElement::setAnimation(Animation* anim, const Animation::Direction& direction)
	{
		animationPlayer.setAnimation(anim, direction);
		updateAnimationImage();
	}
	
	void AnimationElement::setAnimationDirection(const Animation::Direction& direction)
	{
		animationPlayer.setDirection(direction);
	}
	
	void AnimationElement::setAnimationFrameIndex(size_t frameIndex)
	{
		animationPlayer.setFrameIndex(frameIndex);
		updateAnimationImage();
	}
	
	Animation* AnimationElement::getAnimation() const
	{
		return animationPlayer.getAnimation();
	}
	
	const Animation::Direction& AnimationElement::getAnimationDirection() const
	{
		return animationPlayer.getDirection();
	}
	
	size_t AnimationElement::getAnimationFrameIndex() const
	{
		return animationPlayer.getFrameIndex();
	}

	ImageElement* AnimationElement::getImageElement() const
	{
		return imageElement;
	}

	void AnimationElement::updateAnimationImage()
	{
		Animation* animation = animationPlayer.getAnimation();
		size_t frameIndex = animationPlayer.getFrameIndex();
		if(animation!=nullptr)
		{
			if(animation->getTotalFrames()>0)
			{
				imageElement->setImage(animation->getImage(frameIndex));
				imageElement->setImageSourceRect(animation->getImageSourceRect(frameIndex));
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
