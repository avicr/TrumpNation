#include <string.h>
#include "../inc/Game.h"
#include "../inc/Globals.h"
#include "../inc/SpriteList.h"
#include "../inc/TrumpPlayerSprite.h"

Game::Game()
{
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
	NumMexicansEscaped = 0;
	CurrentLevel = 1;
}

void Game::OnMexicanEscaped()
{
	NumMexicansEscaped++;
}

void Game::SetLevel(int NewLevel)
{
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