#ifndef SPRITE_H
#define SPRITE_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif

#include <vector>
#include <string>

using namespace std;

class Sprite
{
protected:
	SDL_Rect m_Pos;
	vector <SDL_Texture*> Frames;

public:
	Sprite();
	void LoadSpriteSheet(string FileName, int SpriteWidth, int SpriteHeight);
};

#endif
