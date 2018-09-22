
#pragma once

#include <GameLibrary/World/World.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>
#include <list>

namespace fgl
{
	class Direction2DListener;
	
	
	
	class Direction2DAspect : public WorldObjectAspect
	{
	public:
		Direction2DAspect(const Vector2d& direction=Vector2d(0,0), double speed=50.0, bool listenerEvents=false);
		
		virtual void update(const ApplicationData& appData);
		
		void setDirection(const Vector2d& direction);
		const Vector2d& getDirection() const;
		
		void setSpeed(double speed);
		double getSpeed() const;
		
		void setSpeedTransformFunc(std::function<Vector2d(Vector2d)> speedTransformFunc);
		const std::function<Vector2d(Vector2d)>& getSpeedTransformFunc() const;
		
		void addListener(Direction2DListener* listener);
		void removeListener(Direction2DListener* listener);
		
	protected:
		virtual void onChangeDirection(Vector2d direction);
		
	private:
		Vector2d direction;
		double speed;
		std::function<Vector2d(Vector2d)> speedTransformFunc;
		std::list<Direction2DListener*> listeners;
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
