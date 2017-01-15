#include "../inc/SwapItem.h"
#include "../inc/SpriteList.h"

void SwapItem::Interact(TrumpPlayerSprite *OtherSprite)
{
	bPendingDelete = true;
	Mix_PlayChannel(-1, PickUpItemFX, 0);
	ThePlayer->DoSwap(true);
}

SwapItem::SwapItem() :
	ItemSprite(NULL)
{	
	PlayAnimation(ResourceManager::SombreroAnimation);
	SetWidth(80);
	SetHeight(80);
	NumNonBrickItems++;

	CollisionRect = { 16, 16, 40, 40 };
	RandomizePosition();
}