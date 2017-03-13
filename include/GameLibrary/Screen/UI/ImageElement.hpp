
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>

namespace fgl
{
	/*! Displays an image.*/
	class ImageElement : public ScreenElement
	{
	public:
		/*! The method of displaying the image within the element.*/
		typedef enum
		{
			/*! Stretch the image to fill the frame*/
			DISPLAY_STRETCH,
			/*! Scale the image to be contained within the frame, but maintain the aspect ratio of the image.*/
			DISPLAY_FIT_CENTER,
			/*! Scale the image to fill the frame, but don't stretch the image. If a part of the image is outside the frame, it is clipped.*/
			DISPLAY_FILL,
			/*! Repeat the image to fill the frame*/
			DISPLAY_REPEAT
		} DisplayMode;

		//! constant RectangleU value to match the source rect to the full size of the image
		static constexpr RectangleU DEFAULT_SRC_RECT = RectangleU(-1,-1,-1,-1);
		
		/*! default constructor; */
		ImageElement();
		/*! \copydoc fgl::ScreenElement::ScreenElement(const RectangleD&frame)*/
		explicit ImageElement(const RectangleD& frame);
		/*! Constructs a screen element with a frame and an image to display with a given display mode
			\param frame the frame (bounding box) of the element
			\param image the image to display inside the element
			\param displayMode the method to use for displaying the image; Default value is DISPLAY_STRETCH*/
		ImageElement(const RectangleD& frame, TextureImage* image, const DisplayMode& displayMode = DISPLAY_STRETCH);
		/*! Constructs a screen element with an image to display with a given display mode. The default frame is (0,0,0,0)
			\param image the image to display inside the element
			\param displayMode the method to use for displaying the image; Default value is DISPLAY_STRETCH*/
		ImageElement(TextureImage* image, const DisplayMode& displayMode = DISPLAY_STRETCH);
		
		
		/*! Sets the TextureImage for the element to display.
			\param image the TextureImage to display, or null to not display an image*/
		void setImage(TextureImage* image);
		/*! Sets the display method to display the TextureImage within the element.
			\param mode a constant that dictates how to display the image \see fgl::ImageElement::DisplayMode*/
		void setDisplayMode(const DisplayMode& mode);
		/*! Sets a specific source area of the image to be displayed. The srcRect is reset when a new image is set on the element.
			\param srcRect the section of the image to display*/
		void setImageSourceRect(const RectangleU& srcRect);
		
		
		/*! Gets the current TextureImage being displayed.
			\returns a TextureImage pointer, or null if no image is being displayed*/
		TextureImage* getImage() const;
		/*! Gets the current display method for the TextureImage within the element.
			\returns a ImageElement::DisplayMode constant*/
		DisplayMode getDisplayMode() const;
		/*! Gets the current section of the image that is set to be displayed, or the entire image if the source rectangle has not been set.
			\returns a rectangle representing the area of the image to be drawn */
		RectangleU getImageSourceRect() const;


		/*! Enables/disables horizontal image mirroring.
			\param toggle true to enable horizontal mirroring, and false to disable it */
		void setHorizontalMirroringEnabled(bool toggle);
		/*! Tells if horizontal image mirroring is enabled
			\returns true if enabled, or false if disabled */
		bool isHorizontalMirroringEnabled() const;
		/*! Enables/disables vertical image mirroring.
			\param toggle true to enable horizontal mirroring, and false to disable it */
		void setVerticalMirroringEnabled(bool toggle);
		/*! Tells if vertical image mirroring is enabled
			\returns true if enabled, or false if disabled */
		bool isVerticalMirroringEnabled() const;


		/*! Gets the frame where the image will be drawn.
			If the display mode is set to DISPLAY_REPEAT, this will give the frame where the first image is drawn.
			\returns a rectangle representing the area where the image is drawn. */
		RectangleD getImageDrawFrame() const;
		/*! Gets the frame where the image will actually be displayed. If the image is drawn outside the frame of the element, this frame will be clipped.
			If the display mode is set to DISPLAY_REPEAT, this will give the entire frame of the element. */
		RectangleD getImageDisplayFrame() const;
		
	protected:
		/*! \copydoc fgl::ScreenElement::drawMain(fgl::ApplicationData,fgl::Graphics)const*/
		virtual void drawMain(ApplicationData appData, Graphics graphics) const override;
		
	private:
		TextureImage* image;
		DisplayMode displayMode;
		RectangleU srcrect;
		bool mirrorHorizontally;
		bool mirrorVertically;
	};
}
