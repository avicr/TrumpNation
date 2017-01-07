#include "../inc/SwapItem.h"
#include "../inc/SpriteList.h"

void SwapItem::Interact(TrumpPlayerSprite *OtherSprite)
{
	bPendingDelete = true;
	Mix_PlayChannel(-1, PickUpItemFX, 0);
	ThePlayer->DoSwap(true);
}

SwapItem::SwapItem() :
	ItemSprite(ResourceManager::HatTexture->Texture)
{	
	NumNonBrickItems++;
}