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
#include <string>
#define TEXTURE_PATH "resource/textures/"
#define HORIZON 264
#define WALL_TOP HORIZON - 104
#define MEXICAN_SPAWN_RATE 0.25
#define WALL_PLACE_ZONE 140
#define ITEM_RATE 4
#define ITEM_SPAWN_PERCENT 5
#define SCORE_LIFE_TIME 1
#define ITEM_LIFE_TIME 8
#define TRUMP_DEFAULT_MAX_VELOCITY 333
#define TRUMP_RED_HAT_MAX_VELOCITY 500
#define RED_HAT_TIME 8
#define TITLE_SCROLL_TIME 740

#define PLACE_WALL_SCORE 500
#define MEXICAN_BLOCK_SCORE 50

#define BOMB_FLASH_TIME 0.20;

extern class ResourceManager *GResourceManager;
extern SDL_Window *GWindow;
extern SDL_Renderer * GRenderer;
extern bool bSDLInitialized;
extern class SpriteList Mexicans;
extern class SpriteList Items;
extern class TrumpPlayerSprite *ThePlayer;

extern Mix_Chunk *LevelClearFX;
extern Mix_Chunk *TrumpDieFX;
extern Mix_Chunk *PickUpItemFX;
extern Mix_Chunk *PlaceWallFX;
extern Mix_Chunk *StepFX;
extern Mix_Music *BGMusic;
extern Mix_Music *TitleMusic;
extern Mix_Music *HatDanceMusic;
extern Game *TheGame;

extern bool bFreezeSpawn;
extern bool bSwapSprites;

extern struct Glyph Numerals36[10];
extern struct Glyph Numerals20[10];
extern struct Glyph NumeralsWhite20[10];

extern double BombCountDown;

SDL_Renderer *GetRenderer();
extern void DrawText(std::string Text, int X, int Y, int SizeX, int SizeY, SDL_Renderer *Renderer, struct Glyph Glyphs[10], float ScaleX = 1, float ScaleY = 1);
#endif