
#pragma once

#include <GameLibrary/World/WorldObject.hpp>
#include <GameLibrary/Draw/Drawable.hpp>

namespace fgl
{
	class Drawable2DAspect : public WorldObjectAspect, public Drawable
	{
	public:
		virtual String getDebugDescription() const override;
		
	protected:
		virtual void onAddObjectToWorld(World* world) override;
		virtual void onRemoveObjectFromWorld(World* world) override;
	};
}
