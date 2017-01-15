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
#define HIGH_SCORE_DISPLAY_COUNT 8
#define FACT_TIME 7
#define GLOBAL_SCALE 2.25

#define NUM_CLOUDS 5
#define CLOUD_SCALE_MIN 0.5
#define CLOUD_SCALE_MAX 2
#define CLOUD_Y_MIN 40
#define CLOUD_Y_MAX 85
#define CLOUD_SPEED_MIN 2
#define CLOUD_SPEED_MAX 4

#define NUM_RANDOM_GROUND_TILES 10
#define NUM_RANDOM_MOUNTAIN_TILES 2
#define MOUNT_TILES_START NUM_RANDOM_GROUND_TILES


#ifdef _WIN32
#define JOY_Y_AXIS 1
#else
#define JOY_Y_AXIS 3
#endif
#define JOYSTICK_DEAD_ZONE 8000

#define PLACE_WALL_SCORE 500
#define MEXICAN_BLOCK_SCORE 50
#define LEVEL_CLEAR_POINTS 2000
#define MEXICAN_ESCAPED_POINTS -50

#define BOMB_FLASH_TIME 0.20

using namespace std;

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
extern class Game *TheGame;

extern bool bFreezeSpawn;
extern bool bSwapSprites;
extern bool bRenderCollision;


extern struct Glyph FontSeg36[94];
extern struct Glyph FontSeg20[94];
extern struct Glyph FontSeg20White[94];
extern struct Glyph FontShadowedWhite[128];
extern struct Glyph FontBlue[128];

extern double BombCountDown;

SDL_Renderer *GetRenderer();
extern void DrawText(string Text, int X, int Y, int SizeX, int SizeY, SDL_Renderer *Renderer, Glyph Glyphs[127], float SpaceScaleX = 1, float SpaceScaleY = 1, bool bRightJustify = false);
extern void DrawTextBitmap(string Text, int X, int Y, int SizeX, int SizeY, SDL_Renderer *Renderer, bool bRightJustified = false);
#endif