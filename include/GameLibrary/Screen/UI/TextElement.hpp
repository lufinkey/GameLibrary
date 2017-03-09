
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>
#include <GameLibrary/Utilities/Font/Font.hpp>

namespace fgl
{
	typedef enum
	{
		TEXTALIGN_LEFT,
		TEXTALIGN_CENTER,
		TEXTALIGN_RIGHT
	} TextAlignment;

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
		Color textColor;
		Font* font;
		unsigned int fontSize;
	};
}
