
#include <GameLibrary/World/Aspects/Drawing/SpriteAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <limits>

namespace fgl
{
	SpriteAspect::SpriteAspect(Animation* animation)
		: animator(animation),
		tintColor(Colors::WHITE),
		alpha(1.0) {
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
	
	
	
	
	void SpriteAspect::addDelegate(SpriteDelegate* delegate) {
		delegates.push_back(delegate);
	}
	
	void SpriteAspect::removeDelegate(SpriteDelegate* delegate) {
		auto delegateIt = std::find(delegates.begin(), delegates.end(), delegate);
		if(delegateIt != delegates.end()) {
			delegates.erase(delegateIt);
		}
	}
}