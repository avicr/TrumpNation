#ifndef ITEMSPRITE_H
#define ITEMSPRITE_H
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/SpriteList.h"
#include "../inc/ScoreSprite.h"

class ItemSprite : public Sprite
{
public:
	virtual ~ItemSprite() {};
	static int NumNonBrickItems;
	ItemSprite(SDL_Texture *InTexture);
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
public:
	BrickItem();	
	virtual void Interact(TrumpPlayerSprite *OtherSprite);
};

class RedHatItem : public ItemSprite
{
public:
	RedHatItem() :
		ItemSprite(ResourceManager::RedHatTexture->Texture)
	{
		NumNonBrickItems++;
	}

	virtual ~RedHatItem()
	{
		NumNonBrickItems--;
	}
	virtual void Interact(TrumpPlayerSprite *OtherSprite)
	{
		Mix_PlayChannel(-1, PickUpItemFX, 0);
		ThePlayer->PickupRedHat();
		bPendingDelete = true;
	}
};

class BombItem : public ItemSprite
{

public:
	BombItem() :
		ItemSprite(ResourceManager::BombTexture->Texture)
	{
		NumNonBrickItems++;
	}

	virtual ~BombItem()
	{
		NumNonBrickItems--;
	}
	virtual void Interact(TrumpPlayerSprite *OtherSprite)
	{
		int ScorePerMexican = MEXICAN_BLOCK_SCORE * (bSwapSprites ? 2 : 1);
		BombCountDown = BOMB_FLASH_TIME;
		Mix_PlayChannel(-1, TrumpDieFX, 0);		
		bPendingDelete = true;		

		for (int i = 0; i < Mexicans.size(); i++)
		{
			SDL_Rect MexiRect = Mexicans[i]->GetCollisionRect();

			Items.push_back(new ScoreSprite(MexiRect.x, MexiRect.y, ScorePerMexican));
		}

		ThePlayer->AddToScore(ScorePerMexican * Mexicans.size());
		Mexicans.DeleteAll();
	}
	
};

#endif
