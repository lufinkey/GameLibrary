
#pragma once

#include "Drawable.hpp"
#include "DrawContext.hpp"

namespace fgl
{
	class DrawManagerDelegate;
	class DrawManagerListener;
	
	
	
	class DrawManager
	{
	public:
		DrawManager();
		
		void update(ApplicationData appData);
		virtual void draw(DrawContext context, Graphics graphics) const;
		
		virtual void addDrawable(Drawable* drawable, std::function<void(Graphics&)> filter=nullptr);
		virtual void removeDrawable(Drawable* drawable);
		
		void addDelegate(DrawManagerDelegate* delegate);
		void removeDelegate(DrawManagerDelegate* delegate);
		
		void addListener(DrawManagerListener* listener);
		void removeListener(DrawManagerListener* listener);
		
		bool shouldDraw(Drawable* drawable) const;
		
	protected:
		virtual void updateDrawables(ApplicationData appData);
		
	private:
		class DrawableNode
		{
		public:
			DrawableNode(Drawable* drawable, std::function<void(Graphics&)> filter=nullptr);
			
			bool equals(const DrawableNode&) const;
			bool operator==(const DrawableNode&) const;
			bool operator!=(const DrawableNode&) const;
			
			String getDebugDescription() const;
			
			Drawable* drawable;
			std::function<void(Graphics&)> filter;
		};
		
		std::list<DrawableNode> drawables;
		
		std::list<DrawManagerDelegate*> delegates;
		std::list<DrawManagerListener*> listeners;
	};
	
	
	
	class DrawManagerDelegate
	{
	public:
		virtual ~DrawManagerDelegate() = default;
		
		virtual bool shouldDraw(const DrawManager* drawManager, const Drawable* drawable) const;
	};
	
	
	class DrawManagerListener
	{
		friend class DrawManager;
	public:
		virtual ~DrawManagerListener() = default;
		
	protected:
		virtual void onBeginDrawManagerUpdates(DrawManager* drawManager);
		virtual void onFinishDrawManagerUpdates(DrawManager* drawManager);
	};
}
