
#include <GameLibrary/GameLibrary.h>

int main(int argc, char *argv[]);

#ifdef main
#undef main
#endif

#include <SDL/SDL_main.h>

int main(int argc, char *argv[])
{
	return GameLibrary_main(argc, argv);
}
