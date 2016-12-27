#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif 

extern SDL_Renderer * GRenderer;
extern class ResourceManager *TheResourceManager;

#endif