
#include <GameLibrary/World/Aspects/Drawing/ColorBoxAspect.hpp>
#include <GameLibrary/World/Aspects/Movement/Transform2DAspect.hpp>

namespace fgl
{
	ColorBoxAspect::ColorBoxAspect(const RectangleD& rect, Color color)
		: rect(rect),
		color(color),
		filled(true) {
		//
	}
	
	
	void ColorBoxAspect::draw(DrawContext context, Graphics graphics) const {
		auto transform2d = getAspect<Transform2DAspect>();
		auto drawRect = rect.translated(transform2d->getPosition());
		graphics.setColor(color);
		if(filled) {
			graphics.fillRect(drawRect);
		}
		else {
			graphics.drawRect(drawRect);
		}
	}
	
	
	void ColorBoxAspect::setRect(const RectangleD& rect_arg) {
		rect = rect_arg;
	}
	
	const RectangleD& ColorBoxAspect::getRect() const {
		return rect;
	}
	
	void ColorBoxAspect::setColor(Color color_arg) {
		color = color_arg;
	}
	
	Color ColorBoxAspect::getColor() const {
		return color;
	}
	
	void ColorBoxAspect::setFilled(bool filled_arg) {
		filled = filled_arg;
	}
	
	bool ColorBoxAspect::isFilled() const {
		return filled;
	}
}
