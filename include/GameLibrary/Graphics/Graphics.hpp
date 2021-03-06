
#pragma once

#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include <GameLibrary/Utilities/Geometry/Transform.hpp>
#include "Color.hpp"

namespace fgl
{
	/*! Handles drawing to a Window object*/
	class Graphics
	{
		friend class Application;
		friend class Font;
		friend class TextureImage;
		friend class Window;
	public:
		/*! copy constructor; When a Graphics object is copied, all transformations are cloned, and the new Graphics object can be transformed independently*/
		Graphics(const Graphics&);
		/*! virtual destructor*/
		virtual ~Graphics();
		/*! assignment operator*/
		Graphics& operator=(const Graphics&) = delete;
		
		
		/*! Gets the default Graphics font.
			\returns a Font pointer*/
		static Font*getDefaultFont();
		/*! Sets the path to load the default Font. This function should be called before an Application's initialize() is called. Otherwise, it will have no effect.*/
		static void setDefaultFontPath(const String&path);
		
		
		/*! Rotates the graphics by the given amount.
			\param degrees the amount, in degrees, to rotate the Graphics*/
		virtual void rotate(double degrees);
		/*! Rotates the graphics by the given amount at the given origin.
			\param degrees the amount, in degrees, to rotate the Graphics
			\param x the x coordinate of the origin of the rotation
			\param y the y coordinate of the origin of the rotation*/
		virtual void rotate(double degrees, double x, double y);
		/*! Rotates the graphics by the given amount at the given origin.
			\param degrees the amount, in degrees, to rotate the Graphics
			\param center the origin of the rotation*/
		virtual void rotate(double degrees, const Vector2d& center);
		

