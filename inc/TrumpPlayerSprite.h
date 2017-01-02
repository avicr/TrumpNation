#ifndef TRUMPPLAYERSPRITE_H
#define TRUMPPLAYERSPRITE_H

#include "../inc/Sprite.h"

class TrumpPlayerSprite : public Sprite
{
public:
	TrumpPlayerSprite();
	virtual void Tick(double DeltaTime);
	virtual void TickAnimation(double DeltaTime);
	SDL_Rect GetCollisionRect();
	void SetHasWall(bool bInHasWall);
	void AddToScore(int Amount);
	int GetScore();

protected:
	int Score;
	bool bPlayingStepFX;
	bool bHasWall;
	SDL_Joystick *Joy;

	void HandleInput(double DeltaTime);
};

#endif