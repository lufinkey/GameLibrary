
#pragma once

#include "Drawable2DAspect.hpp"
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>
#include <GameLibrary/Animation/Animator.hpp>

namespace fgl
{
	class SpriteDelegate;
	
	
	
	class SpriteAspect : public Drawable2DAspect
	{
	public:
		SpriteAspect(Animation* animation = nullptr);
		
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
		
	private:
		Animator animator;
		Color tintColor;
		double alpha;
		std::function<Vector2d(const SpriteAspect*)> originProvider;
		
		std::list<SpriteDelegate*> delegates;
	};
	
	
	
	
	class SpriteDelegate
	{
	public:
		virtual ~SpriteDelegate() = default;
		
		virtual bool shouldDrawSprite(const SpriteAspect* sprite, DrawContext context) const { return true; }
	};
}
