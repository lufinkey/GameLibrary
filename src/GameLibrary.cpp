
#include <GameLibrary/GameLibrary.hpp>

#ifndef TARGETPLATFORM_WINDOWS

int main(int argc, char* argv[]);

#if defined(TARGETPLATFORM_MAC) || defined(TARGETPLATFORM_IOS)
int GameLibrary_autorelease_main(int argc, char* argv[]);
#endif

#ifdef main
#undef main
#endif

#include <SDL_main.h>

int main(int argc, char *argv[])
{
	#if defined(TARGETPLATFORM_MAC) || defined(TARGETPLATFORM_IOS)
		return GameLibrary_autorelease_main(argc, argv);
	#else
		return GameLibrary_main(argc, argv);
	#endif
}

#endif

