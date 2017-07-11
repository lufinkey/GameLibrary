
#pragma once

#ifndef HAVE_STDIO_H
    #define HAVE_STDIO_H
#endif
#include <SDL.h>
#include <cstdio>

SDL_RWops* SDL_RWFromFILE(FILE* file, SDL_bool autoclose);
