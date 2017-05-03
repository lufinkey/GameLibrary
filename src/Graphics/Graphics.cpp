
#include <GameLibrary/Graphics/Graphics.hpp>
#include <GameLibrary/Graphics/TextureImage.hpp>
#include <GameLibrary/IO/Console.hpp>
#include <GameLibrary/Utilities/Math.hpp>
#include <GameLibrary/Utilities/PlatformChecks.hpp>
#include <GameLibrary/Utilities/Font/Font.hpp>
#include <GameLibrary/Window/Viewport.hpp>
#include <GameLibrary/Window/Window.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#define WIN32_LEAN_AND_MEAN
#if defined(TARGETPLATFORM_WINDOWS)
	#include <Windows.h>
#endif

namespace fgl
{
	/*typedef struct
	{
		SDL_GLContext context;
	} GameLibrary_ContextData;*/

	fgl::String Graphics_getDefaultFontPath()
	{
#if defined(TARGETPLATFORM_WINDOWS)
		char windows_path[MAX_PATH];
		GetWindowsDirectoryA(windows_path, MAX_PATH);
		fgl::String fontPath((char*)windows_path);
		fontPath += "\\Fonts\\arial.ttf";
		return fontPath;
#elif defined(TARGETPLATFORM_MAC) || defined(TARGETPLATFORM_IOS_SIMULATOR)
		return "/Library/Fonts/Arial.ttf";
#else
		return "fonts/arial.ttf";
#endif
	}
	
	Font* Graphics::defaultFont = nullptr;
	String Graphics::defaultFontPath = Graphics_getDefaultFontPath();
	
	void Graphics::reset(const Color&clearColor)
	{
		setColor(Color::BLACK);
		setTintColor(Color::WHITE);
		setAlpha(1.0f);
		
		setFont(defaultFont);
		
		transform.reset();
		rotation = 0;
		scaling.x = 1;
		scaling.y = 1;
		
		clipoffset.x = 0;
		clipoffset.y = 0;
		
		SDL_RenderSetViewport((SDL_Renderer*)renderer, nullptr);
		
		if(window->viewport != nullptr && window->viewport->matchWindow)
		{
			const Vector2u& winSz = window->getSize();
			window->viewport->setSize((double)winSz.x, (double)winSz.y);
		}
		
		if(window->viewport != nullptr && window->viewport->maintainResolution)
		{
			if((renderTarget_width!=(unsigned int)window->viewport->size.x) || (renderTarget_height!=(unsigned int)window->viewport->size.y))
			{
				if(renderTarget!=nullptr)
				{
					SDL_DestroyTexture((SDL_Texture*)renderTarget);
					renderTarget = nullptr;
				}
				renderTarget_width = (unsigned int)window->viewport->size.x;
				renderTarget_height = (unsigned int)window->viewport->size.y;
				renderTarget = SDL_CreateTexture((SDL_Renderer*)renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)renderTarget_width, (int)renderTarget_height);
				if(renderTarget==nullptr)
				{
					renderTarget_width = 0;
					renderTarget_height = 0;
					Console::writeErrorLine("Unable to create texture for render target");
				}
				else
				{
					SDL_SetRenderTarget((SDL_Renderer*)renderer, (SDL_Texture*)renderTarget);
				}
			}
			else
			{
				SDL_SetRenderTarget((SDL_Renderer*)renderer, (SDL_Texture*)renderTarget);
			}
		}
		else
		{
			if(renderTarget!=nullptr)
			{
				renderTarget_width = 0;
				renderTarget_height = 0;
				SDL_SetRenderTarget((SDL_Renderer*)renderer, nullptr);
				SDL_DestroyTexture((SDL_Texture*)renderTarget);
			}
		}
		
		SDL_SetRenderDrawColor((SDL_Renderer*)renderer, clearColor.r,clearColor.g,clearColor.b,clearColor.a);
		SDL_RenderClear((SDL_Renderer*)renderer);
		SDL_SetRenderDrawColor((SDL_Renderer*)renderer, 0,0,0,255);
		
		double zoom = 1;
		if(window->viewport!=nullptr)
		{
			zoom = window->viewport->zoom;
		}
		
