
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
		srcrect(DEFAULT_SRC_RECT)
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
			switch(displayMode)
			{
				default:
				case DISPLAY_STRETCH:
				{
					graphics.drawImage(image, getFrame(), srcRect);
				}
				break;
				
				case DISPLAY_FIT_CENTER:
				{
					RectangleD frame = getFrame();
					double imgwidth = (double)srcRect.width;
					double imgheight = (double)srcRect.height;
					if(imgwidth!=0 && imgheight!=0 && frame.width!=0 && frame.height!=0)
					{
						RectangleD drawFrame(frame.x, frame.y, imgwidth, imgheight);
						drawFrame.scaleToFit(frame);
						graphics.drawImage(image, drawFrame, srcRect);
					}
				}
				break;
				
				case DISPLAY_FILL:
				{
					RectangleD frame = getFrame();

					double imgwidth = (double)srcRect.width;
					double imgheight = (double)srcRect.height;
					if(imgwidth!=0 && imgheight!=0 && frame.width!=0 && frame.height!=0)
					{
						RectangleD drawFrame(frame.x, frame.y, imgwidth, imgheight);
						drawFrame.scaleToFill(frame);
						graphics.clip(frame);
						graphics.drawImage(image, drawFrame, srcRect);
					}
				}
				break;
				
				case DISPLAY_REPEAT:
				{
					RectangleD frame = getFrame();
					graphics.clip(frame);
					double imgwidth = (double)srcRect.width;
					double imgheight = (double)srcRect.height;
					unsigned int imageTimesX = (unsigned int)Math::ceil(frame.width/imgwidth);
					unsigned int imageTimesY = (unsigned int)Math::ceil(frame.height/imgheight);
					for(unsigned int y=0; y<imageTimesY; y++)
					{
						for(unsigned int x=0; x<imageTimesX; x++)
						{
							double imageX = frame.x + (imgwidth*((double)x));
							double imageY = frame.y + (imgheight*((double)y));
							graphics.drawImage(image, RectangleD(imageX, imageY, imgwidth, imgheight), srcRect);
						}
					}
				}
				break;
			}
		}
	}

	void ImageElement::setImage(TextureImage*img)
	{
		image = img;
	}
	
	void ImageElement::setDisplayMode(const DisplayMode&mode)
	{
		displayMode = mode;
	}
	
	void ImageElement::setImageSourceRect(const RectangleU&srcrect_arg)
	{
		srcrect = srcrect_arg;
	}
	
	TextureImage* ImageElement::getImage() const
	{
		return image;
	}
	
	ImageElement::DisplayMode ImageElement::getDisplayMode() const
	{
		return displayMode;
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
}
