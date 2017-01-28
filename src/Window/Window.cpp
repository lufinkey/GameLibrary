
#include <GameLibrary/Window/Window.hpp>
#include <GameLibrary/Utilities/PlatformChecks.hpp>
#include "../Application/EventManager.hpp"
#include <SDL.h>
#include <SDL_syswm.h>

#if defined(TARGETPLATFORM_WINDOWS)
	#include <Windows.h>
#endif

namespace fgl
{
//WindowSettings implementation

	WindowSettings::WindowSettings()
		: position(Vector2i(Window::POSITION_UNDEFINED, Window::POSITION_UNDEFINED)),
		size(Vector2u(640, 480)),
		title("Program"),
		icon(nullptr),
		backgroundColor(Color::WHITE),
		style(Window::STYLE_DEFAULT)
	{
		//
	}

	WindowSettings::WindowSettings(const Vector2i& position, const Vector2u& size, const String& title, Image* icon, const Color& backgroundColor, Uint32 style)
		: position(position),
		size(size),
		title(title),
		icon(icon),
		backgroundColor(backgroundColor),
		style(style)
	{
		//
	}

	WindowSettings::~WindowSettings()
	{
		//
	}
		
	void WindowSettings::setPosition(const Vector2i&pos)
	{
		position = pos;
	}

	const Vector2i& WindowSettings::getPosition() const
	{
		return position;
	}
		
	void WindowSettings::setSize(const Vector2u&sz)
	{
		size = sz;
	}

	const Vector2u& WindowSettings::getSize() const
	{
		return size;
	}
		
	void WindowSettings::setTitle(const String&ttl)
	{
		title = ttl;
	}

	const String& WindowSettings::getTitle() const
	{
		return title;
	}
	
	void WindowSettings::setIcon(Image*ico)
	{
		icon = ico;
	}

	const Image* WindowSettings::getIcon() const
	{
		return icon;
	}
	
	void WindowSettings::setBackgroundColor(const Color&bgcolor)
	{
		backgroundColor = bgcolor;
	}
	
	const Color& WindowSettings::getBackgroundColor() const
	{
		return backgroundColor;
	}
	
	void WindowSettings::setStyle(Uint32 sty)
	{
		style = sty;
	}
	
	Uint32 WindowSettings::getStyle() const
	{
		return style;
	}
	
//Window implementation
	
	WindowSettings Window::getDefaultSettings()
	{
		#if defined(TARGETPLATFORMTYPE_MOBILE)
			RectangleI displayBounds = getDisplayBounds(0);
			return WindowSettings(Vector2i(0,0), Vector2u((unsigned int)displayBounds.width, (unsigned int)displayBounds.height), "", nullptr, Color::WHITE, Window::STYLE_BORDERLESS);
		#else
			return WindowSettings(Vector2i(Window::POSITION_UNDEFINED, Window::POSITION_UNDEFINED), Vector2u(640, 480), "", nullptr, Color::WHITE, Window::STYLE_RESIZABLE);
		#endif
	}
	
	Window::Window()
	{
		view = nullptr;
		windowdata = nullptr;
		windowID = 0;
		icondata = nullptr;
		graphics = nullptr;
	}
	
	Window::~Window()
	{
		if(windowdata != nullptr)
		{
			destroy();
		}
		if(view != nullptr)
		{
			delete view;
			view = nullptr;
		}
		if(icondata != nullptr)
		{
			SDL_FreeSurface((SDL_Surface*)icondata);
			icondata = nullptr;
		}
	}
	
