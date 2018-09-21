
#pragma once

#include <GameLibrary/World/World.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>
#include <list>

namespace fgl
{
	class Direction2DAspect : public WorldObjectAspect
	{
	public:
		Direction2DAspect(const Vector2d& direction=Vector2d(0,0), double speed=50.0, bool listenerEvents=false);
		
		virtual void update(const ApplicationData& appData);
		
		void setDirection(const Vector2d& direction);
		const Vector2d& getDirection() const;
		
		void setSpeed(double speed);
		double getSpeed() const;
		
		void setListenerEventsEnabled(bool enabled);
		bool listenerEventsEnabled() const;
		
	protected:
		virtual void onChangeDirection(Vector2d direction);
		
	private:
		Vector2d direction;
		double speed;
		bool listenerEvents;
	};
	
	
	
	class Direction2DListener
	{
		friend class Direction2DAspect;
	public:
		virtual ~Direction2DListener() = default;
		
	protected:
		virtual void onChangeDirection(Direction2DAspect* aspect, Vector2d direction);
	};
}
