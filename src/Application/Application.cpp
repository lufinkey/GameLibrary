
#include <GameLibrary/Application/Application.hpp>
#include <GameLibrary/Exception/InitializeLibraryException.hpp>
#include <GameLibrary/IO/FileTools.hpp>
#include <GameLibrary/Utilities/PlatformChecks.hpp>
#include <GameLibrary/Utilities/Thread.hpp>
#include <GameLibrary/Utilities/Time/DateTime.hpp>
#include "EventManager.hpp"
#include <SDL.h>
#include <ctime>
#include <cstdlib>

#ifdef TARGETPLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

namespace fgl
{
	void ApplicationEventHandler(void*userdata, unsigned int eventtype);
	int ApplicationEventFilter(void*userdata, SDL_Event*event);
	
	class Application_WindowEventListener : public WindowEventListener
	{
	private:
		Application* application;
		
	public:
		Application_WindowEventListener(Application* app) : application(app)
		{
			//
		}
		virtual bool onWindowClose(Window*window) override
		{
			application->close(0);
			return false;
		}
	};

	Application* MainApplication = nullptr;

	Application* Application::getMainApplication()
	{
		return MainApplication;
	}

	Application::Application()
	{
		window = nullptr;
		app_running = false;
		app_closing = false;
		exitcode = EXITCODE_SUCCESS;
		fps = 30;
		sleeptime = (unsigned long long)(1000/fps);

		unsigned int flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS;
		if(!SDL_WasInit(flags))
		{
			if(SDL_Init(flags) != 0)
			{
				throw InitializeLibraryException("SDL", SDL_GetError());
			}
		}

		window = new Window();
		privateWindowListener = (void*)(new Application_WindowEventListener(this));
		window->addEventListener((WindowEventListener*)privateWindowListener);

		#if defined(TARGETPLATFORM_WINDOWS) && defined(_MSC_VER)
		if(IsDebuggerPresent())
		{
			resourceDirectory = FileTools::getCurrentWorkingDirectory();
		}
		else
		{
		#endif
			char* basePath = SDL_GetBasePath();
			if(basePath!=nullptr)
			{
				resourceDirectory = basePath;
				SDL_free(basePath);
			}
			else
			{
				resourceDirectory = FileTools::getCurrentWorkingDirectory();
			}
		#if defined(TARGETPLATFORM_WINDOWS) && defined(_MSC_VER)
		}
		#endif
	}

	Application::~Application()
	{
		delete window;
		delete ((WindowEventListener*)privateWindowListener);
	}

	void Application::initialize()
	{
		//
	}

	void Application::loadContent(AssetManager*assetManager)
	{
		//
	}

	void Application::unloadContent(AssetManager*assetManager)
	{
		//
	}

	void Application::update(ApplicationData appData)
	{
		//
	}

	void Application::draw(ApplicationData appData, Graphics graphics) const
	{
		//
	}

	int Application::run(const WindowSettings&windowSettings, int orientations)
	{
		std::srand((unsigned int)(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())));

		if(!Thread::isMainThread())
		{
			return EXITCODE_NOTMAINTHREAD;
		}
		else if(app_running)
		{
			return EXITCODE_ALREADYRUNNING;
		}

		if(MainApplication == nullptr)
		{
			MainApplication = this;
		}
		app_running = true;

		apptime.reset();

		//window creation
		exitcode = loadMainWindow(windowSettings, orientations);

		if(!app_closing)
		{
			initialize();
		}

		if(!app_closing)
		{
			//application refresh loop
			exitcode = runMainLoop();
		}

		app_running = false;
		app_closing = false;

