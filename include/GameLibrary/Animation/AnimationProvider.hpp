
#pragma once

#include "Animation.hpp"
#include <map>

namespace fgl
{
	class AnimationProvider
	{
	public:
		virtual ~AnimationProvider();
		
		void addAnimation(const String& name, Animation* animation);
		bool hasAnimation(const String& name) const;
		Animation* removeAnimation(const String& name);
		
		virtual Animation* getAnimation(const String& name) const;
		virtual String getAnimationName(Animation* animation) const;
		
	private:
		std::map<String, Animation*> animations;
	};
}
