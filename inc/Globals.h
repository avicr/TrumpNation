#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef _WIN32
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#endif

#include "../inc/Game.h"
#include <vector>
#define TEXTURE_PATH "resource/textures/"
#define HORIZON 264
#define WALL_TOP HORIZON - 104
#define MEXICAN_SPAWN_RATE 0.25
#define WALL_PLACE_ZONE 140

extern class ResourceManager *GResourceManager;
extern SDL_Window *GWindow;
extern SDL_Renderer * GRenderer;
extern bool bSDLInitialized;
extern class SpriteList Mexicans;
extern class SpriteList Items;
extern class TrumpPlayerSprite *ThePlayer;


extern Mix_Chunk *PickUpItemFX;
extern Mix_Chunk *PlaceWallFX;
extern Mix_Chunk *StepFX;
extern Mix_Music *TitleMusic;
extern Game *TheGame;

extern bool bFreezeSpawn;


SDL_Renderer *GetRenderer();
#endif