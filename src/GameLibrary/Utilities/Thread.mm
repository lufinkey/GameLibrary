
#include <GameLibrary/Utilities/Thread.hpp>
#include <GameLibrary/Utilities/PlatformChecks.hpp>

#if defined(TARGETPLATFORM_MAC) || defined(TARGETPLATFORM_IOS)
#import <Foundation/Foundation.h>

namespace fgl
{
	void Thread::runInAutoreleasePool(const std::function<void()>& func)
	{
		@autoreleasepool
		{
			func();
		}
	}
}

#endif
