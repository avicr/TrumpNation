#ifndef TRUMPPLAYERSPRITE_H
#define TRUMPPLAYERSPRITE_H

#include "../inc/Sprite.h"
#include "../inc/SpeechBubble.h"
#include <deque>

enum ePlayerState
{
	StateWaiting = 0,
	StatePlaying,
	StateDying,
	StateDead
};

enum eBrickType
{
	BrickRegular = 0,
	BrickGold
};
class TrumpPlayerSprite : public Sprite
{
public:
	TrumpPlayerSprite();
	virtual void Tick(double DeltaTime);
	virtual void TickAnimation(double DeltaTime);
	SDL_Rect GetScreenSpaceCollisionRect();
	void AddBrick(eBrickType BrickType);
	int GetNumBricks();
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
	deque <eBrickType> GetBrickInvetory();
	bool HasRedHat();
	void Say(int SpeechType, string Phrase);

protected:
	SpeechBubble *CurrentSpeech;
	deque <eBrickType> BrickInventory;
	bool bButtonPreviouslyPressed[3];
	ePlayerState PlayerState;
	int NumBombs;
	double DyingCountDown;
	int NumLives;
	long Score;
	int StepChannel;	
	int NumBricks;
	bool bHasRedHat;
	double RedHatCountDown;
	SDL_Joystick *Joy;

	void HandleInput(double DeltaTime);
};

#endif