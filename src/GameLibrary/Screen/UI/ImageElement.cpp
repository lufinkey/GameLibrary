
#include <GameLibrary/Screen/UI/ImageElement.hpp>

namespace fgl
{
	constexpr RectangleU ImageElement::DEFAULT_SRC_RECT;

	ImageElement::ImageElement() : ImageElement(RectangleD(0,0,0,0),nullptr)
	{
		//
	}
	
	ImageElement::ImageElement(const RectangleD& frame) : ImageElement(frame, nullptr)
	{
		//
	}
	
	ImageElement::ImageElement(const RectangleD& frame, TextureImage* image, const ImageElement::DisplayMode& displayMode)
		: ScreenElement(frame),
		image(image),
		displayMode(displayMode),
		srcrect(DEFAULT_SRC_RECT),
		mirrorImageHorizontally(false),
		mirrorImageVertically(false)
	{
		//
	}
	
	ImageElement::ImageElement(TextureImage* image, const ImageElement::DisplayMode& displayMode)
		: ImageElement(RectangleD(0, 0, 0, 0), image, displayMode)
	{
		//
	}
	
	void ImageElement::drawMain(ApplicationData appData, Graphics graphics) const
	{
		if(image!=nullptr)
		{
			RectangleU srcRect = getImageSourceRect();
			RectangleD drawFrame = getImageDrawFrame();
			if(drawFrame.width!=0 && drawFrame.height!=0)
			{
				RectangleD frame = getFrame();
				graphics.clip(frame);
				if(mirrorImageHorizontally)
				{
					drawFrame.x = drawFrame.x + drawFrame.width;
					drawFrame.width = -drawFrame.width;
				}
				if(mirrorImageVertically)
				{
					drawFrame.y = drawFrame.y + drawFrame.height;
					drawFrame.height = -drawFrame.height;
				}
				if(displayMode==DISPLAY_REPEAT)
				{
					unsigned int imageTimesX = (unsigned int)Math::ceil(Math::abs(frame.width/drawFrame.width));
					unsigned int imageTimesY = (unsigned int)Math::ceil(Math::abs(frame.height/drawFrame.height));
					for(unsigned int y=0; y<imageTimesY; y++)
					{
						for(unsigned int x=0; x<imageTimesX; x++)
						{
							double imageX = drawFrame.x + (drawFrame.width*((double)x));
							double imageY = drawFrame.y + (drawFrame.height*((double)y));
							graphics.drawImage(image, RectangleD(imageX, imageY, drawFrame.width, drawFrame.height), srcRect);
						}
					}
				}
				else
				{
					graphics.drawImage(image, drawFrame, srcRect);
				}
			}
		}
	}

	void ImageElement::setImage(TextureImage*img)
	{
		image = img;
	}

	TextureImage* ImageElement::getImage() const
	{
		return image;
	}
	
	void ImageElement::setDisplayMode(const DisplayMode&mode)
	{
		displayMode = mode;
	}

	ImageElement::DisplayMode ImageElement::getDisplayMode() const
	{
		return displayMode;
	}
	
	void ImageElement::setImageSourceRect(const RectangleU&srcrect_arg)
	{
		srcrect = srcrect_arg;
	}
	
	RectangleU ImageElement::getImageSourceRect() const
	{
		if(image == nullptr)
		{
			return RectangleU(0,0,0,0);
		}
		else if(srcrect.x==-1 && srcrect.y==-1 && srcrect.width==-1 && srcrect.height==-1)
		{
			return RectangleU(0,0,image->getWidth(),image->getHeight());
		}
		else
		{
			return srcrect;
		}
	}

	void ImageElement::setHorizontalImageMirroringEnabled(bool toggle)
	{
		mirrorImageHorizontally = toggle;
	}

	bool ImageElement::isHorizontalImageMirroringEnabled() const
	{
		return mirrorImageHorizontally;
	}

	void ImageElement::setVerticalImageMirroringEnabled(bool toggle)
	{
		mirrorImageVertically = toggle;
	}

	bool ImageElement::isVerticalImageMirroringEnabled() const
	{
		return mirrorImageVertically;
	}

	RectangleD ImageElement::getImageDrawFrame() const
	{
		RectangleD frame = getFrame();
		switch(displayMode)
		{
			case DISPLAY_STRETCH:
			return frame;

			case DISPLAY_FIT_CENTER:
			{
				RectangleU srcRect = getImageSourceRect();
				if(srcRect.width!=0 && srcRect.height!=0 && frame.width!=0 && frame.height!=0)
				{
					RectangleD drawFrame(frame.x, frame.y, (double)srcRect.width, (double)srcRect.height);
					drawFrame.scaleToFit(frame);
					return drawFrame;
				}
				return RectangleD(frame.x+(frame.width/2), frame.y+(frame.height/2), 0, 0);
			}

			case DISPLAY_FILL:
			{
				RectangleU srcRect = getImageSourceRect();
				if(srcRect.width!=0 && srcRect.height!=0 && frame.width!=0 && frame.height!=0)
				{
					RectangleD drawFrame(frame.x, frame.y, (double)srcRect.width, (double)srcRect.height);
					drawFrame.scaleToFill(frame);
					return drawFrame;
				}
				return RectangleD(frame.x+(frame.width/2), frame.y+(frame.height/2), 0, 0);
			}

			case DISPLAY_REPEAT:
			{
				RectangleU srcRect = getImageSourceRect();
				return RectangleD(frame.x, frame.y, (double)srcRect.width, (double)srcRect.height);
			}
		}
		return RectangleD(frame.x+(frame.width/2), frame.y+(frame.height/2), 0, 0);
	}

	RectangleD ImageElement::getImageDisplayFrame() const
	{
		RectangleD frame = getFrame();
		if(displayMode==DISPLAY_REPEAT || displayMode==DISPLAY_STRETCH)
		{
			return frame;
		}
		else
		{
			return frame.getIntersect(getImageDrawFrame());
		}
	}
}
