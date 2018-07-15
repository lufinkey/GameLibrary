
#pragma once

#include "Animation.hpp"
#include <GameLibrary/Utilities/Dictionary.hpp>

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
		
	private:
		struct AnimNode
		{
			Animation* animation = nullptr;
			bool autoDelete = false;
		};
		
		BasicDictionary<String, AnimNode> animations;
	};
}

REGISTER_TYPE(fgl, AnimationProvider)
