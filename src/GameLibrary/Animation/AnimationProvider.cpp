
#include <GameLibrary/Animation/AnimationProvider.hpp>

namespace fgl
{
	AnimationProvider::~AnimationProvider()
	{
		for(auto& pair : animations)
		{
			auto& node = pair.second;
			if(node.autoDelete)
			{
				delete node.animation;
			}
		}
	}
	
	void AnimationProvider::addAnimation(const String& name, Animation* animation, bool autoDelete)
	{
		if(animation == nullptr)
		{
			throw fgl::IllegalArgumentException("animation", "cannot be null");
		}
		if(hasAnimation(name))
		{
			throw fgl::IllegalArgumentException("name", "duplicate animation name");
		}
		AnimNode node;
		node.animation = animation;
		node.autoDelete = autoDelete;
		animations.set(name, node);
	}
	
	bool AnimationProvider::hasAnimation(const String& name) const
	{
		return animations.has(name);
	}
	
	void AnimationProvider::removeAnimation(const String& name)
	{
		if(hasAnimation(name))
		{
			return;
		}
		auto node = animations.get(name);
		if(node.autoDelete)
		{
			delete node.animation;
		}
		animations.remove(name);
	}
	
	Animation* AnimationProvider::getAnimation(const String& name) const
	{
		auto node = animations.get(name);
		return node.animation;
	}
}
