#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif 

#include <vector>
#define TEXTURE_PATH "resource/textures/"
#define HORIZON 264
#define WALL_TOP HORIZON - 104

extern class ResourceManager *GResourceManager;
extern SDL_Window *GWindow;
extern SDL_Renderer * GRenderer;
extern bool bSDLInitialized;
extern bool WallArray[16];
extern std::vector <class Mexican1Sprite*> Mexicans;
extern std::vector <class ItemSprite*> Items;

SDL_Renderer *GetRenderer();
#endif