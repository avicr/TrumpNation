#ifndef SPRITE_H
#define SPRITE_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif

#include <vector>
#include <string>
#include "../inc/ResourceManager.h"

using namespace std;

class Sprite
{
protected:
	static const double MoveRate;
	// Screen region to render the sprite to
	SDL_Rect Rect;

	// Pointer to the current frame in the animation
	SDL_Texture* Texture;

	AnimInfo AnimData;

	double VelX;
	double VelY;
	double AccelX;
	double AccelY;

	double PosX;
	double PosY;

public:
	Sprite();
	void Tick(double DeltaTime);
	void SetTexture(SDL_Texture* Texture);
	void SetPosition(int NewX, int NewY);
	void Render(SDL_Renderer* Renderer);
	void PlayAnimation(AnimationResource *Anim);
	void StopAnimation();
};

#endif
