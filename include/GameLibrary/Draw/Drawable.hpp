
#pragma once

#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Graphics/Graphics.hpp>
#include "DrawContext.hpp"

namespace fgl
{
	class Drawable
	{
	public:
		Drawable();
		virtual ~Drawable() = default;
		
		virtual String getDebugDescription() const;
		virtual bool getFlag(const String& flag) const;
		
		virtual bool shouldDraw(const DrawManager* drawManager) const;
		virtual void draw(DrawContext context, Graphics graphics) const = 0;
	};
}
