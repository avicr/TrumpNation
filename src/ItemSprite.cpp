#include "../inc/ItemSprite.h"

ItemSprite::ItemSprite(SDL_Texture *InTexture)
{	
	SetTexture(InTexture);
	RandomizePosition();	
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

BrickItem::BrickItem()
	: ItemSprite(ResourceManager::BrickTexture->Texture)
{		
}

void BrickItem::Interact(TrumpPlayerSprite *OtherSprite)
{
	OtherSprite->SetHasWall(true);
	Mix_PlayChannel(-1, PickUpItemFX, 0);
	bPendingDelete = true;
}