	void Window::create(const WindowSettings& windowSettings)
	{
		if(windowdata != nullptr)
		{
			destroy();
		}
		
		int positionx = 0;
		int positiony = 0;
		#ifndef TARGETPLATFORMTYPE_MOBILE
			if(windowSettings.position.x == Window::POSITION_CENTERED)
			{
				positionx = SDL_WINDOWPOS_CENTERED;
			}
			else if(windowSettings.position.x == Window::POSITION_UNDEFINED)
			{
				positionx = SDL_WINDOWPOS_UNDEFINED;
			}
			if(windowSettings.position.y == Window::POSITION_CENTERED)
			{
				positiony = SDL_WINDOWPOS_CENTERED;
			}
			else if(windowSettings.position.y == Window::POSITION_UNDEFINED)
			{
				positiony = SDL_WINDOWPOS_UNDEFINED;
			}
		#endif
		
		Uint32 flags = 0;
		Uint32 style = windowSettings.style;
		if(style != Window::STYLE_DEFAULT)
		{
			if((style & Window::STYLE_BORDERLESS) == Window::STYLE_BORDERLESS)
			{
				flags = flags | SDL_WINDOW_BORDERLESS;
			}
			if((style & Window::STYLE_FULLSCREEN) == Window::STYLE_FULLSCREEN)
			{
				flags = flags | SDL_WINDOW_FULLSCREEN;
			}
			if((style & Window::STYLE_HIDDEN) == Window::STYLE_HIDDEN)
			{
				flags = flags | SDL_WINDOW_HIDDEN;
			}
			else
			{
				flags = flags | SDL_WINDOW_SHOWN;
			}
			if((style & Window::STYLE_RESIZABLE) == Window::STYLE_RESIZABLE)
			{
				flags = flags | SDL_WINDOW_RESIZABLE;
			}
			if((style & Window::STYLE_MINIMIZED) == Window::STYLE_MINIMIZED)
			{
				flags = flags | SDL_WINDOW_MINIMIZED;
			}
			else if((style & Window::STYLE_MAXIMIZED) == Window::STYLE_MAXIMIZED)
			{
				flags = flags | SDL_WINDOW_MAXIMIZED;
			}
		}
		else
		{
			flags = flags | SDL_WINDOW_SHOWN;
		}
		
		windowdata = (void*)SDL_CreateWindow(windowSettings.title,positionx,positiony,windowSettings.size.x,windowSettings.size.y, flags | SDL_WINDOW_OPENGL);
		if(windowdata == nullptr)
		{
			//TODO replace with more specific exception type
			throw Exception(SDL_GetError());
		}
		windowID = SDL_GetWindowID((SDL_Window*)windowdata);
		
		if(windowSettings.getIcon()!=nullptr)
		{
			icondata = createIconData(windowSettings.getIcon());
			if(icondata!=nullptr)
			{
				SDL_SetWindowIcon((SDL_Window*)windowdata, (SDL_Surface*)icondata);
			}
		}
		
		windowed_size = windowSettings.size;
		bool createdView = false;
		if(view == nullptr)
		{
			view = new View((double)windowed_size.x, (double)windowed_size.y);
			createdView = true;
		}
		
		try
		{
			graphics = new Graphics(*this);
		}
		catch(const Exception&e)
		{
			SDL_DestroyWindow((SDL_Window*)windowdata);
			windowdata = nullptr;
			windowID = 0;
			if(createdView)
			{
				delete view;
				view = nullptr;
			}
			if(icondata!=nullptr)
			{
				SDL_FreeSurface((SDL_Surface*)icondata);
				icondata = nullptr;
			}
			//TODO replace with more specific exception type
			throw Exception(e);
		}
		
		if((style & Window::STYLE_FULLSCREEN) != Window::STYLE_FULLSCREEN)
		{
			windowed_size = windowSettings.size;
		}
		else
		{
			if(view!=nullptr)
			{
				Vector2d viewSize = view->getSize();
				windowed_size = Vector2u((unsigned int)viewSize.x, (unsigned int)viewSize.y);
			}
			else
			{
				windowed_size = windowSettings.size;
			}
		}

		assetManager = new AssetManager(*this);

		EventManager::addWindow(this);
	}
	
