
#include <GameLibrary/Animation/Animator.hpp>

namespace fgl
{
	Animator::Animator(Animation* animation)
		: Animator(nullptr, animation) {
		//
	}
	
	Animator::Animator(AnimationProvider* animationProvider, Animation* animation)
		: animPlayer(animation),
		animProvider(animationProvider),
		animationChanged(false),
		mirroredHorizontal(false),
		mirroredVertical(false) {
		//
	}
	
	void Animator::update(const ApplicationData& appData) {
		animPlayer.update(appData, [this](AnimationPlayer::AnimationEvent animEvent) {
			if(animEventHandler) {
				switch(animEvent) {
					case AnimationPlayer::ANIMATIONEVENT_FRAMECHANGED:
						animationChanged = false;
						if(listeners.size() > 0) {
							auto tmpListeners = listeners;
							for(auto listener : tmpListeners) {
								listener->onAnimationFrameChange(this);
							}
						}
						if(!animationChanged) {
							animEventHandler(AnimatorEvent::FRAME_CHANGED);
						}
						break;
						
					case AnimationPlayer::ANIMATIONEVENT_FINISHED:
						animEventHandler(AnimatorEvent::FINISHED);
						break;
				}
			}
		});
	}
	
	void Animator::draw(Graphics graphics, RectangleD rect) const {
		if(mirroredHorizontal) {
			graphics.scale({ -1.0, 1.0 }, rect.getCenter());
		}
		if(mirroredVertical) {
			graphics.scale({ 1.0, -1.0 }, rect.getCenter());
		}
		animPlayer.draw(graphics, rect);
	}
	
	void Animator::changeAnimation(const String& name) {
		changeAnimation(name, Animation::Direction::FORWARD, nullptr);
	}
	
	void Animator::changeAnimation(const String& name, Animation::Direction direction) {
		changeAnimation(name, direction, nullptr);
	}
	
	void Animator::changeAnimation(const String& name, const std::function<void(AnimatorEvent)>& onEvent) {
		changeAnimation(name, Animation::Direction::FORWARD, onEvent);
	}
	
	void Animator::changeAnimation(const String& name, Animation::Direction direction, const std::function<void(AnimatorEvent)>& onEvent) {
		changeAnimation(animProvider->getAnimation(name), direction, onEvent);
	}
	
	void Animator::changeAnimation(Animation* animation) {
		changeAnimation(animation, Animation::Direction::FORWARD, nullptr);
	}
	
	void Animator::changeAnimation(Animation* animation, Animation::Direction direction) {
		changeAnimation(animation, direction, nullptr);
	}
	
	void Animator::changeAnimation(Animation* animation, const std::function<void(AnimatorEvent)>& onEvent) {
		changeAnimation(animation, Animation::Direction::FORWARD, onEvent);
	}
	
	void Animator::changeAnimation(Animation* animation, Animation::Direction direction, const std::function<void(AnimatorEvent)>& onEvent) {
		auto prevAnimation = animPlayer.getAnimation();
		auto prevEventHandler = animEventHandler;
		animPlayer.setAnimation(animation, direction);
		animEventHandler = onEvent;
		animationChanged = true;
		if(animation != prevAnimation || direction != Animation::Direction::NO_CHANGE) {
			if(listeners.size() > 0) {
				auto tmpListeners = listeners;
				for(auto listener : tmpListeners) {
					listener->onAnimationChange(this);
				}
			}
		}
		if(prevEventHandler) {
			prevEventHandler(AnimatorEvent::CHANGED);
		}
	}
	
	Animation* Animator::getAnimation(const String& name) const {
		return animProvider->getAnimation(name);
	}
	
	Animation* Animator::getAnimation() const {
		return animPlayer.getAnimation();
	}
	
	String Animator::getAnimationName(Animation* animation) const {
		return animProvider->getAnimationName(animation);
	}
	
	String Animator::getAnimationName() const {
		auto animation = animPlayer.getAnimation();
		if(animation == nullptr) {
			return "";
		}
		return animProvider->getAnimationName(animation);
	}
	
	Vector2d Animator::getSize() const {
		auto animation = getAnimation();
		size_t frameIndex = getFrameIndex();
		if(animation == nullptr) {
			return Vector2d(0,0);
		}
		return animation->getRect(frameIndex).getSize();
	}
	
	size_t Animator::getFrameIndex() const {
		return animPlayer.getFrameIndex();
	}
	
	
	
	void Animator::setAnimationProvider(AnimationProvider* animationProvider) {
		animProvider = animationProvider;
	}
	
	AnimationProvider* Animator::getAnimationProvider() const {
		return animProvider;
	}
	
	
	
	void Animator::setMirroredHorizontal(bool mirroredHorizontal_arg) {
		mirroredHorizontal = mirroredHorizontal_arg;
	}
	
	bool Animator::isMirroredHorizontal() const {
		return mirroredHorizontal;
	}
	
	void Animator::setMirroredVertical(bool mirroredVertical_arg) {
		mirroredVertical = mirroredVertical_arg;
	}
	
	bool Animator::isMirroredVertical() const {
		return mirroredVertical;
	}
	
	
	
	void Animator::addListener(AnimatorListener* listener) {
		listeners.push_back(listener);
	}
	
	void Animator::removeListener(AnimatorListener* listener) {
		auto listenerIt = std::find(listeners.begin(), listeners.end(), listener);
		if(listenerIt != listeners.end()) {
			listeners.erase(listenerIt);
		}
	}
	
	
	
	AnimationPlayer* Animator::getPlayer() {
		return &animPlayer;
	}
	
	const AnimationPlayer* Animator::getPlayer() const {
		return &animPlayer;
	}
	
	void Animator::synchronizeFrames(Animator* cmpAnimator) {
		animPlayer.synchronizeFrameTime(cmpAnimator->animPlayer);
		size_t frameIndex = cmpAnimator->animPlayer.getFrameIndex();
		if(frameIndex != animPlayer.getFrameIndex() && frameIndex < animPlayer.getAnimation()->getFrameCount()) {
			animPlayer.setFrameIndex(frameIndex);
			if(animEventHandler) {
				animEventHandler(AnimatorEvent::FRAME_CHANGED);
			}
		}
	}
}