		if(MainApplication == this)
		{
			MainApplication = nullptr;
		}
		return exitcode;
	}

	void Application::close(int code)
	{
		exitcode = code;
		app_closing = true;
	}

	int Application::loadMainWindow(const WindowSettings&windowSettings, int orientations)
	{
		RectangleI displayRect = Window::getDisplayBounds(0);

		unsigned int bigger;
		unsigned int smaller;
				
		if(displayRect.width > displayRect.height)
		{
			bigger = displayRect.width;
			smaller = displayRect.height;
		}
		else
		{
			bigger = displayRect.height;
			smaller = displayRect.width;
		}

		WindowSettings settings = windowSettings;
		#if defined(TARGETPLATFORMTYPE_MOBILE)
			settings.setPosition(Vector2i(0,0));
			if(((orientations & ORIENTATION_PORTRAIT) == ORIENTATION_PORTRAIT)
			|| ((orientations & ORIENTATION_PORTRAITUPSIDEDOWN) == ORIENTATION_PORTRAITUPSIDEDOWN))
			{
				settings.setSize(Vector2u(smaller,bigger));
			}
			else
			{
				settings.setSize(Vector2u(bigger,smaller));
			}
		#endif

		ArrayList<String> hints;
		if(orientations == ORIENTATION_ALL)
		{
			hints.add("Portrait");
			hints.add("PortraitUpsideDown");
			hints.add("LandscapeLeft");
			hints.add("LandscapeRight");
		}
		else
		{
			if((orientations & ORIENTATION_PORTRAIT) == ORIENTATION_PORTRAIT)
			{
				hints.add("Portrait");
			}
			if((orientations & ORIENTATION_PORTRAITUPSIDEDOWN) == ORIENTATION_PORTRAITUPSIDEDOWN)
			{
				hints.add("PortraitUpsideDown");
			}
			if((orientations & ORIENTATION_LANDSCAPELEFT) == ORIENTATION_LANDSCAPELEFT)
			{
				hints.add("LandscapeLeft");
			}
			if((orientations & ORIENTATION_LANDSCAPERIGHT) == ORIENTATION_LANDSCAPERIGHT)
			{
				hints.add("LandscapeRight");
			}
		}

		String hintString;
		for(unsigned int i=0; i<hints.size(); i++)
		{
			hintString += hints.get(i);
			if(i != (hints.size() - 1))
			{
				hintString += ' ';
			}
		}
		SDL_SetHintWithPriority("SDL_HINT_ORIENTATIONS", hintString, SDL_HINT_OVERRIDE);
		SDL_SetEventFilter(&ApplicationEventFilter, this);
		
		window->create(settings);
		window->clear(Color::BLACK);
		window->getAssetManager()->setRootDirectory(resourceDirectory);
		
		return exitcode;
	}

	int Application::runMainLoop()
	{
		EventManager::update();
		window->refresh();
		
		loadContent(window->getAssetManager());
		
		if(!app_closing) //makes sure Application::close(int) hasn't been called
		{
			window->getGraphics()->reset();
		}
		
		apptime.start();
		
		while(app_running && !app_closing)
		{
			long long startFrameTime = apptime.getMilliseconds();

			EventManager::update();
			if(EventManager::recievedQuitRequest() || !window->isOpen())
			{
				this->close(0);
			}
			
			double framespeedMult = (double)(((long double)sleeptime)/((long double)1000));
			ApplicationData appdata(this, window, window->getAssetManager(), apptime, window->getViewportTransform(), framespeedMult);
			if(!app_closing)
			{
				update(appdata);
			}
			if(!app_closing)
			{
				draw(appdata, *(window->getGraphics()));
			}
			if(!app_closing)
			{
				window->refresh();

				long long endFrameTime = apptime.getMilliseconds();
				unsigned long long totalFrameTime = (unsigned long long)(endFrameTime - startFrameTime);
				if(totalFrameTime > sleeptime || sleeptime==0)
				{
					Thread::sleep(1);
				}
				else
				{
					unsigned long long adjustedSleepTime = sleeptime - totalFrameTime;
					Thread::sleep(adjustedSleepTime);
				}
			}
		}

		apptime.stop();

		unloadContent(window->getAssetManager());

		window->destroy();

		return exitcode;
	}

	void Application::setFPS(unsigned int framerate)
	{
		fps = framerate;
		if(fps==0)
		{
			sleeptime = 1;
		}
		else
		{
			sleeptime = (unsigned long long)(1000/fps);
		}
	}

	unsigned int Application::getFPS() const
	{
		return fps;
	}

	Window* Application::getWindow() const
	{
		return window;
	}

	const TimeInterval& Application::getTime() const
	{
		return apptime;
	}
	
	const String& Application::getResourceDirectory() const
	{
		return resourceDirectory;
	}

	void Application::callListenerEvent(unsigned int eventtype)
	{
		listenermutex.lock();
		ArrayList<ApplicationEventListener*> listeners = eventListeners;
		listenermutex.unlock();

		for(unsigned int i=0; i<listeners.size(); i++)
		{
			ApplicationEventListener* listener = listeners.get(i);

			switch(eventtype)
			{
				case SDL_APP_WILLENTERBACKGROUND:
				listener->onApplicationWillEnterBackground(this);
				break;

				case SDL_APP_DIDENTERBACKGROUND:
				listener->onApplicationDidEnterBackground(this);
				break;

				case SDL_APP_WILLENTERFOREGROUND:
				listener->onApplicationWillEnterForeground(this);
				break;

				case SDL_APP_DIDENTERFOREGROUND:
				listener->onApplicationDidEnterForeground(this);
				break;

				case SDL_APP_LOWMEMORY:
				listener->onApplicationLowMemory(this);
				break;
				
				case SDL_APP_TERMINATING:
				listener->onApplicationTerminating(this);
				break;
			}
		}
	}

	int ApplicationEventFilter(void*userdata, SDL_Event*event)
	{
		ApplicationEventHandler(userdata, event->type);
		return 1;
	}

	void ApplicationEventHandler(void*userdata, unsigned int eventtype)
	{
		Application*application = (Application*)userdata;
		switch(eventtype)
		{
			case SDL_APP_WILLENTERBACKGROUND:
			break;

			case SDL_APP_DIDENTERBACKGROUND:
			break;

			case SDL_APP_WILLENTERFOREGROUND:
			break;

			case SDL_APP_DIDENTERFOREGROUND:
			break;

			case SDL_APP_LOWMEMORY:
			break;
			
			case SDL_APP_TERMINATING:
			break;
		}
		application->callListenerEvent(eventtype);
	}
}
