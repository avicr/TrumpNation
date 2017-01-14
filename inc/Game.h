#ifndef GAME_H
#define GAME_H

#include "../inc/Globals.h"

class Game
{
public:
	int WallArray[16];


	Game();
	~Game();

	void OnMexicanEscaped();
	int GetLevelNumber();
	void SetLevel(int Level);
	bool LevelComplete();
	int GetNumMexicansEscaped();	
	SDL_Texture *GetGroundTexture();

protected:
	SDL_Texture *GroundTexture;
	//int GroundTileMap[5][16];
	int NumMexicansEscaped;
	int CurrentLevel;

	void GenerateRandomTerrain();
};

#endif