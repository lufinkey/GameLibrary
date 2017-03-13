
#import "iOSUtils.h"
#include <GameLibrary/Window/Window.hpp>
#include "SDL_syswm.h"

namespace fgl
{
	UIViewController* Window_getRootViewController(Window* window)
	{
		if(window==nullptr)
		{
			return [UIApplication sharedApplication].keyWindow.rootViewController;
		}
		
		SDL_Window* windowdata = (SDL_Window*)window->getWindowData();
		if(windowdata==nullptr)
		{
			return nil;
		}
		SDL_SysWMinfo systemWindowInfo;
		SDL_VERSION(&systemWindowInfo.version);
		if (!SDL_GetWindowWMInfo(windowdata, &systemWindowInfo))
		{
			// consider doing some kind of error handling here
			return nil;
		}
		UIWindow* appWindow = systemWindowInfo.info.uikit.window;
		return appWindow.rootViewController;
	}
}
