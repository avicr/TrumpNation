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
#define SPAWN_VARIABLE_TIME 0.25
#define ITEM_RATE 4
#define ITEM_SPAWN_PERCENT 25 //5
#define CAT_SPAWN_PERCENT 4
#define SCORE_LIFE_TIME 1
#define ITEM_LIFE_TIME 8
#define TRUMP_DEFAULT_MAX_VELOCITY 333
#define TRUMP_RED_HAT_MAX_VELOCITY 450
#define RED_HAT_TIME 12
#define TITLE_SCROLL_TIME 20
#define NUM_INTRO_CYCLES 25
#define HIGH_SCORE_DISPLAY_COUNT 8
#define FACT_TIME 7
#define GLOBAL_SCALE 2.25

#define BRICK_FIRST_SPAWN_PENALITY 5
#define BRICK_ON_BRICK_SPAWN_PENALITY 0.25
#define ITEM_ON_BRICK_SPAWN_PENALITY 0.15
#define BRICK_SPAWN_RATE 1.5
#define BRICK_SPAWN_PERCENT 33
#define GOLD_BRICK_SPAWN_PERCENT 20
#define GOLD_BRICK_MILE_START 12
#define SOMBRERO_MILE_START 4
#define EXTRA_LIFE_MILE_START 8
#define MAX_BRICKS 3
#define BRICK_FORCE_TIME_BONUS 0.15

#define NUM_CLOUDS 5
#define CLOUD_SCALE_MIN 0.5
#define CLOUD_SCALE_MAX 2
#define CLOUD_Y_MIN 40
#define CLOUD_Y_MAX 75
#define CLOUD_SPEED_MIN 2
#define CLOUD_SPEED_MAX 4

#define MAX_CLIMBING_SOUNDS 1

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
#define PLACE_GOLD_WALL_SCORE 1500
#define MEXICAN_BLOCK_SCORE 50
#define LEVEL_CLEAR_POINTS 2000
#define MEXICAN_ESCAPED_POINTS -50
#define CAT_SCORE 5000

#define BOMB_FLASH_TIME 0.20

#define CHAN_MEXICAN_JUMP  99
#define CHAN_MEXICAN_CLIMB 98
#define CHAN_ITEM_SPAWN 97
#define CHAN_ITEM_PICKUP 96
#define CHAN_MEXICAN_LAND 95
#define CHAN_TALK 94

using namespace std;

extern class ResourceManager *GResourceManager;
extern SDL_Window *GWindow;
extern SDL_Renderer * GRenderer;
extern bool bSDLInitialized;
extern class SpriteList Mexicans;
extern class SpriteList Items;
extern class SpriteList DecoSprites;
extern class TrumpPlayerSprite *ThePlayer;

extern Mix_Chunk *MexicanSpawnedFX;
extern Mix_Chunk *MexicanEscapedFX;
extern Mix_Chunk *MexicanJumpFX;
extern Mix_Chunk *TalkFX;
extern Mix_Chunk *MexicanClimbFX;
extern Mix_Chunk *MexicanLandFX;
extern Mix_Chunk *BrickSpawnFX;
extern Mix_Chunk *ItemSpawnFX;
extern Mix_Chunk *LevelClearFX;
extern Mix_Chunk *TrumpDieFX;
extern Mix_Chunk *PickUpItemFX;
extern Mix_Chunk *PlaceWallFX;
extern Mix_Chunk *MenuSound1FX;
extern Mix_Chunk *MenuSound2FX;
extern Mix_Chunk *StepFX;
extern Mix_Music *BGMusic;
extern Mix_Music *TitleMusic;
extern Mix_Music *HatDanceMusic;
extern Mix_Music *BGMusicFast;
extern class Game *TheGame;

extern bool bDoSpawnPop;
extern bool bFreezeSpawn;
extern bool bSwapSprites;
extern bool bRenderCollision;
extern double BrickSpawnCountDown;

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