#ifndef MEXICAN1SPRITE_H
#define MEXICAN1SPRITE_H

#include "../inc/Sprite.h"

class Mexican1Sprite : public Sprite
{
public:
	Mexican1Sprite();
	~Mexican1Sprite();
	bool HandleWallPlaced(int WallIndex);
	SDL_Rect GetScreenSpaceCollisionRect();
	void Render(SDL_Renderer *Renderer);
	void CheckCollision(TrumpPlayerSprite *OtherSprite);
	void Interact(TrumpPlayerSprite *OtherSprite);
	int GetScoreWorth();
	void StopSounds();

protected:
	bool bReachedMaxGrowth;
	float AttackCountDown;
	float JumpCountDown;
	bool bIsJumping;
	bool bHasPlayedSpawnSound;
	static int NumClimbingSoundsPlaying;
	int ClimbChannel;
	double Growth;
	bool bClimbingWall;
	void HandleInput(double DeltaTime);
};

#endif