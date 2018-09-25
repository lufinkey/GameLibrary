
#pragma once

#include <GameLibrary/World/World.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>
#include <list>

namespace fgl
{
	class Direction2DListener;
	
	typedef std::function<void(const ApplicationData& appData, Vector2d speed, Vector2d prevSpeed)> SpeedApplyerFunc;
	
	struct SpeedApplyerOptions
	{
		SpeedApplyerOptions() : SpeedApplyerOptions(100) {}
		SpeedApplyerOptions(double speed) : maxSpeedUpChange(speed * 8), maxSlowDownChange(speed * 4), maxAirSpeedUpChange(speed * 6), maxAirSlowDownChange(speed * 2) {}
		
		double maxSpeedUpChange;
		double maxSlowDownChange;
		double maxAirSpeedUpChange;
		double maxAirSlowDownChange;
	};
	
	
	
	class Direction2DAspect : public WorldObjectAspect
	{
	public:
		Direction2DAspect(const Vector2d& direction=Vector2d(0,0), double speed=50.0);
		
		virtual void update(const ApplicationData& appData);
		
		void setDirection(const Vector2d& direction);
		const Vector2d& getDirection() const;
		
		void setSpeed(double speed);
		double getSpeed() const;
		
		void setSpeedApplyer(SpeedApplyerFunc speedApplyer);
		const SpeedApplyerFunc& getSpeedApplyer() const;
		
		void addListener(Direction2DListener* listener);
		void removeListener(Direction2DListener* listener);
		
		static SpeedApplyerFunc createVelocity2DXSpeedApplyer(WorldObject* object, SpeedApplyerOptions options);
		
	protected:
		virtual void onChangeDirection(Vector2d direction);
		
	private:
		Vector2d direction;
		double speed;
		Vector2d prevSpeed;
		SpeedApplyerFunc speedApplyer;
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
