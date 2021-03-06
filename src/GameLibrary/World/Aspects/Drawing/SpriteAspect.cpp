
#include <GameLibrary/World/Aspects/Drawing/SpriteAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <limits>

namespace fgl
{
	SpriteAspect::SpriteAspect()
		: SpriteAspect((Animation*)nullptr) {
		//
	}
	
	SpriteAspect::SpriteAspect(Animation* animation)
		: animator(animation),
		tintColor(Colors::WHITE),
		alpha(1.0) {
		//
	}
	
	SpriteAspect::SpriteAspect(AnimationProvider* animProvider, Animation* animation)
		: animator(animProvider, animation),
		tintColor(Colors::WHITE),
		alpha(1.0) {
		//
	}
	
	SpriteAspect::SpriteAspect(AnimationProvider* animProvider, String animationName)
		: SpriteAspect(animProvider, animProvider->getAnimation(animationName)) {
		//
	}
	
	void SpriteAspect::update(const ApplicationData& appData) {
		animator.update(appData);
	}
	
	Animator* SpriteAspect::getAnimator() {
		return &animator;
	}
	
	const Animator* SpriteAspect::getAnimator() const {
		return &animator;
	}
	
	bool SpriteAspect::shouldDraw(const DrawManager* drawManager) const {
		return true;
	}
	
	
	
	
	void SpriteAspect::draw(DrawContext context, Graphics graphics) const {
		for(auto delegate : delegates) {
			if(!delegate->shouldDrawSprite(this, context)) {
				return;
			}
		}
		
		if(tintColor != Colors::WHITE) {
			graphics.compositeTintColor(tintColor);
		}
		if(alpha < 1.0) {
			graphics.compositeAlpha((float)alpha);
		}
		auto transform2d = getAspect<Transform2DAspect>();
		auto position = transform2d->getPosition();
		auto origin = getOrigin();
		auto size = animator.getSize();
		if(scale.x != 1.0 || scale.y != 1.0) {
			graphics.scale(scale, position);
		}
		animator.draw(graphics, RectangleD(position-origin, size));
	}
	
	
	
	
	void SpriteAspect::setOriginProvider(std::function<Vector2d(const SpriteAspect*)>& provider) {
		originProvider = provider;
	}
	
	const std::function<Vector2d(const SpriteAspect*)>& SpriteAspect::getOriginProvider() const {
		return originProvider;
	}
	
	Vector2d SpriteAspect::getOrigin() const {
		if(originProvider) {
			return originProvider(this);
		}
		auto size = animator.getSize();
		return size / 2.0;
	}
	
	void SpriteAspect::setTintColor(Color tintColor_arg) {
		tintColor = tintColor_arg;
	}
	
	Color SpriteAspect::getTintColor() const {
		return tintColor;
	}
	
	void SpriteAspect::setAlpha(double alpha_arg) {
		alpha = alpha_arg;
	}
	
	double SpriteAspect::getAlpha() const {
		return alpha;
	}
	
	void SpriteAspect::setScale(Vector2d scale_arg) {
		scale = scale_arg;
	}
	
	Vector2d SpriteAspect::getScale() const {
		return scale;
	}
	
	
	
	
	void SpriteAspect::addDelegate(SpriteDelegate* delegate) {
		delegates.push_back(delegate);
	}
	
	void SpriteAspect::removeDelegate(SpriteDelegate* delegate) {
		auto delegateIt = std::find(delegates.begin(), delegates.end(), delegate);
		if(delegateIt != delegates.end()) {
			delegates.erase(delegateIt);
		}
	}
	
	void SpriteAspect::addListener(SpriteListener* listener) {
		listeners.push_back(listener);
		if(listeners.size() == 1) {
			animator.addListener(this);
		}
	}
	
	void SpriteAspect::removeListener(SpriteListener* listener) {
		auto listenerIt = std::find(listeners.begin(), listeners.end(), listener);
		if(listenerIt != listeners.end()) {
			listeners.erase(listenerIt);
			if(listeners.size() == 0) {
				animator.removeListener(this);
			}
		}
	}
	
	
	
	void SpriteAspect::onAnimationChange(Animator* animator) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onAnimationChange(this);
			}
		}
	}
	
	
	
	void SpriteAspect::onAnimationFrameChange(Animator* animator) {
		if(listeners.size() > 0) {
			auto tmpListeners = listeners;
			for(auto listener : tmpListeners) {
				listener->onAnimationFrameChange(this);
			}
		}
	}
}
