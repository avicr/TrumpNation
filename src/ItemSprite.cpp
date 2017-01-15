#include "../inc/ItemSprite.h"

int ItemSprite::NumNonBrickItems = 0;

ItemSprite::ItemSprite(SDL_Texture *InTexture)
	: Sprite()
{	
	CollisionRenderColor.a = 128;
	CollisionRenderColor.r = 255;
	CollisionRenderColor.g = 255;
	CollisionRenderColor.b = 0;

	SetTexture(InTexture);
	Rect.w *= GLOBAL_SCALE;
	Rect.h *= GLOBAL_SCALE;

	CollisionRect = Rect;
	CollisionRect.x = 0;
	CollisionRect.y = 0;

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
	SDL_Rect TrumpCollision = OtherSprite->GetScreenSpaceCollisionRect();
	SDL_Rect ResultRect;
	SDL_Rect CollisionToUse = GetScreenSpaceCollisionRect();

	return SDL_IntersectRect(&TrumpCollision, &CollisionToUse, &ResultRect);
}

void ItemSprite::RandomizePosition()
{	
	// Plus four for good measure 
	int BoundX = CollisionRect.w + CollisionRect.x + 4;
	int BoundY = CollisionRect.h + CollisionRect.y + 4;

	do
	{
		SetPosition((rand() % (1024 - BoundX)), (rand() % ((600 - (HORIZON + 65)) - BoundY)) + (HORIZON) + 65);
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
	Sprite::Tick(DeltaTime);
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
	CollisionRect = { -8, -8, 49, 34 };
	CountDown = -1;
}

void BrickItem::Interact(TrumpPlayerSprite *OtherSprite)
{
	OtherSprite->SetHasWall(true);
	Mix_PlayChannel(-1, PickUpItemFX, 0);
	bPendingDelete = true;
}