		if(window->viewport!=nullptr && window->viewport->maintainResolution && renderTarget!=nullptr)
		{
			setClipRect(0, 0, renderTarget_width, renderTarget_height);
			Vector2d winSize((double)renderTarget_width, (double)renderTarget_height);
			double difX = (winSize.x - (winSize.x*zoom))/(2*zoom);
			double difY = (winSize.y - (winSize.y*zoom))/(2*zoom);
			scale(zoom, zoom);
			translate(difY, difX);
		}
		else if(window->viewport == nullptr || window->viewport->matchWindow)
		{
			const Vector2u& winSz = window->getSize();
			Vector2d winSize = Vector2d((double)winSz.x, (double)winSz.y);
			if(window->viewport != nullptr)
			{
				window->viewport->setSize((double)winSz.x, (double)winSz.y);
			}
			
			setClipRect(0, 0, (double)winSz.x, (double)winSz.y);
			
			double difX = (winSize.x - (winSize.x*zoom))/(2*zoom);
			double difY = (winSize.y - (winSize.y*zoom))/(2*zoom);
			
			scale(zoom,zoom);
			translate(difY, difX);
		}
		else if(window->viewport->letterboxed)
		{
			double multScale = 1;
			Vector2u winSz = window->getSize();
			Vector2d winSize = Vector2d((double)winSz.x, (double)winSz.y);
			Vector2d viewSize = window->viewport->getSize();
			setClipRect(0, 0, (double)winSz.x, (double)winSz.y);
			
			double ratX = winSize.x /viewSize.x;
			double ratY = winSize.y /viewSize.y;
			if(ratX<ratY)
			{
				multScale = ratX;
			}
			else
			{
				multScale = ratY;
			}
			
			double fixedWidth = viewSize.x*multScale;
			double fixedHeight = viewSize.y*multScale;
			
			double difX = ((winSize.x - (winSize.x*zoom))+(winSize.x - fixedWidth))/(2*zoom*multScale);
			double difY = ((winSize.y - (winSize.y*zoom))+(winSize.y - fixedHeight))/(2*zoom*multScale);
			
			double letterBoxW = (winSize.x - fixedWidth)/2;
			double letterBoxH = (winSize.y - fixedHeight)/2;
			
			setColor(Color::BLACK);
			if(letterBoxW>0)
			{
				fillRect(0,0,letterBoxW,winSize.y);
				fillRect(letterBoxW+fixedWidth,0,letterBoxW,winSize.y);
			}
			if(letterBoxH>0)
			{
				fillRect(0,0,winSize.x,letterBoxH);
				fillRect(0,letterBoxH+fixedHeight,winSize.x,letterBoxH);
			}
			clipoffset.x = letterBoxW;
			clipoffset.y = letterBoxH;
			setClipRect(0,0,fixedWidth, fixedHeight);

			double scaleVal = zoom*multScale;

			scale(scaleVal,scaleVal);
			translate(difX, difY);
		}
		else
		{
			Vector2u winSz = window->getSize();
			Vector2d winSize = Vector2d((double)winSz.x, (double)winSz.y);
			Vector2d viewSize = window->viewport->getSize();
			setClipRect(0, 0, (double)winSz.x, (double)winSz.y);
			
			double ratX = winSize.x /viewSize.x;
			double ratY = winSize.y /viewSize.y;

			double difX = (winSize.x - (winSize.x*zoom))/(2*zoom*ratX);
			double difY = (winSize.y - (winSize.y*zoom))/(2*zoom*ratY);

			scale(zoom*ratX, zoom*ratY);
			translate(difX,difY);
		}
	}
	
	Graphics::Graphics(Window& win)
	{
		if(win.windowdata == nullptr)
		{
			//TODO replace with more specific exception type
			throw Exception("Cannot create Graphics object for window that is not created");
		}
		window = &win;
		renderTarget = nullptr;
		renderTarget_width = 0;
		renderTarget_height = 0;
		renderer = (void*)SDL_CreateRenderer((SDL_Window*)win.windowdata,-1,SDL_RENDERER_ACCELERATED);
		if(renderer==nullptr)
		{
			//TODO replace with more specific exception type
			throw Exception(SDL_GetError());
		}
		
		color = Color::BLACK;
		tintColor = Color::WHITE;
		alpha = 1.0f;
		
		if(defaultFont == nullptr)
		{
			defaultFont = new Font();
			String error;
			bool success = defaultFont->loadFromPath(defaultFontPath, &error);
			if(!success)
			{
				Console::writeLine("\""+defaultFontPath+"\": "+error);
			}
			defaultFont->setAntialiasing(true);
		}
		font = defaultFont;
		
		if(win.viewport == nullptr || win.viewport->matchWindow)
		{
			const Vector2u& winSize = win.getSize();
			cliprect = RectangleD(0, 0, (double)winSize.x, (double)winSize.y);
		}
		clipoffset = Vector2d(0,0);
		
		transform = TransformD();
		rotation = 0;
		scaling.x = 1;
		scaling.y = 1;
		
		derived = false;
		
		pixel = new TextureImage();
		pixel->create(1,1,*this);
		Color pixelColor = Color::WHITE;
		pixel->update(&pixelColor);
		
		reset();
	}
	
	Graphics::Graphics(const Graphics&g)
		: window(g.window),
		renderer(g.renderer),
		renderTarget(g.renderTarget),
		renderTarget_width(g.renderTarget_width),
		renderTarget_height(g.renderTarget_height),
		color(g.color),
		tintColor(g.tintColor),
		alpha(g.alpha),
		font(g.font),
		pixel(g.pixel),
		cliprect(g.cliprect),
		clipoffset(g.clipoffset),
		rotation(g.rotation),
		scaling(g.scaling),
		transform(g.transform),
		derived(true)
	{
		//
	}
	
	Graphics::~Graphics()
	{
		if(!derived)
		{
			delete font;
			delete pixel;
			if(renderTarget!=nullptr)
			{
				SDL_SetRenderTarget((SDL_Renderer*)renderer, nullptr);
				SDL_DestroyTexture((SDL_Texture*)renderTarget);
			}
			SDL_DestroyRenderer((SDL_Renderer*)renderer);
		}
	}

	void Graphics::beginDraw()
	{
		double clipLeft = (clipoffset.x + cliprect.x);
		double clipTop = (clipoffset.y + cliprect.y);
		double clipRight = clipLeft + cliprect.width;
		double clipBottom = clipTop + cliprect.height;
		
		SDL_Rect clip;
		clip.x = (int)clipLeft;
		clip.y = (int)clipTop;
		clip.w = (int)(clipRight - (double)clip.x);
		clip.h = (int)(clipBottom - (double)clip.y);
		
		Color colorComp = color.composite(tintColor);
		byte newAlpha = (byte)(colorComp.a * alpha);
		
		SDL_RenderSetClipRect((SDL_Renderer*)renderer, &clip);
		SDL_SetRenderDrawColor((SDL_Renderer*)renderer, colorComp.r, colorComp.g, colorComp.b, newAlpha);
		
		//SDL_GLContext context = SDL_RendererGetGLContext((SDL_Renderer*)renderer);
		//SDL_GL_MakeCurrent((SDL_Window*)window->windowdata, context);
		//glPushMatrix();
		//glRotatef(1.0f,1.0f,1.0f,1.0f);
	}

	void Graphics::endDraw()
	{
		//SDL_GLContext context = SDL_RendererGetGLContext((SDL_Renderer*)renderer);
		//SDL_GL_MakeCurrent((SDL_Window*)window->windowdata, context);
		//glPopMatrix();
		
		SDL_SetRenderDrawColor((SDL_Renderer*)renderer, 0,0,0,255);
		SDL_RenderSetClipRect((SDL_Renderer*)renderer, nullptr);
	}

	void* Graphics::getRenderer() const
	{
		return renderer;
	}

	Font* Graphics::getDefaultFont()
	{
		if(defaultFont == nullptr)
		{
			defaultFont = new Font();
			String error;
			bool success = defaultFont->loadFromPath(defaultFontPath, &error);
			if(!success)
			{
				Console::writeLine(error);
			}
			defaultFont->setAntialiasing(true);
		}
		return defaultFont;
	}
	
	void Graphics::setDefaultFontPath(const String&path)
	{
		defaultFontPath = path;
	}
	
	void Graphics::rotate(double degrees)
	{
		transform.rotate(degrees);
		rotation += degrees;
	}

	void Graphics::rotate(double degrees, double x, double y)
	{
		transform.rotate(degrees,x,y);
		rotation += degrees;
	}

	void Graphics::rotate(double degrees, const Vector2d&center)
	{
		rotate(degrees, center.x, center.y);
	}
	
	void Graphics::scale(double scaleX, double scaleY)
	{
		transform.scale(scaleX,scaleY);
		scaling.x *= scaleX;
		scaling.y *= scaleY;
	}
	
	void Graphics::scale(const Vector2d& factors)
	{
		scale(factors.x, factors.y);
	}
	
	void Graphics::scale(double scaleX, double scaleY, double originX, double originY)
	{
		transform.scale(scaleX,scaleY, originX, originY);
		scaling.x *= scaleX;
		scaling.y *= scaleY;
	}

	void Graphics::scale(const Vector2d& factors, const Vector2d& origin)
	{
		scale(factors.x, factors.y, origin.x, origin.y);
	}
		
	void Graphics::translate(double x, double y)
	{
		transform.translate(x, y);
	}

	void Graphics::translate(const Vector2d&delta)
	{
		translate(delta.x, delta.y);
	}
	
	TransformD Graphics::getTransform() const
	{
		return transform;
	}
	
	void Graphics::setAlpha(float a)
	{
		if(alpha > 1.0f)
		{
			alpha = 1.0f;
		}
		else if(alpha < 0.0f)
		{
			alpha = 0.0f;
		}
		alpha = a;
	}

	void Graphics::compositeAlpha(float aComposite)
	{
		alpha = alpha*aComposite;
		if(alpha > 1.0f)
		{
			alpha = 1.0f;
		}
		else if(alpha < 0.0f)
		{
			alpha = 0.0f;
		}
	}

	float Graphics::getAlpha() const
	{
		return alpha;
	}
	
	void Graphics::setColor(const Color& c)
	{
		color = c;
	}

	void Graphics::compositeColor(const Color& c)
	{
		color = color.composite(c);
	}

	const Color& Graphics::getColor() const
	{
		return color;
	}
	
	void Graphics::setTintColor(const Color& c)
	{
		tintColor = c;
	}

	void Graphics::compositeTintColor(const Color& c)
	{
		tintColor = tintColor.composite(c);
	}

	const Color& Graphics::getTintColor() const
	{
		return tintColor;
	}

	void Graphics::setFont(Font*f)
	{
		font = f;
	}

	Font* Graphics::getFont() const
	{
		return font;
	}

	void Graphics::setClipRect(double x, double y, double width, double height)
	{
		setClipRect(RectangleD(x,y,width,height));
	}
	
	void Graphics::setClipRect(const RectangleD&cr)
	{
		cliprect = cr;
	}
	
	void Graphics::clip(const RectangleD& cr)
	{
		RectangleD trueCR = transform.transform(cr);
		trueCR.x -= clipoffset.x;
		trueCR.y -= clipoffset.y;
		cliprect = cliprect.getIntersect(trueCR);
	}
	
	const RectangleD& Graphics::getClipRect()
	{
		return cliprect;
	}
	
	void Graphics::drawString(const String& text, double x1, double y1)
	{
		drawString((WideString)text, x1, y1);
	}
	
	void Graphics::drawString(const String&text, const Vector2d& point)
	{
		drawString((WideString)text, point.x, point.y);
	}
	
	void Graphics::drawString(const WideString& text, double x1, double y1)
	{
		unsigned int fontSize = font->getSize();
		unsigned int renderedFontSize = fontSize;
		if(window->getViewport()==nullptr || !window->getViewport()->maintainResolution)
		{
			renderedFontSize = (unsigned int)Math::abs(scaling.y*(double)fontSize);
		}
		ArrayList<RenderedGlyphContainer::RenderedGlyph> glyphs = font->getRenderedGlyphs((Font::GlyphString)text,renderer, renderedFontSize, font->getStyle());
		Vector2u dimensions = font->measureString(text);
		font->setSize(renderedFontSize);
		Vector2u realDimensions = font->measureString(text);
		Color compColor = color.composite(tintColor);

		bool negWidth = (scaling.x < 0);
		bool negHeight = (scaling.y < 0);

		beginDraw();
		
		double y1_top = y1 - (double)dimensions.y;
		double x_offset = 0;
		double scaleRatio = Math::abs(scaling.x/scaling.y);
		double dimensionRatio = (double)dimensions.x/(double)realDimensions.x;
		for(size_t i = 0; i < glyphs.size(); i++)
		{
			RenderedGlyphContainer::RenderedGlyph& glyph = glyphs.get(i);
			SDL_Texture*texture = (SDL_Texture*)glyph.texture;
			Vector2u glyphDimensions = font->measureString((Font::GlyphString)text.charAt(i));
			unsigned int format = 0;
			int access = 0;
			int w = 0;
			int h = 0;
			SDL_QueryTexture(texture, &format, &access, &w, &h);
			double realWidth = (double)w*scaleRatio;
			double realHeight = (double)h;

			if(negWidth)
			{
				realWidth = -realWidth;
			}
			if(negHeight)
			{
				realHeight = -realHeight;
			}
			
			Vector2d pnt = transform.transform(Vector2d(x1 + x_offset, y1_top));

			drawTextureRaw(glyph.texture, pnt.x, pnt.y, pnt.x+realWidth, pnt.y+realHeight, 0, 0, (unsigned int)w, (unsigned int)h, rotation, compColor);
			
			x_offset += (double)glyphDimensions.x*dimensionRatio;
		}

		endDraw();

		font->setSize(fontSize);
	}
	
	void Graphics::drawString(const WideString&text, const Vector2d& point)
	{
		drawString(text, point.x, point.y);
	}

	void Graphics::drawLineRaw(double x1, double y1, double x2, double y2, double width)
	{
		if(width==1.0)
		{
			SDL_RenderDrawLine((SDL_Renderer*)renderer, (int)x1, (int)y1, (int)x2, (int)y2);
		}
		else
		{
			RectangleD dstRect;
			double degrees = 0;

			double widthOffset = 0;
			if(width >= 2.0)
			{
				widthOffset = width/2;
			}

			if(x1==x2 || y1==y2)
			{
				if(x1 > x2)
				{
					double tmpX = x1;
					x1 = x2;
					x2 = tmpX;
				}
				else if(y1 > y2)
				{
					double tmpY = y1;
					y1 = y2;
					y2 = tmpY;
				}

				if(x1 == x2)
				{
					dstRect.x = x1-widthOffset;
					dstRect.y = y1;
					dstRect.width = width;
					dstRect.height = y2-y1;
				}
				else if(y1 == y2)
				{
					dstRect.x = x1;
					dstRect.y = y1-widthOffset;
					dstRect.width = x2-x1;
					dstRect.height = width;
				}
				else
				{
					//this would literally never happen, but lets put some code here anyway
					Console::writeErrorLine("congrats. your processor is fucked");
					dstRect.x = 0;
					dstRect.y = 0;
					dstRect.width = 0;
					dstRect.height = 0;
				}
			}
			else
			{
				degrees = -Math::radtodeg(Math::atan2(x2-x1, y2-y1))+90;
				double dist = Math::distance(x1, y1, x2, y2);

				dstRect.x = x1;
				dstRect.y = y1-widthOffset;
				dstRect.width = dist;
				dstRect.height = width;
			}

			SDL_Point origin;
			origin.x = 0;
			origin.y = 0;
			if(degrees != 0)
			{
				origin.y = (int)widthOffset;
			}

			Uint8 r = 0;
			Uint8 g = 0;
			Uint8 b = 0;
			Uint8 a = 0;
			SDL_GetRenderDrawColor((SDL_Renderer*)renderer, &r, &g, &b, &a);

			SDL_Texture* texture = (SDL_Texture*)pixel->texture;

			SDL_Rect intDstRect;
			intDstRect.x = (int)dstRect.x;
			intDstRect.y = (int)dstRect.y;
			intDstRect.w = (int)((dstRect.x+dstRect.width) - (double)intDstRect.x);
			intDstRect.h = (int)((dstRect.y+dstRect.height) - (double)intDstRect.y);

			SDL_Rect srcRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = 1;
			srcRect.h = 1;

			SDL_SetTextureColorMod(texture, r, g, b);
			SDL_SetTextureAlphaMod(texture, a);

			SDL_RenderCopyEx((SDL_Renderer*)renderer, texture, &srcRect, &intDstRect, degrees, &origin, SDL_FLIP_NONE);

			SDL_SetTextureColorMod(texture, 255, 255, 255);
			SDL_SetTextureAlphaMod(texture, 255);
		}
	}

	double Graphics::getTransformedLineWidth(double x1, double y1, double x2, double y2) const
	{
		double lineWidth = 1;
		double xDist = Math::abs(x2-x1);
		double yDist = Math::abs(y2-y1);
		if(xDist > yDist)
		{
			lineWidth = scaling.y;
		}
		else if(yDist > xDist)
		{
			lineWidth = scaling.x;
		}
		else
		{
			lineWidth = (scaling.x+scaling.y)/2;
		}
		return lineWidth;
	}
	
	void Graphics::drawLine(double x1, double y1, double x2, double y2, double thickness)
	{
		double lineWidth = getTransformedLineWidth(x1, x2, y1, y2)*thickness;
		Vector2d pnt1 = transform.transform(Vector2d(x1, y1));
		Vector2d pnt2 = transform.transform(Vector2d(x2, y2));
		
		beginDraw();

		drawLineRaw(pnt1.x, pnt1.y, pnt2.x, pnt2.y, lineWidth);
		
		endDraw();
	}
	
	void Graphics::drawLine(const Vector2d& point1, const Vector2d& point2, double thickness)
	{
		drawLine(point1.x, point1.y, point2.x, point2.y, thickness);
	}

	void Graphics::drawRect(double x, double y, double width, double height)
	{
		if(width==0 || height==0)
		{
			return;
		}
		double right = x+width;
		double bottom = y+height;

		double fullwidth = width*scaling.x;
		double fullheight = height*scaling.y;

		Vector2d topleft = transform.transform(Vector2d(x, y));
		Vector2d topright = transform.transform(Vector2d(right, y));
		Vector2d bottomleft = transform.transform(Vector2d(x, bottom));

		beginDraw();

		Uint8 r = 0;
		Uint8 g = 0;
		Uint8 b = 0;
		Uint8 a = 0;
		SDL_GetRenderDrawColor((SDL_Renderer*)renderer, &r, &g, &b, &a);
		Color color(r, g, b, a);

		drawImageRaw(pixel, topleft.x, topleft.y, topleft.x+fullwidth, topleft.y+scaling.y, 0, 0, 1, 1, rotation, color);
		drawImageRaw(pixel, topright.x, topright.y, topright.x-scaling.x, topright.y+fullheight, 0, 0, 1, 1, -rotation, color);
		drawImageRaw(pixel, topleft.x, topleft.y, topleft.x+scaling.x, topleft.y+fullheight, 0, 0, 1, 1, rotation, color);
		drawImageRaw(pixel, bottomleft.x, bottomleft.y, bottomleft.x+fullwidth, bottomleft.y-scaling.y, 0, 0, 1, 1, -rotation, color);
		
		endDraw();
	}

	void Graphics::drawRect(const RectangleD& rect)
	{
		drawRect(rect.x, rect.y, rect.width, rect.height);
	}

	void Graphics::fillRect(double x, double y, double width, double height)
	{
		Vector2d pnt = transform.transform(Vector2d(x, y));
		
		beginDraw();

		Uint8 r = 0;
		Uint8 g = 0;
		Uint8 b = 0;
		Uint8 a = 0;
		SDL_GetRenderDrawColor((SDL_Renderer*)renderer, &r, &g, &b, &a);

		drawImageRaw(pixel, pnt.x, pnt.y, pnt.x+(width*scaling.x), pnt.y+(height*scaling.y), 0, 0, 1, 1, rotation, Color(r, g, b, a));
		
		endDraw();
	}

	void Graphics::fillRect(const RectangleD& rect)
	{
		fillRect(rect.x, rect.y, rect.width, rect.height);
	}

	/*void Graphics::drawOval(double x1,double y1,double w,double h)
	{
		//
	}

	void Graphics::drawOval(const RectangleD& rect)
	{
		drawOval(rect.x, rect.y, rect.width, rect.height);
	}

	void Graphics::fillOval(double x1,double y1,double w,double h)
	{
		//
	}

	void Graphics::fillOval(const RectangleD& rect)
	{
		fillOval(rect.x, rect.y, rect.width, rect.height);
	}*/
	
	void Graphics::drawPolygon(const PolygonD& polygon)
	{
		if(polygon.getPoints().size() > 0)
		{
			const ArrayList<Vector2d>& origPoints = polygon.getPoints();
			PolygonD transformedPolygon = transform.transform(polygon);
			const ArrayList<Vector2d>& points = transformedPolygon.getPoints();

			beginDraw();
			
			if(points.size() == 1)
			{
				const Vector2d& point = points.get(0);
				SDL_RenderDrawPoint((SDL_Renderer*)renderer, (int)point.x, (int)point.y);
			}
			else
			{
				for(unsigned int i=0; i<points.size(); i++)
				{
					const Vector2d& point = points.get(i);
					const Vector2d& origPoint = origPoints.get(i);
					if(i == (points.size()-1))
					{
						const Vector2d& origStartPoint = origPoints.get(0);
						const Vector2d& startPoint = points.get(0);
						double lineWidth = getTransformedLineWidth(origPoint.x, origPoint.y, origStartPoint.x, origStartPoint.y);
						drawLineRaw(point.x, point.y, startPoint.x, startPoint.y, lineWidth);
					}
					else
					{
						const Vector2d& origNextPoint = origPoints.get(i+1);
						const Vector2d& nextPoint = points.get(i+1);
						double lineWidth = getTransformedLineWidth(origPoint.x, origPoint.y, origNextPoint.x, origNextPoint.y);
						drawLineRaw(point.x, point.y, nextPoint.x, nextPoint.y, lineWidth);
					}
				}
			}
			
			endDraw();
		}
	}
	
	void Graphics::fillPolygon(const PolygonD& polygon)
	{
		if(polygon.getPoints().size() > 0)
		{
			PolygonD transformedPolygon = transform.transform(polygon);
			const ArrayList<Vector2d>& points = transformedPolygon.getPoints();
			Sint16* polygonX = new Sint16[points.size()];
			Sint16* polygonY = new Sint16[points.size()];
			for(size_t i=0; i<points.size(); i++)
			{
				polygonX[i] = (Sint16)points[i].x;
				polygonY[i] = (Sint16)points[i].y;
			}
			
			beginDraw();
			
			Uint8 r = 0;
			Uint8 g = 0;
			Uint8 b = 0;
			Uint8 a = 0;
			SDL_GetRenderDrawColor((SDL_Renderer*)renderer, &r, &g, &b, &a);
			filledPolygonRGBA((SDL_Renderer*)renderer, polygonX, polygonY, (int)points.size(), r, g, b, a);
			
			endDraw();
		}
	}

	void Graphics::drawTextureRaw(void* texture, double dx1, double dy1, double dx2, double dy2, unsigned int sx1, unsigned int sy1, unsigned int sx2, unsigned int sy2, double rotation, const Color& colormod)
	{
		bool flipHort = false;
		bool flipVert = false;
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if(dx2 < dx1)
		{
			double tmp = dx1;
			dx1 = dx2;
			dx2 = tmp;
			//really SDL...? I really have to force cast this bitwise operation?
			flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
			flipHort = true;
		}
		if(dy2 < dy1)
		{
			double tmp = dy1;
			dy1 = dy2;
			dy2 = tmp;
			//...absolutely brilliant
			flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
			flipVert = true;
		}

		SDL_Rect dstrect;
		dstrect.x = (int)dx1;
		dstrect.y = (int)dy1;
		dstrect.w = (int)(dx2 - (double)dstrect.x);
		dstrect.h = (int)(dy2 - (double)dstrect.y);

		SDL_Point center;
		if(flipHort)
		{
			center.x = dstrect.w;
			rotation = -rotation;
		}
		else
		{
			center.x = 0;
		}
		if(flipVert)
		{
			center.y = dstrect.h;
			rotation = -rotation;
		}
		else
		{
			center.y = 0;
		}

		SDL_Rect srcrect;
		srcrect.x = (int)sx1;
		srcrect.y = (int)sy1;
		srcrect.w = (int)(sx2 - sx1);
		srcrect.h = (int)(sy2 - sy1);

		SDL_SetTextureColorMod((SDL_Texture*)texture, colormod.r, colormod.g, colormod.b);
		SDL_SetTextureAlphaMod((SDL_Texture*)texture, colormod.a);

		SDL_RenderCopyEx((SDL_Renderer*)renderer, (SDL_Texture*)texture, &srcrect, &dstrect, rotation, &center, flip);

		SDL_SetTextureColorMod((SDL_Texture*)texture, 255, 255, 255);
		SDL_SetTextureAlphaMod((SDL_Texture*)texture, 255);
	}

	void Graphics::drawImageRaw(TextureImage* img, double dx1, double dy1, double dx2, double dy2, unsigned int sx1, unsigned int sy1, unsigned int sx2, unsigned int sy2, double rotation, const Color& colormod)
	{
		void* texture = img->texture;
		if(texture!=nullptr)
		{
			drawTextureRaw(texture, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, rotation, colormod);
		}
	}
	
	void Graphics::drawImage(TextureImage*img, double x, double y)
	{
		if(img==nullptr)
		{
			throw IllegalArgumentException("img", "cannot be null");
		}
		//SDL_Texture*texture = (SDL_Texture*)img->texture;
		unsigned int texWidth = img->getWidth();
		unsigned int texHeight = img->getHeight();

		double dx1 = x;
		double dy1 = y;
		double dx2 = x+((double)texWidth);
		double dy2 = y+((double)texHeight);
		drawImage(img, dx1,dy1,dx2,dy2, 0,0,texWidth,texHeight);
	}

	void Graphics::drawImage(TextureImage*img, const Vector2d& point)
	{
		drawImage(img, point.x, point.y);
	}

	void Graphics::drawImage(TextureImage*img, double x, double y, double width, double height)
	{
		if(img==nullptr)
		{
			throw IllegalArgumentException("img", "cannot be null");
		}
		//SDL_Texture*texture = (SDL_Texture*)img->texture;
		unsigned int texWidth = img->width;
		unsigned int texHeight = img->height;
		
		double dx1 = x;
		double dy1 = y;
		double dx2 = x+width;
		double dy2 = y+height;
		drawImage(img, dx1,dy1,dx2,dy2, 0,0,texWidth,texHeight);
	}

	void Graphics::drawImage(TextureImage*img, const RectangleD& rect)
	{
		drawImage(img, rect.x, rect.y, rect.width, rect.height);
	}
	
	void Graphics::drawImage(TextureImage* img, double dx1, double dy1, double dx2, double dy2, unsigned int sx1, unsigned int sy1, unsigned int sx2, unsigned int sy2)
	{
		if(img==nullptr)
		{
			throw IllegalArgumentException("img", "cannot be null");
		}
		unsigned int texWidth = img->width;
		unsigned int texHeight = img->height;
		if(texWidth != 0 && texHeight != 0)
		{
			Vector2d pnt1 = transform.transform(Vector2d(dx1, dy1));

			double realDx2 = pnt1.x + ((dx2-dx1)*scaling.x);
			double realDy2 = pnt1.y + ((dy2-dy1)*scaling.y);
			
			RectangleD dstrectBox = RectangleD(pnt1.x, pnt1.y, realDx2-pnt1.x, realDy2-pnt1.y);
			if(rotation!=0)
			{
				dstrectBox = RectangleD(dx1, dy1, dx2-dx1, dy2-dy1);
			}
			if(dstrectBox.width < 0)
			{
				dstrectBox.width = -dstrectBox.width;
				dstrectBox.x = dstrectBox.x - dstrectBox.width;
			}
			if(dstrectBox.height < 0)
			{
				dstrectBox.height = -dstrectBox.height;
				dstrectBox.y = dstrectBox.y - dstrectBox.height;
			}
			if(rotation!=0)
			{
				dstrectBox = transform.transform(dstrectBox);
			}
			RectangleD cliprectBox(clipoffset.x+cliprect.x, clipoffset.y+cliprect.y, cliprect.width, cliprect.height);
			if(!cliprectBox.intersects(dstrectBox))
			{
				return;
			}
			
			beginDraw();

			drawImageRaw(img, pnt1.x, pnt1.y, realDx2, realDy2, sx1, sy1, sx2, sy2, rotation, Color(tintColor.r, tintColor.g, tintColor.b, (byte)(tintColor.a * alpha)));

			endDraw();
		}
	}

	void Graphics::drawImage(TextureImage*img, const RectangleD& dst, const RectangleU& src)
	{
		drawImage(img, dst.x, dst.y, dst.x+dst.width, dst.y+dst.height, src.x, src.y, src.x+src.width, src.y+src.height);
	}
}
