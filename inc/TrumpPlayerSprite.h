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
	SDL_Rect GetScreenSpaceCollisionRect();
	void SetHasWall(bool bInHasWall);
	void AddToScore(int Amount);
	void AddLives(int Amount);
	long GetScore();
	int GetNumLives();
	void SetNumLives(int Amount);
	void TakeDamage();
	void Reset();
	ePlayerState GetPlayerState();
	void PickupRedHat();
	void DoSwap(bool bSwap);
	void Render(SDL_Renderer *Renderer);
	void AddBombs(int NumToAdd);
	void KillEverything(bool bBecauseBomb = true);
	int GetNumBombs();

protected:
	bool bButtonPreviouslyPressed[3];
	ePlayerState PlayerState;
	int NumBombs;
	double DyingCountDown;
	int NumLives;
	long Score;
	bool bPlayingStepFX;
	bool bHasWall;
	bool bHasRedHat;
	double RedHatCountDown;
	SDL_Joystick *Joy;

	void HandleInput(double DeltaTime);
};

#endif