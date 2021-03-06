#ifndef ITEMSPRITE_H
#define ITEMSPRITE_H
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/SpriteList.h"
#include "../inc/ScoreSprite.h"

class ItemSprite : public Sprite
{
protected:
	double Growth;
	bool bReachedMaxSize;
public:
	ItemSprite(SDL_Texture *InTexture);
	virtual ~ItemSprite() {};
	static int NumNonBrickItems;	
	ItemSprite(int X, int Y);
	virtual void CheckCollision(TrumpPlayerSprite *OtherSprite);
	virtual void Interact(TrumpPlayerSprite *OtherSprite) {};
	virtual void RandomizePosition();
	virtual bool HitTest(TrumpPlayerSprite *OtherSprite);
	virtual void Render(SDL_Renderer *Renderer);
	virtual void Tick(double DeltaTime);
};

class BrickItem : public ItemSprite
{
protected:
	bool bIsFirstBrick;
public:
	BrickItem(bool bFirstBrick = false);	
	~BrickItem();
	virtual void Interact(TrumpPlayerSprite *OtherSprite);
};

class BrickGoldItem : public BrickItem
{
public:
	BrickGoldItem(bool bFirstBrick = false) : BrickItem(bFirstBrick)
	{
		SetTexture(ResourceManager::BrickGoldTexture->Texture);
		SetWidth(Rect.w *= GLOBAL_SCALE);
		SetHeight(Rect.h *= GLOBAL_SCALE);
	}
	virtual void Interact(TrumpPlayerSprite *OtherSprite)
	{
		SDL_Log("GOLD");
		OtherSprite->AddBrick(BrickGold);
		Mix_PlayChannel(CHAN_ITEM_PICKUP, PickUpItemFX, 0);
		bPendingDelete = true;
	}
};

class RedHatItem : public ItemSprite
{
public:
	RedHatItem() :
		ItemSprite(ResourceManager::RedHatTexture->Texture)
	{
		PlayAnimation(ResourceManager::RedHatAnimation);		
		
		SetWidth(80);
		SetHeight(80);
		NumNonBrickItems++;
				
		CollisionRect = { 16, 16, 48, 48 };
		RandomizePosition();
	}

	virtual ~RedHatItem()
	{
		NumNonBrickItems--;
	}
	virtual void Interact(TrumpPlayerSprite *OtherSprite)
	{
		if (!ThePlayer->HasRedHat())
		{
			Mix_PlayMusic(BGMusicFast, -1);
		}
		Mix_PlayChannel(CHAN_ITEM_PICKUP, PickUpItemFX, 0);		
		ThePlayer->PickupRedHat();
		bPendingDelete = true;
	}	
};

class BombItem : public ItemSprite
{

public:
	BombItem() :
		ItemSprite(NULL)
	{
		PlayAnimation(ResourceManager::BombAnimation);
		SetWidth(64);
		SetHeight(64);
		NumNonBrickItems++;

		CollisionRect = { 0, 0, 48, 48 };
		RandomizePosition();
	}

	virtual ~BombItem()
	{
		NumNonBrickItems--;
	}
	virtual void Interact(TrumpPlayerSprite *OtherSprite)
	{
		//Mix_PlayChannel(-1, PickUpItemFX, 0);
		bPendingDelete = true;
		//OtherSprite->AddBombs(1);
		OtherSprite->KillEverything();
	}	
	
};

class ExtraLifeItem : public ItemSprite
{
public:
	ExtraLifeItem() :
		ItemSprite(ResourceManager::TrumpFaceTexture->Texture)
	{
		CollisionRect = { -4, -4, 48, 48 };
		SetWidth(40);
		SetHeight(40);
		RandomizePosition();
		NumNonBrickItems++;
	}

	virtual ~ExtraLifeItem()
	{
		NumNonBrickItems--;
	}

	virtual void Interact(TrumpPlayerSprite *OtherSprite)
	{
		Mix_PlayChannel(CHAN_ITEM_PICKUP, PickUpItemFX, 0);
		ThePlayer->AddLives(1);
		bPendingDelete = true;
	}
};

#endif
