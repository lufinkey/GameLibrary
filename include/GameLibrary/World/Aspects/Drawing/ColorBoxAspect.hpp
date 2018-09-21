
#pragma once

#include "Drawable2DAspect.hpp"

namespace fgl
{
	class ColorBoxAspect : public Drawable2DAspect
	{
	public:
		ColorBoxAspect(const RectangleD& rect, Color color);
		
		virtual void draw(DrawContext context, Graphics graphics) const override;
		
		void setRect(const RectangleD& rect);
		const RectangleD& getRect() const;
		
		void setColor(Color color);
		Color getColor() const;
		
		void setFilled(bool filled);
		bool isFilled() const;
		
	private:
		RectangleD rect;
		Color color;
		bool filled;
	};
}