		/*! Scales the graphics by the given amount.
			\param scaleX the ratio to scale the graphics along the x axis
			\param scaleY the ratio to scale the graphics along the y axis*/
		virtual void scale(double scaleX, double scaleY);
		/*! Scales the graphics by the given amount.
			\param factors the ratio to scale the graphics along the x and y axis*/
		virtual void scale(const Vector2d& factors);
		/*! Scales the graphics by the given amount at the given origin.
			\param scaleX the ratio to scale the graphics along the x axis
			\param scaleY the ratio to scale the graphics along the y axis
			\param originX the x coordinate of the origin of the scaling
			\param originY the y coordinate of the origin of the scaling*/
		virtual void scale(double scaleX, double scaleY, double originX, double originY);
		/*! Scales the graphics by the given amount at the given origin.
			\param factors the ratio to scale the graphics along the x and y axis
			\param origin the origin of the scaling*/
		virtual void scale(const Vector2d& factors, const Vector2d& origin);
		
		
		/*! Translates the graphics by the given amount.
			\param x the amount, along the x axis, to translate
			\param y the amount, along the y axis, to translate*/
		virtual void translate(double x, double y);
		/*! Translates the graphics by the givem amount.
			\param delta the amount, along the x and y axis, to translate*/
		virtual void translate(const Vector2d&delta);
		
		
		/*! Gets the current transformation of the Graphics
			\returns a TransformD object*/
		virtual TransformD getTransform() const;
		
		
		/*! Sets the alpha (transparency) of the Graphics.
			\param alpha the alpha value, from 0.0 (fully transparent) to 1.0 (fully visible); If the value is outside these boundaries, it is adjusted*/
		virtual void setAlpha(float alpha);
		/*! Composites the alpha (transparency) of the Graphics. this->alpha *= alphaComposite;
			\param alphaComposite the alpha value to composite, from 0.0 (fully transparent) to 1.0 (fully visible); If the resulting alpha value is outside these boundaries, it is adjusted*/
		virtual void compositeAlpha(float alphaComposite);
		/*! Gets the current alpha value.
			\returns an unsigned byte value*/
		virtual float getAlpha() const;
		
		
		/*! Sets the Color that the graphics draws with.
			\param color the drawing color for the graphics*/
		virtual void setColor(const Color&color);
		/*! Composites the current drawing Color. \see Color::composite(const Color&)const
			\param comp the color to composite on the current drawing color*/
		virtual void compositeColor(const Color&comp);
		/*! Gets the current drawing color.
			\returns a const Color reference*/
		virtual const Color& getColor() const;
		
		
		/*! Sets the tinting Color of the graphics.
		This value is independent from Graphics::compositeColor(const Color&) and creates a composited Color on each draw operation.
			\param tint the Color to use to tint drawing operations*/
		virtual void setTintColor(const Color&tint);
		/*! Composites the current tinting Color. \see Color::composite(const Color&)const
			\param compTint the color to composite on the current tinting color*/
		virtual void compositeTintColor(const Color&compTint);
		/*! Gets the current tinting color.
			\returns a const Color reference*/
		virtual const Color& getTintColor() const;
		
		
		/*! Sets the font used to draw strings.
			\param font the font to use for string drawing*/
		virtual void setFont(Font*font);
		/*! Gets the font being used to draw strings.
			\returns a Font pointer*/
		virtual Font* getFont() const;
		
		
		/*! Sets the clipping region for draw operations.
			\param x the x coordinate of the clipping rectangle
			\param y the x coordinate of the clipping rectangle
			\param width the width of the clipping rectangle
			\param height the height of the clipping rectangle*/
		virtual void setClipRect(double x, double y, double width, double height);
		/*! Sets the clipping region for draw operations.
			\param rect the clipping rectangle*/
		virtual void setClipRect(const RectangleD&rect);
		/*! Clips the current clipping region further using another clipping region.
			\param rect the clipping rectangle*/
		virtual void clip(const RectangleD&rect);
		/*! Gets the current clipping rectangle.
			\returns a const RectangleD reference*/
		virtual const RectangleD& getClipRect();
		
		
		/*! Renders a string to the Window.
			\param text the string of text to draw
			\param x the x coordinate of the bottom left of the string
			\param y the y coordinate of the bottom left of the string*/
		void drawString(const String &text, double x, double y);
		/*! Renders a string to the Window.
			\param text the string of text to draw
			\param point the coordinates of the bottom left of the string*/
		void drawString(const String& text, const Vector2d& point);
		/*! Renders a string to the Window.
			\param text the string of text to draw
			\param x the x coordinate of the bottom left of the string
			\param y the y coordinate of the bottom left of the string*/
		void drawString(const WideString& text, double x, double y);
		/*! Renders a string to the Window.
			\param text the string of text to draw
			\param point the coordinates of the bottom left of the string*/
		void drawString(const WideString& text, const Vector2d& point);
		
		
		/*! Renders a line to the Window.
			\param x1 the x coordinate of the starting point of the line
			\param y1 the y coordinate of the starting point of the line
			\param x2 the x coordinate of the ending point of the line
			\param y2 the y coordinate of the ending point of the line*/
		void drawLine(double x1,double y1,double x2,double y2, double thickness=1.0);
		/*! Renders a line to the Window.
			\param point1 the coordinates of the starting point of the line
			\param point2 the coordinates of the ending point of the line*/
		void drawLine(const Vector2d& point1, const Vector2d& point2, double thickness=1.0);
		
		
		/*! Renders a rectangle outline to the Window.
			\param x the x coordinate of the rectangle
			\param y the y coordinate of the rectangle
			\param width the width of the rectangle
			\param height the height of the rectangle*/
		void drawRect(double x,double y,double width,double height);
		/*! Renders a rectangle outline to the Window.
			\param rect the rectangle to draw*/
		void drawRect(const RectangleD& rect);
		
		
		/*! Renders a filled rectangle to the Window.
			\param x the x coordinate of the rectangle
			\param y the y coordinate of the rectangle
			\param width the width of the rectangle
			\param height the height of the rectangle*/
		void fillRect(double x,double y,double width,double height);
		/*! Renders a filled rectangle to the Window.
			\param rect the rectangle to draw*/
		void fillRect(const RectangleD& rect);
		
		
		//void drawOval(double x1,double y1,double w,double h);
		//void drawOval(const RectangleD& rect);

		//void fillOval(double x1,double y1,double w,double h);
		//void fillOval(const RectangleD& rect);
		
