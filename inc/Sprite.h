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

	// Screen region to render the sprite to
	SDL_Rect Rect;

	// Pointer to the current frame in the animation
	SDL_Texture* CurrentFrame;

public:
	Sprite();
	void SetFrame(SDL_Texture* Texture);
	void SetPosition(int NewX, int NewY);
	void Render(SDL_Renderer* Renderer);
};

#endif
