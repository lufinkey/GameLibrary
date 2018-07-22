
#include <GameLibrary/Animation/AnimationProvider.hpp>

namespace fgl
{
	AnimationProvider::~AnimationProvider() {
		for(auto& pair : animations) {
			auto& node = pair.second;
			if(node.autoDelete) {
				delete node.animation;
			}
		}
	}
	
	void AnimationProvider::addAnimation(const String& name, Animation* animation, bool autoDelete) {
		if(animation == nullptr) {
			throw fgl::IllegalArgumentException("animation", "cannot be null");
		}
		if(name.length() == 0) {
			throw fgl::IllegalArgumentException("name", "cannot be an empty string");
		}
		if(hasAnimation(name)) {
			throw fgl::IllegalArgumentException("name", "duplicate animation name");
		}
		AnimNode node;
		node.animation = animation;
		node.autoDelete = autoDelete;
		animations[name] = node;
	}
	
	bool AnimationProvider::hasAnimation(const String& name) const {
		return (animations.find(name) != animations.end());
	}
	
	void AnimationProvider::removeAnimation(const String& name) {
		if(hasAnimation(name)) {
			return;
		}
		auto node = animations.at(name);
		if(node.autoDelete) {
			delete node.animation;
		}
		auto it = animations.find(name);
		animations.erase(it);
	}
	
	Animation* AnimationProvider::getAnimation(const String& name) const {
		auto node = animations.at(name);
		return node.animation;
	}
	
	String AnimationProvider::getAnimationName(Animation* animation) const {
		for(auto& pair : animations) {
			if(pair.second.animation == animation) {
				return pair.first;
			}
		}
		return "";
	}
}
