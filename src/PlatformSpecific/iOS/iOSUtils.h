
#pragma once

#include <GameLibrary/Utilities/PlatformChecks.hpp>

#ifdef TARGETPLATFORM_IOS

#import <UIKit/UIKit.h>

namespace fgl
{
	class Window;
	
	UIViewController* Window_getRootViewController(Window* window);
}

#endif
