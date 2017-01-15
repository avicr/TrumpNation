#ifndef SWAPITEM_H
#define SWAPITEM_H
#include "../inc/ItemSprite.h"


class SwapItem : public ItemSprite
{
public: 	
	SwapItem();
	virtual ~SwapItem() { NumNonBrickItems--; }
	virtual void Interact(TrumpPlayerSprite *OtherSprite);	
};
#endif