#include "../inc/ItemSprite.h"

int ItemSprite::NumNonBrickItems = 0;

ItemSprite::ItemSprite(SDL_Texture *InTexture)
{	
	SetTexture(InTexture);
	RandomizePosition();	
	CountDown = ITEM_LIFE_TIME;
}

ItemSprite::ItemSprite(int X, int Y) :
	ItemSprite(NULL)
{
	SetPosition(X, Y);	
}

void ItemSprite::CheckCollision(TrumpPlayerSprite *OtherSprite)
{	
	if (HitTest(OtherSprite))
	{
		Interact(OtherSprite);
	}
}

bool ItemSprite::HitTest(TrumpPlayerSprite *OtherSprite)
{
	SDL_Rect TrumpCollision = OtherSprite->GetCollisionRect();
	SDL_Rect ResultRect;

	return SDL_IntersectRect(&TrumpCollision, &Rect, &ResultRect);
}

void ItemSprite::RandomizePosition()
{
	do
	{
		SetPosition((rand() % (1024 - Rect.w)), (rand() % ((600 - (HORIZON + 65)) - Rect.h)) + (HORIZON) + 65);
	} while (HitTest(ThePlayer));

	
}

void ItemSprite::Render(SDL_Renderer *Renderer)
{
	if (CountDown <= ITEM_LIFE_TIME * 0.25 && (int)round(CountDown * 100) % 8 >= 4)
	{

	}
	else
	{
		Sprite::Render(Renderer);
	}
}

void ItemSprite::Tick(double DeltaTime)
{
	if (CountDown > -1)
	{
		CountDown -= DeltaTime;

		if (CountDown <= 0)
		{
			bPendingDelete = true;
		}
	}
}

BrickItem::BrickItem()
	: ItemSprite(ResourceManager::BrickTexture->Texture)
{		
	CountDown = -1;
}

void BrickItem::Interact(TrumpPlayerSprite *OtherSprite)
{
	OtherSprite->SetHasWall(true);
	Mix_PlayChannel(-1, PickUpItemFX, 0);
	bPendingDelete = true;
}