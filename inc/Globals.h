#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif 

#define TEXTURE_PATH "resource/textures/"

extern class ResourceManager *GResourceManager;
extern SDL_Window *GWindow;
extern SDL_Renderer * GRenderer;
extern bool bSDLInitialized;
extern bool WallArray[16];

SDL_Renderer *GetRenderer();
#endif