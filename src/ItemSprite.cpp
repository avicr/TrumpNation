#include "../inc/ItemSprite.h"

void ItemSprite::CheckCollision(TrumpPlayerSprite *OtherSprite)
{
	SDL_Rect TrumpCollision = OtherSprite->GetCollisionRect();
	SDL_Rect ResultRect;

	if (SDL_IntersectRect(&TrumpCollision, &Rect, &ResultRect))
	{
		Interact(OtherSprite);
	}
}

BrickItem::BrickItem(int X, int Y)
{
	SetPosition(X, Y);
	SetTexture(ResourceManager::BrickTexture->Texture);
}

void BrickItem::Interact(TrumpPlayerSprite *OtherSprite)
{
	OtherSprite->SetHasWall(true);
	bPendingDelete = true;
}