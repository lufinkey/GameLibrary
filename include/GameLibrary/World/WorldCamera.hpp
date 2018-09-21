
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>

namespace fgl
{
	class World;
	
	
	class WorldCamera
	{
		friend class World;
		friend class WorldElement;
	public:
		WorldCamera(const RectangleD& frame = RectangleD(0, 0, 360, 240), const Vector2d& center = { 0,0 }, const Vector2d& resolution = { 360, 240 });
		virtual ~WorldCamera();
		
		RectangleD getFrame() const;
		void setFrame(const RectangleD& frame);
		
		Vector2d getResolution() const;
		void setResolution(const Vector2d& resolution);
		
		Vector2d getCenter() const;
		void setCenter(const Vector2d& center);
		
		ScreenElement* getScreenElement();
		const ScreenElement* getScreenElement() const;
		
		RectangleD getVisibleRect() const;
		
		World* getWorld();
		const World* getWorld() const;
		
	protected:
		virtual void drawWorld(const World* world, ApplicationData appData, Graphics graphics) const;
		
	private:
		class WorldElement : public ScreenElement {
			friend class WorldCamera;
		public:
			WorldElement(const RectangleD& frame, WorldCamera* camera);
		protected:
			virtual void drawMain(ApplicationData appData, Graphics graphics) const;
		private:
			WorldCamera* camera;
		};
		
		Vector2d center;
		Vector2d resolution;
		ScreenElement* screenElement;
		World* world;
	};
}
