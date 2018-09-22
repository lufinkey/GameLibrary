
#include <GameLibrary/Animation/AnimationProvider.hpp>

namespace fgl
{
	AnimationProvider::~AnimationProvider() {
		for(auto& pair : animations) {
			delete pair.second;
		}
	}
	
	void AnimationProvider::addAnimation(const String& name, Animation* animation) {
		if(animation == nullptr) {
			throw fgl::IllegalArgumentException("animation", "cannot be null");
		}
		if(name.length() == 0) {
			throw fgl::IllegalArgumentException("name", "cannot be an empty string");
		}
		if(hasAnimation(name)) {
			throw fgl::IllegalArgumentException("name", "duplicate animation name");
		}
		animations[name] = animation;
	}
	
	bool AnimationProvider::hasAnimation(const String& name) const {
		return (animations.find(name) != animations.end());
	}
	
	Animation* AnimationProvider::removeAnimation(const String& name) {
		auto it = animations.find(name);
		if(it == animations.end()) {
			return nullptr;
		}
		auto animation = it->second;
		animations.erase(it);
		return animation;
	}
	
	Animation* AnimationProvider::getAnimation(const String& name) const {
		return animations.at(name);
	}
	
	String AnimationProvider::getAnimationName(Animation* animation) const {
		for(auto& pair : animations) {
			if(pair.second == animation) {
				return pair.first;
			}
		}
		return "";
	}
}
