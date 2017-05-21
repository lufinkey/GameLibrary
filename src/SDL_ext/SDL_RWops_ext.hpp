
#pragma once

#define HAVE_STDIO_H
#include <SDL.h>
#include <cstdio>

SDL_RWops* SDL_RWFromFILE(FILE* file, SDL_bool autoclose);