	void Window::refresh()
	{
		if(windowdata!=nullptr)
		{
			if(graphics->renderTarget!=nullptr)
			{
				SDL_SetRenderTarget((SDL_Renderer*)graphics->renderer, nullptr);
				if(view==nullptr || view->matchesWindow())
				{
					SDL_SetRenderDrawColor((SDL_Renderer*)graphics->renderer, 0, 0, 0, 0);
					SDL_RenderClear((SDL_Renderer*)graphics->renderer);
					Vector2u windowSize = getSize();
					SDL_Rect dstRect;
					dstRect.x = 0;
					dstRect.y = 0;
					dstRect.w = (int)windowSize.x;
					dstRect.h = (int)windowSize.y;
					SDL_RenderCopy((SDL_Renderer*)graphics->renderer, (SDL_Texture*)graphics->renderTarget, nullptr, &dstRect);
				}
				else if(view->isLetterboxed())
				{
					SDL_SetRenderDrawColor((SDL_Renderer*)graphics->renderer, 0, 0, 0, 0);
					SDL_RenderClear((SDL_Renderer*)graphics->renderer);
					
					double multScale = 1;
					Vector2d winSize = (Vector2d)getSize();
					Vector2d viewSize((double)graphics->renderTarget_width, (double)graphics->renderTarget_height);
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
					
					double letterBoxW = (winSize.x - fixedWidth)/2;
					double letterBoxH = (winSize.y - fixedHeight)/2;
					
					SDL_Rect dstRect;
					dstRect.x = (int)letterBoxW;
					dstRect.y = (int)letterBoxH;
					dstRect.w = (int)(winSize.x-(2*letterBoxW));
					dstRect.h = (int)(winSize.y-(2*letterBoxH));
					
					SDL_RenderSetClipRect((SDL_Renderer*)graphics->renderer, nullptr);
					SDL_RenderCopy((SDL_Renderer*)graphics->renderer, (SDL_Texture*)graphics->renderTarget, nullptr, &dstRect);
				}
				else
				{
					SDL_SetRenderDrawColor((SDL_Renderer*)graphics->renderer, 0, 0, 0, 0);
					SDL_RenderClear((SDL_Renderer*)graphics->renderer);
					
					SDL_Rect dstRect;
					dstRect.x = 0;
					dstRect.y = 0;
					dstRect.w = (int)graphics->renderTarget_width;
					dstRect.h = (int)graphics->renderTarget_height;
					
					SDL_RenderCopy((SDL_Renderer*)graphics->renderer, (SDL_Texture*)graphics->renderTarget, nullptr, &dstRect);
				}
			}
			//SDL_GL_SwapWindow((SDL_Window*)windowdata);
			SDL_RenderPresent((SDL_Renderer*)graphics->renderer);
			graphics->reset(backgroundColor);
		}
	}
	
