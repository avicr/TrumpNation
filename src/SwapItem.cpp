#include "../inc/SwapItem.h"
#include "../inc/SpriteList.h"

void SwapItem::Interact(TrumpPlayerSprite *OtherSprite)
{
	bSwapSprites = true;	
	bPendingDelete = true;
	TheGame->DoSwap();
}

SwapItem::SwapItem() :
	ItemSprite(ResourceManager::HatTexture->Texture)
{	
}