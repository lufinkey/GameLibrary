
#pragma once

#include "Animation.hpp"
#include "AnimationPlayer.hpp"
#include "AnimationProvider.hpp"
#include <GameLibrary/Application/ApplicationData.hpp>

namespace fgl
{
	class AnimatorListener;
	
	enum class AnimatorEvent
	{
		FRAME_CHANGED,
		FINISHED,
		CHANGED
	};
	
	
	
	class Animator
	{
	public:
		Animator(Animation* animation=nullptr);
		Animator(AnimationProvider* provider, Animation* animation=nullptr);
		
		void update(const ApplicationData& appData);
		
		void draw(Graphics graphics, RectangleD rect) const;
		
		void changeAnimation(const String& name);
		void changeAnimation(const String& name, Animation::Direction direction);
		void changeAnimation(const String& name, const std::function<void(AnimatorEvent)>& onEvent);
		void changeAnimation(const String& name, Animation::Direction direction, const std::function<void(AnimatorEvent)>& onEvent);
		void changeAnimation(Animation* animation);
		void changeAnimation(Animation* animation, Animation::Direction direction);
		void changeAnimation(Animation* animation, const std::function<void(AnimatorEvent)>& onEvent);
		void changeAnimation(Animation* animation, Animation::Direction direction, const std::function<void(AnimatorEvent)>& onEvent);
		Animation* getAnimation(const String& name) const;
		Animation* getAnimation() const;
		String getAnimationName(Animation* animation) const;
		String getAnimationName() const;
		Vector2d getSize() const;
		size_t getFrameIndex() const;
		
		void setAnimationProvider(AnimationProvider* animationProvider);
		AnimationProvider* getAnimationProvider() const;
		
		void addListener(AnimatorListener* listener);
		void removeListener(AnimatorListener* listener);
		
		void synchronizeFrames(Animator* cmpAnimator);
		
		const std::function<void(AnimatorEvent)>& getAnimationEventHandler() const;
		AnimationPlayer* getPlayer();
		const AnimationPlayer* getPlayer() const;
		
	private:
		AnimationPlayer animPlayer;
		std::function<void(AnimatorEvent)> animEventHandler;
		AnimationProvider* animProvider;
		std::list<AnimatorListener*> listeners;
		bool animationChanged;
	};
	
	
	
	class AnimatorListener {
		friend class Animator;
	public:
		virtual ~AnimatorListener() = default;
		
	protected:
		virtual void onAnimationChange(Animator* animator) {}
		virtual void onAnimationFrameChange(Animator* animator) {}
	};
}
