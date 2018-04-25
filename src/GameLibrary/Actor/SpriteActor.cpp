
#include <GameLibrary/Actor/SpriteActor.hpp>
#include <GameLibrary/Graphics/PixelIterator.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <GameLibrary/Exception/IllegalStateException.hpp>

namespace fgl
{
	SpriteActorAnimationEvent::SpriteActorAnimationEvent(const SpriteActorAnimationEvent& event)
		: target(event.target),
		name(event.name),
		animation(event.animation)
	{
		//
	}
	
	SpriteActorAnimationEvent& SpriteActorAnimationEvent::operator=(const SpriteActorAnimationEvent& event)
	{
		target = event.target;
		name = event.name;
		animation = event.animation;
		return *this;
	}
	
	SpriteActorAnimationEvent::SpriteActorAnimationEvent(SpriteActor* target, const String& name, Animation* animation)
		: target(target),
		name(name),
		animation(animation)
	{
		//
	}
	
	SpriteActor* SpriteActorAnimationEvent::getTarget() const
	{
		return target;
	}
	
	const String& SpriteActorAnimationEvent::getAnimationName() const
	{
		return name;
	}
	
	Animation* SpriteActorAnimationEvent::getAnimation() const
	{
		return animation;
	}
	
	SpriteActor::SpriteActor() : SpriteActor(0,0)
	{
		//
	}
	
	SpriteActor::SpriteActor(double x1, double y1)
	{
		x = x1;
		y = y1;
	}
	
	SpriteActor::~SpriteActor()
	{
		for(unsigned int i=0; i<animations.size(); i++)
		{
			AnimationInfo& info = animations.get(i);
			if(info.destruct)
			{
				delete info.animation;
			}
		}
	}
	
	void SpriteActor::update(ApplicationData appData)
	{
		animationPlayer.update(appData, [&](AnimationPlayer::AnimationEvent event){
			if(event==AnimationPlayer::ANIMATIONEVENT_FINISHED)
			{
				onAnimationFinish(SpriteActorAnimationEvent(this, animationName, animationPlayer.getAnimation()));
			}
		});
		Actor::update(appData);
	}

	void SpriteActor::draw(ApplicationData appData, Graphics graphics) const
	{
		drawActor(appData, graphics, x, y, scale);
	}
	
	void SpriteActor::drawActor(ApplicationData&appData, Graphics&graphics, double x, double y, double scale) const
	{
		if(visible && scale!=0 && animationPlayer.getAnimation()!=nullptr)
		{
			graphics.translate(x, y);
			Graphics boundingBoxGraphics(graphics);
			if(rotation!=0)
			{
				graphics.rotate(rotation);
			}
			Graphics frameGraphics(graphics);
			if(mirroredHorizontal)
			{
				if(mirroredVertical)
				{
					graphics.scale(-scale,-scale);
				}
				else
				{
					graphics.scale(-scale,scale);
				}
			}
			else
			{
				if(mirroredVertical)
				{
					graphics.scale(scale,-scale);
				}
				else
				{
					graphics.scale(scale,scale);
				}
			}

			Actor::draw(appData, graphics);
			
			Graphics actorGraphics(graphics);
			actorGraphics.compositeTintColor(color);
			actorGraphics.setAlpha((float)alpha);
			
			animationPlayer.draw(actorGraphics);
			
			if(frame_visible)
			{
				frameGraphics.setColor(frame_color);
				frameGraphics.drawRect(-(width/2), -(height/2), width, height);
				boundingBoxGraphics.setColor(frame_color);
				boundingBoxGraphics.drawRect(-(framesize.x/2), -(framesize.y/2), framesize.x, framesize.y);
			}
		}
	}
	
	RectangleD SpriteActor::getFrame() const
	{
		return RectangleD(x-(framesize.x/2), y-(framesize.y/2), framesize.x, framesize.y);
	}
	
	void SpriteActor::scaleToFit(const RectangleD&container)
	{
		if(width==0 || height==0)
		{
			double oldScale = scale;
			setScale(1);
			if(width==0 || height==0)
			{
				setScale(oldScale);
				x = container.x + (container.width/2);
				y = container.y + (container.height/2);
				return;
			}
		}
		if(container.width==0 || container.height==0)
		{
			setScale(0);
			x = container.x + (container.width/2);
			y = container.y + (container.height/2);
			return;
		}
		RectangleD currentFrame = getFrame();
		RectangleD oldFrame = currentFrame;
		currentFrame.scaleToFit(container);
		double ratio = currentFrame.width/oldFrame.width;
		x = container.x + (container.width/2);
		y = container.y + (container.height/2);
		setScale(getScale()*ratio);
	}
	
	void SpriteActor::onAnimationFinish(const SpriteActorAnimationEvent& evt)
	{
		//Open for implementation
	}
	
