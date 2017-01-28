
#include <GameLibrary/GameLibrary.hpp>

#ifndef TARGETPLATFORM_WINDOWS
int main(int argc, char *argv[]);

#ifdef main
#undef main
#endif

#include <SDL_main.h>

int main(int argc, char *argv[])
{
	return GameLibrary_main(argc, argv);
}
#endif