	void Window::clear(const Color&clearColor)
	{
		if(windowdata != nullptr)
		{
			SDL_Renderer* renderer = SDL_GetRenderer((SDL_Window*)windowdata);
			SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);
		}
	}
	
	void Window::destroy()
	{
		if(windowdata != nullptr)
		{
			delete assetManager;
			assetManager = nullptr;
			delete graphics;
			graphics = nullptr;
			SDL_DestroyWindow((SDL_Window*)windowdata);
			windowdata = nullptr;
			if(view != nullptr)
			{
				delete view;
				view = nullptr;
			}
			EventManager::removeWindow(this);
			windowID = 0;
		}
	}
	
	Image* Window::capture()
	{
		if(windowdata == nullptr)
		{
			return nullptr;
		}

		Image*img = new Image();
		int winWidth = 0;
		int winHeight = 0;
		SDL_GetWindowSize((SDL_Window*)windowdata, &winWidth, &winHeight);
		img->create((unsigned int)winWidth, (unsigned int)winHeight);
		if(SDL_RenderReadPixels((SDL_Renderer*)graphics->renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, (void*)(img->getPixels().getData()), (unsigned int)winWidth * 4) < 0)
		{
			//TODO replace with more specific exception type
			throw Exception(SDL_GetError());
		}
		return img;
	}
	
	Graphics* Window::getGraphics()
	{
		return graphics;
	}
	
	AssetManager* Window::getAssetManager()
	{
		return assetManager;
	}
	
	RectangleI Window::getDisplayBounds(unsigned int displayIndex)
	{
		SDL_Rect rect;
		if(SDL_GetDisplayBounds((int)displayIndex, &rect) == 0)
		{
			return RectangleI(rect.x,rect.y,rect.w,rect.h);
		}
		return RectangleI();
	}
	
	Vector2i Window::getPosition() const
	{
		if(windowdata != nullptr)
		{
			int x = 0;
			int y = 0;
			SDL_GetWindowPosition((SDL_Window*)windowdata,&x,&y);
			return Vector2i(x, y);
		}
		return Vector2i(0,0);
	}
	
	void Window::setPosition(const Vector2i&pos)
	{
		if(windowdata!=nullptr)
		{
			#ifndef TARGETPLATFORMTYPE_MOBILE
				SDL_SetWindowPosition((SDL_Window*)windowdata,pos.x,pos.y);
			#endif
		}
	}
	
	Vector2u Window::getSize() const
	{
		if(windowdata != nullptr)
		{
			int w = 0;
			int h = 0;
			SDL_GetWindowSize((SDL_Window*)windowdata,&w,&h);
			return Vector2u((unsigned int)w, (unsigned int)h);
		}
		return Vector2u(0, 0);
	}
	
	void Window::setSize(const Vector2u&size)
	{
#if defined(TARGETPLATFORMTYPE_DESKTOP)
		if(windowdata != nullptr)
		{
			if((SDL_GetWindowFlags((SDL_Window*)windowdata) & SDL_WINDOW_FULLSCREEN) != SDL_WINDOW_FULLSCREEN)
			{
				//update windowed_size if window is not fullscreen
				windowed_size = size;
			}
			SDL_SetWindowSize((SDL_Window*)windowdata,(int)size.x,(int)size.y);
			if(view != nullptr && view->matchesWindow())
			{
				view->setSize((double)size.x, (double)size.y);
			}
		}
#endif
	}
	
	String Window::getTitle() const
	{
		if(windowdata != nullptr)
		{
			return SDL_GetWindowTitle((SDL_Window*)windowdata);
		}
		return "";
	}
	
	void Window::setTitle(const String&title)
	{
		if(windowdata != nullptr)
		{
			SDL_SetWindowTitle((SDL_Window*)windowdata, title);
		}
	}
	
	Color Window::getBackgroundColor() const
	{
		return backgroundColor;
	}
	
	void Window::setBackgroundColor(const Color&bgcolor)
	{
		backgroundColor = bgcolor;
	}
	
	void Window::setIcon(Image*icon)
	{
		if(windowdata != nullptr)
		{
			if(icondata != nullptr)
			{
				SDL_FreeSurface((SDL_Surface*)icondata);
				icondata = nullptr;
			}
			if(icon == nullptr)
			{
				SDL_SetWindowIcon((SDL_Window*)windowdata, nullptr);
			}
			else
			{
				icondata = createIconData(icon);
				SDL_SetWindowIcon((SDL_Window*)windowdata, (SDL_Surface*)icondata);
			}
		}
	}
	
	View* Window::getView() const
	{
		return view;
	}
	
	bool Window::isOpen() const
	{
		if(windowdata != nullptr)
		{
			return true;
		}
		return false;
	}
	
	bool Window::isFocused() const
	{
		if(windowdata != nullptr)
		{
			Uint32 flags = SDL_GetWindowFlags((SDL_Window*)windowdata);
			if((flags & SDL_WINDOW_INPUT_FOCUS) == SDL_WINDOW_INPUT_FOCUS)
			{
				return true;
			}
		}
		return false;
	}
	
	bool Window::isVisible() const
	{
		if(windowdata != nullptr)
		{
			Uint32 flags = SDL_GetWindowFlags((SDL_Window*)windowdata);
			if((flags & SDL_WINDOW_SHOWN) == SDL_WINDOW_SHOWN)
			{
				return true;
			}
		}
		return false;
	}
	
	void Window::setVisible(bool toggle)
	{
		if(windowdata != nullptr)
		{
			Uint32 flags = SDL_GetWindowFlags((SDL_Window*)windowdata);
			if(toggle)
			{
				if((flags & SDL_WINDOW_HIDDEN) == SDL_WINDOW_HIDDEN)
				{
					SDL_ShowWindow((SDL_Window*)windowdata);
				}
			}
			else
			{
				if((flags & SDL_WINDOW_SHOWN) == SDL_WINDOW_SHOWN)
				{
					SDL_HideWindow((SDL_Window*)windowdata);
				}
			}
		}
	}
	
	bool Window::isFullscreen() const
	{
		if(windowdata != nullptr)
		{
			Uint32 flags = SDL_GetWindowFlags((SDL_Window*)windowdata);
			if((flags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN)
			{
				return true;
			}
		}
		return false;
	}
	
	void Window::setFullscreen(bool toggle)
	{
		setFullscreen(toggle, windowed_size.x, windowed_size.y);
	}
	
	void Window::setFullscreen(bool toggle, unsigned int width, unsigned int height)
	{
		if(windowdata != nullptr)
		{
			Uint32 flags = SDL_GetWindowFlags((SDL_Window*)windowdata);
			if(toggle)
			{
				if((flags & SDL_WINDOW_FULLSCREEN) != SDL_WINDOW_FULLSCREEN)
				{
					int oldWidth = 0;
					int oldHeight = 0;
					SDL_GetWindowSize((SDL_Window*)windowdata, &oldWidth, &oldHeight);
					SDL_SetWindowSize((SDL_Window*)windowdata, (int)width, (int)height);
					if(SDL_SetWindowFullscreen((SDL_Window*)windowdata, SDL_WINDOW_FULLSCREEN)==0)
					{
						windowed_size = Vector2u((unsigned int)oldWidth, (unsigned int)oldHeight);
						if(view != nullptr && view->matchesWindow())
						{
							view->setSize((double)width, (double)height);
						}
					}
					else
					{
						//An error occured. I guess just ignore this...?
					}
				}
			}
			else
			{
				if((flags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN)
				{
					if(SDL_SetWindowFullscreen((SDL_Window*)windowdata, 0)==0)
					{
						SDL_SetWindowSize((SDL_Window*)windowdata, (int)width, (int)height);
						if(view != nullptr && view->matchesWindow())
						{
							view->setSize((double)width, (double)height);
						}
					}
					else
					{
						//An error occured. I guess just ignore this...?
					}
				}
			}
		}
	}
	
	void* Window::createIconData(const Image* icon) const
	{
		void*icondata = nullptr;
		if(icon != nullptr)
		{
			const ArrayList<Color>& pixels = icon->getPixels();
			if(pixels.size()>0)
			{
				icondata = (void*)SDL_CreateRGBSurfaceFrom((void*)pixels.getData(), (int)icon->getWidth(), (int)icon->getHeight(), 32, (int)(icon->getWidth()*4), 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
			}
		}
		return icondata;
	}
	
	void Window::addEventListener(WindowEventListener*listener)
	{
		listenermutex.lock();
		eventListeners.add(listener);
		listenermutex.unlock();
	}
	
	void Window::removeEventListener(WindowEventListener*listener)
	{
		listenermutex.lock();
		size_t index = eventListeners.indexOf(listener);
		if(index != ArrayList<WindowEventListener*>::NOT_FOUND)
		{
			eventListeners.remove(index);
		}
		listenermutex.unlock();
	}
	
	void Window::callListenerEvent(byte eventType, int x, int y, bool external, bool* returnVal)
	{
		listenermutex.lock();
		ArrayList<WindowEventListener*> listeners = eventListeners;
		listenermutex.unlock();
		
		bool closeWindowCatch = true;
		
		for(unsigned int i = 0; i<listeners.size(); i++)
		{
			WindowEventListener* listener = listeners.get(i);
			bool returnCatch = false;
			switch(eventType)
			{
				case SDL_WINDOWEVENT_SHOWN:
				listener->onWindowShown(this);
				break;
				
				case SDL_WINDOWEVENT_HIDDEN:
				listener->onWindowHidden(this);
				break;
				
				case SDL_WINDOWEVENT_EXPOSED:
				listener->onWindowExposed(this);
				break;
				
				case SDL_WINDOWEVENT_MOVED:
				listener->onWindowMoved(this, Vector2d((double)x, (double)y));
				break;
				
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				listener->onWindowResized(this, Vector2u((unsigned int)x,(unsigned int)y), external);
				break;
				
				case SDL_WINDOWEVENT_MINIMIZED:
				listener->onWindowMinimize(this);
				break;
				
				case SDL_WINDOWEVENT_MAXIMIZED:
				listener->onWindowMinimize(this);
				break;
				
				case SDL_WINDOWEVENT_RESTORED:
				listener->onWindowRestore(this);
				break;
				
				case SDL_WINDOWEVENT_ENTER:
				listener->onWindowMouseEnter(this);
				break;
				
				case SDL_WINDOWEVENT_LEAVE:
				listener->onWindowMouseLeave(this);
				break;
				
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				listener->onWindowFocusGained(this);
				break;
				
				case SDL_WINDOWEVENT_FOCUS_LOST:
				listener->onWindowFocusLost(this);
				break;
				
				case SDL_WINDOWEVENT_CLOSE:
				returnCatch = listener->onWindowClose(this);
				if(!returnCatch)
				{
					closeWindowCatch = false;
				}
				if(returnVal!=nullptr)
				{
					*returnVal = closeWindowCatch;
				}
				break;
			}
		}
	}
	
	TransformD Window::getViewTransform() const
	{
		TransformD transform;
		
		if(windowdata == nullptr)
		{
			return transform;
		}
		
		double zoom = 1;
		if(view!=nullptr)
		{
			zoom = view->getZoom();
		}
		
		if(view == nullptr || view->matchesWindow())
		{
			const Vector2u& winSz = getSize();
			Vector2d winSize = Vector2d((double)winSz.x, (double)winSz.y);
			if(view != nullptr)
			{
				view->setSize((double)winSz.x, (double)winSz.y);
			}
			
			double difX = (winSize.x - (winSize.x*zoom))/(2*zoom);
			double difY = (winSize.y - (winSize.y*zoom))/(2*zoom);
			
			transform.scale(zoom, zoom);
			transform.translate(difX, difY);
		}
		else if(view->isLetterboxed())
		{
			double multScale = 1;
			Vector2u winSz = getSize();
			Vector2d winSize = Vector2d((double)winSz.x, (double)winSz.y);
			Vector2d viewSize = view->getSize();
			
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
			
			double scaleVal = zoom*multScale;
			
			transform.scale(scaleVal, scaleVal);
			transform.translate(difX, difY);
		}
		else
		{
			Vector2u winSz = getSize();
			Vector2d winSize = Vector2d((double)winSz.x, (double)winSz.y);
			Vector2d viewSize = view->getSize();
			
			double ratX = winSize.x /viewSize.x;
			double ratY = winSize.y /viewSize.y;
			
			double difX = (winSize.x - (winSize.x*zoom))/(2*zoom*ratX);
			double difY = (winSize.y - (winSize.y*zoom))/(2*zoom*ratY);
			
			double scaleValX = (zoom*ratX);
			double scaleValY = (zoom*ratY);

			transform.scale(scaleValX, scaleValY);
			transform.translate(difX, difY);
		}
		return transform;
	}
	
	void Window::getHandlePtr(void*ptr) const
	{
		if(windowdata!=nullptr)
		{
			SDL_SysWMinfo info;
			SDL_GetWindowWMInfo((SDL_Window*)windowdata, &info);
			#if defined(TARGETPLATFORM_WINDOWS)
				*((HWND*)ptr) = info.info.win.window;
			#else
				//TODO handle other systems
			#endif
		}
	}
	
	void* Window::getWindowData() const
	{
		return windowdata;
	}

	WindowEventListener::~WindowEventListener()
	{
		//
	}

	void WindowEventListener::onWindowShown(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowHidden(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowExposed(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowMoved(Window*window, const Vector2d&position)
	{
		//
	}

	void WindowEventListener::onWindowResized(Window*window, const Vector2u&size, bool external)
	{
		//
	}

	void WindowEventListener::onWindowMinimize(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowMaximize(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowRestore(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowMouseEnter(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowMouseLeave(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowFocusGained(Window*window)
	{
		//
	}

	void WindowEventListener::onWindowFocusLost(Window*window)
	{
		//
	}

	bool WindowEventListener::onWindowClose(Window*window)
	{
		return true;
	}
}
