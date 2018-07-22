
#pragma once

#include "Animation.hpp"
#include <map>

namespace fgl
{
	class AnimationProvider
	{
	public:
		virtual ~AnimationProvider();
		
		void addAnimation(const String& name, Animation* animation, bool autoDelete=false);
		bool hasAnimation(const String& name) const;
		void removeAnimation(const String& name);
		
		virtual Animation* getAnimation(const String& name) const;
		virtual String getAnimationName(Animation* animation) const;
		
	private:
		struct AnimNode
		{
			Animation* animation = nullptr;
			bool autoDelete = false;
		};
		
		std::map<String, AnimNode> animations;
	};
}
