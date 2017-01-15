#include <string.h>
#include "../inc/Game.h"
#include "../inc/Globals.h"
#include "../inc/SpriteList.h"
#include "../inc/TrumpPlayerSprite.h"

Game::Game()
{
	memset(WallArray, 0, sizeof(WallArray));
	GroundTexture = SDL_CreateTexture(GRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 16 * 64, 7 * 64);
	GenerateRandomTerrain();
	/*WallArray[0] = true;
	WallArray[1] = true;
	WallArray[2] = true;
	WallArray[3] = true;
	WallArray[4] = true;
	WallArray[5] = true;
	WallArray[6] = true;
	WallArray[7] = true;
	WallArray[8] = false;
	WallArray[9] = true;
	WallArray[10] = true;
	WallArray[11] = true;
	WallArray[12] = true;
	WallArray[13] = true;
	WallArray[14] = true;
	WallArray[15] = true;*/	
	NumMexicansEscaped = 0;
	CurrentLevel = 1;
}

void Game::OnMexicanEscaped()
{
	NumMexicansEscaped++;
}

void Game::SetLevel(int NewLevel)
{
	if (NewLevel != CurrentLevel)
	{
		GenerateRandomTerrain();
	}
	CurrentLevel = NewLevel;
	memset(WallArray, 0, sizeof(WallArray));

	/*WallArray[0] = true;
	WallArray[1] = true;
	WallArray[2] = true;
	WallArray[3] = true;
	WallArray[4] = true;
	WallArray[5] = true;
	WallArray[6] = true;
	WallArray[7] = true;
	WallArray[8] = false;
	WallArray[9] = true;
	WallArray[10] = true;
	WallArray[11] = true;
	WallArray[12] = true;
	WallArray[13] = true;
	WallArray[14] = true;
	WallArray[15] = true;*/
}

int Game::GetLevelNumber()
{
	return CurrentLevel;
}

bool Game::LevelComplete()
{
	for (int i = 0; i < 16; i++)
	{
		if (!WallArray[i])
		{
			return false;
		}
	}

	ThePlayer->KillEverything(false);
	Mix_PlayChannel(-1, LevelClearFX, 0);
	SetLevel(CurrentLevel + 1);
	return true;
}

int Game::GetNumMexicansEscaped()
{

	return NumMexicansEscaped;
}

Game::~Game()
{
	SDL_DestroyTexture(GroundTexture);
}

void Game::GenerateRandomTerrain()
{
	SDL_SetRenderTarget(GRenderer, GroundTexture);
	for (int Col = 0; Col < 16; Col++)
	{
		int TileX = 64 * (rand () % 2);

		TileX += MOUNT_TILES_START * 64;

		SDL_Rect SrcRect = { TileX, 0, 64, 64 };
		SDL_Rect DstRect = { Col * 64, 0, 64, 64 };
		
		SDL_RenderCopy(GRenderer, ResourceManager::Tiles->Texture, &SrcRect, &DstRect);		
	}
	
	for (int Row = 0; Row < 5; Row++)
	{
		for (int Col = 0; Col < 16; Col++)
		{
			//GroundTileMap[Row][Col] = rand() % NUM_RANDOM_GROUND_TILES;
			SDL_Rect SrcRect = { 64 * (rand() % NUM_RANDOM_GROUND_TILES), 0, 64, 64 };
			SDL_Rect DstRect = { Col * 64, 128 + Row * 64, 64, 64 };			

			SDL_RenderCopy(GRenderer, ResourceManager::Tiles->Texture, &SrcRect, &DstRect);			
		}
	}

	SDL_SetRenderTarget(GRenderer, NULL);
}

SDL_Texture *Game::GetGroundTexture()
{
	return GroundTexture;
}