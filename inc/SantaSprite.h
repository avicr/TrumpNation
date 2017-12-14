#ifndef SANTASPRITE_H
#define SANTASPRITE_H
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/SpriteList.h"
#include "../inc/ScoreSprite.h"
#include "../inc/ItemSprite.h"

class SantaSprite : public ItemSprite
{
protected:
	float Timer;
public:

	SantaSprite() :
		ItemSprite(NULL)
	{
		Timer = 0;
		PlayAnimation(ResourceManager::SantaAnimation);
		SetWidth(80);
		SetHeight(80);
		NumNonBrickItems++;
		
		CollisionRect = { 16, 16, 48, 48 };		

		// Plus four for good measure 
		int BoundX = CollisionRect.w + CollisionRect.x + 4;
		int BoundY = CollisionRect.h + CollisionRect.y + 4;

		do
		{
			SetPosition((rand() % (1024 - BoundX)), (rand() % ((600 - (HORIZON + 65)) - BoundY)) + (HORIZON)+65);
		} while (HitTest(ThePlayer));
		VelX = 6;
		TransitionSpeed = 15;
		StopSpeed = 2;
		MoveRate = 60;
		MaxVelocity = 0;

		if (ThePlayer && ThePlayer->GetPosX() < PosX)
		{			
			Flip = SDL_FLIP_HORIZONTAL;
		}		
	}
	void Tick(double DeltaTime)
	{
		float DelayTime = 1.25;

		if (bChristmasMode)
		{
			DelayTime = 0.8;
		}
		ItemSprite::Tick(DeltaTime);
		Timer += DeltaTime;
		if (MaxVelocity == 0)
		{
			if (ThePlayer && ThePlayer->GetPosX() < PosX)
			{
				Flip = SDL_FLIP_HORIZONTAL;
			}
			else
			{
				Flip = SDL_FLIP_NONE;
			}
		}

		if (Timer > DelayTime && MaxVelocity == 0)
		{
			if (Flip == SDL_FLIP_HORIZONTAL)
			{
				MovingFlags = MOVING_RIGHT;
			}
			else
			{
				MovingFlags = MOVING_LEFT;
			}
			MaxVelocity = 350;
		}
	}
	~SantaSprite()
	{
		NumNonBrickItems--;
	}
	virtual void Interact(TrumpPlayerSprite *OtherSprite)
	{
		int Score = 1500 * TheGame->GetLevelNumber();
		Score *= bSwapSprites ? 2 : 1;

		DecoSprites.insert(DecoSprites.begin(), new ScoreSprite(Rect.x, Rect.y - 74, Score));
		OtherSprite->AddToScore(Score);

		Mix_PlayChannel(CHAN_ITEM_PICKUP, PickUpItemFX, 0);
		bPendingDelete = true;
	}
};

#endif