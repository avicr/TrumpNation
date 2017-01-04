#ifndef TRUMPPLAYERSPRITE_H
#define TRUMPPLAYERSPRITE_H

#include "../inc/Sprite.h"

enum ePlayerState
{
	StateWaiting = 0,
	StatePlaying,
	StateDying,
	StateDead
};
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
	int GetNumLives();
	void SetNumLives(int Amount);
	void TakeDamage();
	void Reset();
	ePlayerState GetPlayerState();
	void Render(SDL_Renderer *Renderer);

protected:
	ePlayerState PlayerState;
	double DyingCountDown;
	int NumLives;
	int Score;
	bool bPlayingStepFX;
	bool bHasWall;
	SDL_Joystick *Joy;

	void HandleInput(double DeltaTime);
};

#endif