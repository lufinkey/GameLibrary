
#include <GameLibrary/Screen/UI/TextElement.hpp>
#include <limits>

namespace fgl
{
	TextElement::TextElement() : TextElement(RectangleD(0,0,0,0))
	{
		//
	}

	TextElement::TextElement(const RectangleD& frame)
		: ScreenElement(frame),
		textAlignment(TEXTALIGN_LEFT),
		verticalTextAlignment(VERTICALALIGN_TOP),
		textColor(Colors::BLACK),
		font(Graphics::getDefaultFont()),
		fontSize(18),
		fontSizeWidthAdjustmentEnabled(false),
		fontSizeHeightAdjustmentEnabled(false)
	{
		//
	}

	void TextElement::drawMain(ApplicationData appData, Graphics graphics) const
	{
		auto fontSize = getAdjustedFontSize();
		ArrayList<MeasuredLine> lines = measureLines(fontSize);
		font->setSize(fontSize);
		graphics.setFont(font);
		graphics.setColor(textColor);
		RectangleD frame = getFrame();
		graphics.clip(frame);
		double y = frame.y;
		double fullHeight = 0;
		for(auto& line : lines)
		{
			fullHeight += line.size.y;
		}
		switch(verticalTextAlignment)
		{
			case VERTICALALIGN_TOP:
			y = frame.y;
			break;

			case VERTICALALIGN_CENTER:
			y = frame.y+(frame.height/2)-(fullHeight/2);
			break;

			case VERTICALALIGN_BOTTOM:
			y = frame.y+frame.height-fullHeight;
			break;
		}
		switch(textAlignment)
		{
			case TEXTALIGN_LEFT:
			for(auto& line : lines)
			{
				graphics.drawString(line.text, Vector2d(frame.x, y+line.size.y));
				y += line.size.y;
			}
			break;

			case TEXTALIGN_CENTER:
			{
				double centerX = frame.x+(frame.width/2);
				for(auto& line : lines)
				{
					graphics.drawString(line.text, Vector2d(centerX-(line.size.x/2), y+line.size.y));
					y += line.size.y;
				}
			}
			break;

			case TEXTALIGN_RIGHT:
			{
				double rightX = frame.x+frame.width;
				for(auto& line : lines)
				{
					graphics.drawString(line.text, Vector2d(rightX-line.size.x, y+line.size.y));
					y += line.size.y;
				}
			}
			break;
		}
	}

	ArrayList<TextElement::MeasuredLine> TextElement::measureLines(unsigned int fontSize) const
	{
		if(font==nullptr)
		{
			return {};
		}
		WideString wideText = text.toBasicString<wchar_t>();
		ArrayList<MeasuredLine> lines;
		size_t lastLineStart = 0;
		char16_t lastChar = '\0';
		for(size_t wideText_length=wideText.length(), i=0; i<wideText_length; i++)
		{
			char16_t c = wideText[i];
			if(c==u'\n')
			{
				if(lastChar!=u'\r')
				{
					MeasuredLine measuredLine;
					measuredLine.text = wideText.substring(lastLineStart, i);
					measuredLine.size = (Vector2d)font->measureString(measuredLine.text, fontSize);
					lines.add(measuredLine);
					lastLineStart = i+1;
				}
				else
				{
					lastLineStart = i+1;
				}
			}
		}
		if(lastLineStart<wideText.length())
		{
			MeasuredLine measuredLine;
			measuredLine.text = wideText.substring(lastLineStart, wideText.length());
			measuredLine.size = (Vector2d)font->measureString(measuredLine.text, fontSize);
			lines.add(measuredLine);
		}
		return lines;
	}

	void TextElement::setText(const String& text_arg)
	{
		text = text_arg;
	}

	const String& TextElement::getText() const
	{
		return text;
	}

	void TextElement::setTextAlignment(TextAlignment textAlignment_arg)
	{
		textAlignment = textAlignment_arg;
	}
	
	TextAlignment TextElement::getTextAlignment() const
	{
		return textAlignment;
	}

	void TextElement::setVerticalTextAlignment(VerticalAlignment verticalTextAlignment_arg)
	{
		verticalTextAlignment = verticalTextAlignment_arg;
	}

	VerticalAlignment TextElement::getVerticalTextAlignment() const
	{
		return verticalTextAlignment;
	}
	
	void TextElement::setTextColor(const fgl::Color& textColor_arg)
	{
		textColor = textColor_arg;
	}
	
	const Color& TextElement::getTextColor() const
	{
		return textColor;
	}
	
	void TextElement::setFont(Font* font_arg)
	{
		font = font_arg;
	}
	
	Font* TextElement::getFont() const
	{
		return font;
	}
	
	void TextElement::setFontSize(unsigned int fontSize_arg)
	{
		fontSize = fontSize_arg;
	}
	
	unsigned int TextElement::getFontSize() const
	{
		return fontSize;
	}

	void TextElement::setFontSizeWidthAdjustmentEnabled(bool enabled)
	{
		fontSizeWidthAdjustmentEnabled = enabled;
	}

	bool TextElement::isFontSizeWidthAdjustmentEnabled() const
	{
		return fontSizeWidthAdjustmentEnabled;
	}

	void TextElement::setFontSizeHeightAdjustmentEnabled(bool enabled)
	{
		fontSizeHeightAdjustmentEnabled = enabled;
	}

	bool TextElement::isFontSizeHeightAdjustmentEnabled() const
	{
		return fontSizeHeightAdjustmentEnabled;
	}

	unsigned int TextElement::getAdjustedFontSize() const
	{
		auto frame = getFrame();
		return getAdjustedFontSize(Vector2d(frame.width, frame.height));
	}

	unsigned int TextElement::getAdjustedFontSize(const Vector2d& frameSize) const
	{
		if(fontSizeWidthAdjustmentEnabled || fontSizeHeightAdjustmentEnabled)
		{
			auto measuredLines = measureLines(fontSize);
			double longestLine = 0;
			double totalHeight = 0;
			for(auto& line : measuredLines)
			{
				if(line.size.x > longestLine)
				{
					longestLine = line.size.x;
				}
				totalHeight += line.size.y;
			}
			if(totalHeight>0 && longestLine>0)
			{
				double widthAdjustment = 1.0;
				if(fontSizeWidthAdjustmentEnabled)
				{
					if(longestLine > frameSize.x)
					{
						widthAdjustment = frameSize.x/longestLine;
					}
				}
				double heightAdjustment = 1.0;
				if(fontSizeHeightAdjustmentEnabled)
				{
					if(totalHeight > frameSize.y)
					{
						heightAdjustment = frameSize.y/totalHeight;
					}
				}
				double adjustment = Math::min(widthAdjustment, heightAdjustment);
				return (unsigned int)(adjustment*(double)fontSize);
			}
		}
		return fontSize;
	}
	
	double TextElement::measureTextHeight(double frameWidth) const
	{
		double textHeight = 0;
		ArrayList<MeasuredLine> lines = measureLines(getAdjustedFontSize(Vector2d(frameWidth, std::numeric_limits<double>::max())));
		for(auto& line : lines)
		{
			textHeight += line.size.y;
		}
		return textHeight;
	}
}