		/*! Renders a polygon outline to the Window
			\param polygon the polygon to draw */
		void drawPolygon(const PolygonD& polygon);
		/*! Renders a filled polygon to the Window
			\param polygon the polygon to draw */
		void fillPolygon(const PolygonD& polygon);
		
		
		/*! Renders a TextureImage to the Window.
			\param img the TextureImage to draw
			\param x the x coordinate of the TextureImage
			\param y the y coordinate of the TextureImage*/
		void drawImage(TextureImage*img, double x, double y);
		/*! Renders a TextureImage to the Window.
			\param img the TextureImage to draw
			\param point the (x,y) coordinate to draw the TextureImage*/
		void drawImage(TextureImage*img, const Vector2d& point);
		/*! Renders a TextureImage to the Window.
			\param img the TextureImage to draw
			\param x the x coordinate of the TextureImage
			\param y the y coordinate of the TextureImage*/
		void drawImage(TextureImage*img, double x, double y, double width, double height);
		/*! Renders a TextureImage to the Window.
			\param img the TextureImage to draw
			\param rect the destination rectangle to draw the TextureImage*/
		void drawImage(TextureImage*img, const RectangleD& rect);
		/*! Renders a TextureImage to the Window.
			\param img the TextureImage to draw
			\param dx1 the x coordinate of the top left of the destination rectangle to draw the TextureImage
			\param dy1 the y coordinate of the top left of the destination rectangle to draw the TextureImage
			\param dx2 the x coordinate of the bottom right of the destination rectangle to draw the TextureImage
			\param dy2 the y coordinate of the bottom right of the destination rectangle to draw the TextureImage
			\param sx1 the x coordinate of the top left of the source rectangle inside the TextureImage
			\param sy1 the y coordinate of the top left of the source rectangle inside the TextureImage
			\param sx2 the x coordinate of the bottom right of the source rectangle inside the TextureImage
			\param sy2 the y coordinate of the bottom right of the source rectangle inside the TextureImage*/
		void drawImage(TextureImage*img, double dx1, double dy1, double dx2, double dy2, unsigned int sx1, unsigned int sy1, unsigned int sx2, unsigned int sy2);
		/*! Renders a TextureImage to the Window.
			\param img the TextureImage to draw
			\param dst the destination rectangle to draw the TextureImage
			\param src the source rectangle inside the TextureImage*/
		void drawImage(TextureImage*img, const RectangleD& dst, const RectangleU& src);
		
	protected:
		/*! Clears the Window render and resets all the properties of the Graphics*/
		void reset(const Color&clearColor = Colors::WHITE);
		
		/*! Constructs a Graphics object to draw to a given Window*/
		explicit Graphics(Window& window);
		
		/*! Called internally to begin a drawing operation.
		To create a custom drawing operation, beginDraw() should be called before the operation, and endDraw() should be called when the operation finishes.*/
		void beginDraw();
		
		/*! Called internally to end a drawing operation.
		To create a custom drawing operation, beginDraw() should be called before the operation, and endDraw() should be called when the operation finishes.*/
		void endDraw();

		/*! Returns the renderer pointer used to draw to the window
			\returns a void pointer that must be cast to an SDL_Renderer pointer */
		void* getRenderer() const;
		
		//! Draws a line without calling beginDraw or endDraw or transforming coordinates
		void drawLineRaw(double x1, double y1, double x2, double y2, double width);
		//! Gets the line width based on the transform
		double getTransformedLineWidth(double x1, double y1, double x2, double y2) const;
		//! Draws an SDL_Texture without calling beginDraw or endDraw or transforming coordinates
		void drawTextureRaw(void* texture, double dx1, double dy1, double dx2, double dy2, unsigned int sx1, unsigned int sy1, unsigned int sx2, unsigned int sy2, double rotation, const Color& colormod);
		//! Draws an image without calling beginDraw or endDraw or transforming coordinates
		void drawImageRaw(TextureImage* img, double dx1, double dy1, double dx2, double dy2, unsigned int sx1, unsigned int sy1, unsigned int sx2, unsigned int sy2, double rotation, const Color& colormod);

	private:
		Window*window;
		void*renderer;
		void*renderTarget;
		unsigned int renderTarget_width;
		unsigned int renderTarget_height;

		Color color;
		Color tintColor;
		float alpha;
		
		Font*font;
		static Font*defaultFont;
		static String defaultFontPath;

		TextureImage*pixel;
		
		RectangleD cliprect;
		Vector2d clipoffset;

		double rotation;
		Vector2d scaling;
		
		TransformD transform;

		bool derived;
	};
}