	void SpriteActor::updateSize()
	{
		Animation* animation = animationPlayer.getAnimation();
		size_t frameIndex = animationPlayer.getFrameIndex();
		if(animation == nullptr || animation->getFrameCount()==0)
		{
			framesize.x = 0;
			framesize.y = 0;
			width = 0;
			height = 0;
			return;
		}
		
		RectangleD frame = animation->getRect(frameIndex);
		frame.x*=scale;
		frame.y*=scale;
		frame.width*=scale;
		frame.height*=scale;
		
		width = frame.width;
		height = frame.height;
		
		frame = rotationMatrix.transform(frame);
		
		framesize.x = frame.width;
		framesize.y = frame.height;
	}
	
	void SpriteActor::addAnimation(const String&name, Animation*animation, bool destruct)
	{
		if(animation == nullptr)
		{
			throw IllegalArgumentException("animation", "null");
		}
		if(name.length() == 0)
		{
			throw IllegalArgumentException("name", "cannot be empty string");
		}
		if(hasAnimation(name))
		{
			throw IllegalArgumentException("name", "duplicate animation name");
		}
		
		AnimationInfo animInfo;
		animInfo.name = name;
		animInfo.animation = animation;
		animInfo.destruct = destruct;
		
		size_t prevSize = animations.size();
		
		animations.add(animInfo);
		
		if(prevSize == 0)
		{
			changeAnimation(name, Animation::Direction::FORWARD);
		}
	}
	
	void SpriteActor::removeAnimation(const String&name)
	{
		size_t totalAnimations = animations.size();
		for(size_t i=0; i<totalAnimations; i++)
		{
			AnimationInfo animInfo = animations.get(i);
			if(animInfo.name.equals(name))
			{
				if(animInfo.name.equals(animationName))
				{
					animationPlayer.setAnimation(nullptr);
					animationName = "";
				}
				if(animInfo.destruct)
				{
					delete animInfo.animation;
				}
				animations.remove(i);
				return;
			}
		}
	}
	
	bool SpriteActor::hasAnimation(const String&name) const
	{
		size_t totalAnimations = animations.size();
		for(size_t i=0; i<totalAnimations; i++)
		{
			if(animations.get(i).name.equals(name))
			{
				return true;
			}
		}
		return false;
	}
	
	Animation* SpriteActor::getAnimation(const String&name) const
	{
		size_t totalAnimations = animations.size();
		for(size_t i=0; i<totalAnimations; i++)
		{
			const AnimationInfo&animInfo = animations.get(i);
			if(animInfo.name.equals(name))
			{
				return animInfo.animation;
			}
		}
		return nullptr;
	}
	
	void SpriteActor::changeAnimation(const String& name, const Animation::Direction& direction)
	{
		Animation* animation = getAnimation(name);
		if(animation == nullptr)
		{
			throw IllegalArgumentException("name", "animation does not exist");
		}
		
		animationName = name;
		animationPlayer.setAnimation(animation, direction);
		updateSize();
	}
	
	void SpriteActor::changeAnimation(Animation* animation, const Animation::Direction& direction)
	{
		animationName = "";
		animationPlayer.setAnimation(animation, direction);
		updateSize();
	}
	
	bool SpriteActor::checkPointCollision(const Vector2d&point)
	{
		Animation* animation = animationPlayer.getAnimation();
		if(animation==nullptr || animation->getFrameCount()==0)
		{
			return false;
		}
		RectangleD frame = getFrame();
		if(frame.contains(point))
		{
			Vector2d pointFixed = point;
			pointFixed.x -= x;
			pointFixed.y -= y;
			
			pointFixed = inverseRotationMatrix.transform(pointFixed);
			
			pointFixed.x += width/2;
			pointFixed.y += height/2;
			
			if((mirroredHorizontal && !animation->isMirroredHorizontal()) || (!mirroredHorizontal && animation->isMirroredHorizontal()))
			{
				pointFixed.x = width - pointFixed.x;
			}
			if((mirroredVertical && !animation->isMirroredVertical()) || (!mirroredVertical && animation->isMirroredVertical()))
			{
				pointFixed.y = height - pointFixed.y;
			}
			
			double ratX = pointFixed.x/width;
			double ratY = pointFixed.y/height;
			if(ratX < 0 || ratY < 0 || ratX>=1 || ratY>=1)
			{
				return false;
			}
			
			size_t frameIndex = animationPlayer.getFrameIndex();
			auto& frame = animation->getFrame(frameIndex);
			TextureImage* img = frame.getImage();
			RectangleU srcRect = frame.getSourceRect();
			unsigned int pxlX = (unsigned int)(ratX*((double)srcRect.width));
			unsigned int pxlY = (unsigned int)(ratY*((double)srcRect.height));

			return img->checkPixel((unsigned int)srcRect.x+pxlX,(unsigned int)srcRect.y+pxlY);
		}
		return false;
	}
	
