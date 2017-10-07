
#include <GameLibrary/GameLibrary.hpp>

#if defined(TARGETPLATFORM_MAC) || defined(TARGETPLATFORM_IOS)

int main(int argc, char* argv[]);

#import <Foundation/Foundation.h>

int GameLibrary_autorelease_main(int argc, char *argv[])
{
	@autoreleasepool
	{
		return GameLibrary_main(argc, argv);
	}
}

#endif
