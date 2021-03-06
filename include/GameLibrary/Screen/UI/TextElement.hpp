
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
		TextElement(const RectangleD& frame);
		TextElement(
			const String& text = "",
			TextAlignment alignment = TEXTALIGN_LEFT,
			VerticalAlignment verticalAlignment = VERTICALALIGN_TOP,
			unsigned int size = 18,
			Color color = Colors::BLACK,
			const RectangleD& frame = fgl::RectangleD(0,0,0,0));

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

		void setFontSizeWidthAdjustmentEnabled(bool enabled);
		bool isFontSizeWidthAdjustmentEnabled() const;
		void setFontSizeHeightAdjustmentEnabled(bool enabled);
		bool isFontSizeHeightAdjustmentEnabled() const;
		unsigned int getAdjustedFontSize() const;
		unsigned int getAdjustedFontSize(const Vector2d& frameSize) const;
		
		double measureTextHeight(double frameWidth) const;

	private:
		struct MeasuredLine
		{
			WideString text;
			Vector2d size;
		};

		ArrayList<MeasuredLine> measureLines(unsigned int fontSize) const;

		String text;
		TextAlignment textAlignment;
		VerticalAlignment verticalTextAlignment;
		Color textColor;
		Font* font;
		unsigned int fontSize;
		bool fontSizeWidthAdjustmentEnabled;
		bool fontSizeHeightAdjustmentEnabled;
	};
}
