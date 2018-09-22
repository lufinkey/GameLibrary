
#pragma once

#include "Drawable2DAspect.hpp"
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <GameLibrary/Animation/Animator.hpp>

namespace fgl
{
	class SpriteDelegate;
	class SpriteListener;
	
	
	
	class SpriteAspect : public Drawable2DAspect, protected AnimatorListener
	{
	public:
		SpriteAspect(Animation* animation = nullptr);
		SpriteAspect(AnimationProvider* animProvider = nullptr);
		
		Animator* getAnimator();
		const Animator* getAnimator() const;
		
		virtual void update(const ApplicationData& appData) override;
		
		virtual bool shouldDraw(const DrawManager* drawManager) const override;
		virtual void draw(DrawContext context, Graphics graphics) const override;
		
		void setOriginProvider(std::function<Vector2d(const SpriteAspect*)>& provider);
		const std::function<Vector2d(const SpriteAspect*)>& getOriginProvider() const;
		Vector2d getOrigin() const;
		
		void setTintColor(Color tintColor);
		Color getTintColor() const;
		
		void setAlpha(double alpha);
		double getAlpha() const;
		
		void addDelegate(SpriteDelegate* delegate);
		void removeDelegate(SpriteDelegate* delegate);
		
		void addListener(SpriteListener* listener);
		void removeListener(SpriteListener* listener);
		
	protected:
		virtual void onAnimationChange(Animator* animator) override;
		virtual void onAnimationFrameChange(Animator* animator) override;
		
	private:
		Animator animator;
		Color tintColor;
		double alpha;
		std::function<Vector2d(const SpriteAspect*)> originProvider;
		
		std::list<SpriteDelegate*> delegates;
		std::list<SpriteListener*> listeners;
	};
	
	
	
	
	class SpriteDelegate
	{
	public:
		virtual ~SpriteDelegate() = default;
		
		virtual bool shouldDrawSprite(const SpriteAspect* sprite, DrawContext context) const { return true; }
	};
	
	
	
	class SpriteListener
	{
		friend class SpriteAspect;
	public:
		virtual ~SpriteListener() = default;
		
	protected:
		virtual void onAnimationChange(SpriteAspect* sprite) {}
		virtual void onAnimationFrameChange(SpriteAspect* sprite) {}
	};
	
	
	
	
	const auto SPRITEORIGIN_CENTER = [](const SpriteAspect* sprite) -> Vector2d {
		auto size = sprite->getAnimator()->getSize();
		return size / 2.0;
	};
	const auto SPRITEORIGIN_TOPLEFT = [](const SpriteAspect* sprite) -> Vector2d {
		return { 0.0, 0.0 };
	};
	const auto SPRITEORIGIN_TOPRIGHT = [](const SpriteAspect* sprite) -> Vector2d {
		auto size = sprite->getAnimator()->getSize();
		return { size.x, 0.0 };
	};
	const auto SPRITEORIGIN_BOTTOMLEFT = [](const SpriteAspect* sprite) -> Vector2d {
		auto size = sprite->getAnimator()->getSize();
		return { 0.0, size.y };
	};
	const auto SPRITEORIGIN_BOTTOMRIGHT = [](const SpriteAspect* sprite) -> Vector2d {
		auto size = sprite->getAnimator()->getSize();
		return { size.x, size.y };
	};
}
