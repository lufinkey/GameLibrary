
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>
#include <GameLibrary/Utilities/Font/Font.hpp>

namespace fgl
{
	typedef enum : Uint8
	{
		TEXTALIGN_LEFT,
		TEXTALIGN_CENTER,
		TEXTALIGN_RIGHT
	} TextAlignment;

	typedef enum : Uint8
	{
		VERTICALALIGN_TOP,
		VERTICALALIGN_CENTER,
		VERTICALALIGN_BOTTOM
	} VerticalAlignment;

	class TextElement : public ScreenElement
	{
	public:
		TextElement();
		TextElement(const RectangleD& frame);

		virtual void drawMain(ApplicationData appData, Graphics graphics) const override;

		void setText(const String& text);
		const String& getText() const;

		void setTextAlignment(TextAlignment textAlignment);
		TextAlignment getTextAlignment() const;

		void setVerticalTextAlignment(VerticalAlignment verticalTextAlignment);
		VerticalAlignment getVerticalTextAlignment() const;
		
		void setTextColor(const Color& textColor);
		const Color& getTextColor() const;

		void setFont(Font* font);
		Font* getFont() const;

		void setFontSize(unsigned int fontSize);
		unsigned int getFontSize() const;
		
		double measureTextHeight(double frameWidth) const;

	private:
		struct MeasuredLine
		{
			WideString text;
			Vector2d size;
		};

		ArrayList<MeasuredLine> measureLines() const;

		String text;
		TextAlignment textAlignment;
		VerticalAlignment verticalTextAlignment;
		Color textColor;
		Font* font;
		unsigned int fontSize;
	};
}