	bool SpriteActor::isColliding(SpriteActor* actor) const
	{
		if(actor == nullptr)
		{
			throw IllegalArgumentException("actor", "null");
		}
		Animation* animation = animationPlayer.getAnimation();
		Animation* actor_animation = actor->animationPlayer.getAnimation();
		if(animation==nullptr || actor_animation==nullptr || scale==0 || actor->scale==0)
		{
			return false;
		}
		RectangleD frame = getFrame();
		RectangleD actor_frame = actor->getFrame();
		if(frame.intersects(actor_frame))
		{
			RectangleD overlap = frame.getIntersect(actor_frame);
			RectangleD actor_overlap = actor_frame.getIntersect(frame);
			
			double incr = 1;
			if(scale < actor->scale)
			{
				incr = scale;
			}
			else
			{
				incr = actor->scale;
			}
			
			size_t frameIndex = animationPlayer.getFrameIndex();
			auto& animFrame = animation->getFrame(frameIndex);
			auto img = animFrame.getImage();
			auto srcRect = animFrame.getSourceRect();
			
			size_t actor_frameIndex = actor->animationPlayer.getFrameIndex();
			auto& actor_animFrame = actor_animation->getFrame(actor_frameIndex);
			auto actor_img = actor_animFrame.getImage();
			auto actor_srcRect = actor_animFrame.getSourceRect();
			
			bool mirrorHorizontal = false;
			if(mirroredHorizontal != animation->isMirroredHorizontal())
			{
				mirrorHorizontal = true;
			}
			bool mirrorVertical = false;
			if(mirroredVertical != animation->isMirroredVertical())
			{
				mirrorVertical = true;
			}
			
			bool actor_mirrorHorizontal = false;
			if(actor->mirroredHorizontal != actor_animation->isMirroredHorizontal())
			{
				actor_mirrorHorizontal = true;
			}
			bool actor_mirrorVertical = false;
			if(actor->mirroredVertical != actor_animation->isMirroredVertical())
			{
				actor_mirrorVertical = true;
			}
			
			PixelIterator*pxlIter = nullptr;
			if(rotation == 0)
			{
				Vector2u dimensions((unsigned int)img->getWidth(), (unsigned int)img->getHeight());
				pxlIter = new PixelIterator(dimensions, srcRect, frame, overlap, incr, incr, mirrorHorizontal, mirrorVertical);
			}
			else
			{
				TransformD transform = rotationMatrix;
				transform.translate(-(width/2), -(height/2));
				double ratiox = ((double)srcRect.width)/width;
				double ratioy = ((double)srcRect.height)/height;
				Vector2u dimensions((unsigned int)img->getWidth(), (unsigned int)img->getHeight());
				pxlIter = new PixelIterator(dimensions, srcRect, frame, overlap, incr, incr, transform, Vector2d(ratiox, ratioy), mirrorHorizontal, mirrorVertical);
			}
			PixelIterator& pxlIterRef = *pxlIter;
			
			PixelIterator*actor_pxlIter = nullptr;
			if(actor->rotation == 0)
			{
				Vector2u dimensions((unsigned int)actor_img->getWidth(), (unsigned int)actor_img->getHeight());
				actor_pxlIter = new PixelIterator(dimensions, actor_srcRect, actor_frame, actor_overlap, incr, incr, actor_mirrorHorizontal, actor_mirrorVertical);
			}
			else
			{
				TransformD transform = actor->rotationMatrix;
				transform.translate(-(actor->width/2), -(actor->height/2));
				double ratiox = ((double)actor_srcRect.width)/actor->width;
				double ratioy = ((double)actor_srcRect.height)/actor->height;
				Vector2u dimensions((unsigned int)actor_img->getWidth(), (unsigned int)actor_img->getHeight());
				actor_pxlIter = new PixelIterator(dimensions, actor_srcRect, actor_frame, actor_overlap, incr, incr, transform, Vector2d(ratiox, ratioy), actor_mirrorHorizontal, actor_mirrorVertical);
			}
			PixelIterator& actor_pxlIterRef = *actor_pxlIter;

			bool running = pxlIterRef.nextPixelIndex();
			bool actor_running = actor_pxlIterRef.nextPixelIndex();
			while(running && actor_running)
			{
				double pxlIndex = pxlIterRef.getCurrentPixelIndex();
				double actor_pxlIndex = actor_pxlIterRef.getCurrentPixelIndex();
				bool pxlOn = false;
				if(pxlIndex >= 0)
				{
					pxlOn = img->checkPixel((unsigned int)pxlIndex);
				}
				bool actor_pxlOn = false;
				if(actor_pxlIndex >= 0)
				{
					actor_pxlOn = actor_img->checkPixel((unsigned int)actor_pxlIndex);
				}
				
				if(pxlOn && actor_pxlOn)
				{
					delete pxlIter;
					delete actor_pxlIter;
					return true;
				}
				
				running = pxlIterRef.nextPixelIndex();
				actor_running = actor_pxlIterRef.nextPixelIndex();
			}
			if(running != actor_running)
			{
				delete pxlIter;
				delete actor_pxlIter;
				throw Exception("Unknown collision bug. This exception means there is a bug within the SpriteActor::isColliding function");
			}
			delete pxlIter;
			delete actor_pxlIter;
			return false;
		}
		return false;
	}
}
