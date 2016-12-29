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

enum eMovingFlags
{
	MOVING_NONE  = 0,
	MOVING_UP    = 1,
	MOVING_DOWN  = 2,
	MOVING_LEFT  = 4,
	MOVING_RIGHT = 8
};

class Sprite
{
protected:
	SDL_RendererFlip Flip;
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

	double MoveRate;
	double MaxVelocity;
	double TransitionSpeed;
	double StopSpeed;
	
	int MovingFlags;

public:
	Sprite();
	virtual void Tick(double DeltaTime);
	virtual void TickAnimation(double DeltaTime);
	virtual void HandleInput(double DeltaTime);
	void SetTexture(SDL_Texture* Texture);
	void SetPosition(int NewX, int NewY);
	void SetWidth(int NewWidth);
	void SetHeight(int NewHeight);
	void Render(SDL_Renderer* Renderer);
	void PlayAnimation(AnimationResource *Anim);
	void SetAnimationPlayRate(double Rate);
	void UpdateAnimationData();
	void StopAnimation();
};

#endif
