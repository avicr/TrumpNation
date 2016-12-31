#ifndef ITEMSPRITE_H
#define ITEMSPRITE_H
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"

class ItemSprite : public Sprite
{
public:
	virtual void CheckCollision(TrumpPlayerSprite *OtherSprite);
	virtual void Interact(TrumpPlayerSprite *OtherSprite) {};
};

class BrickItem : public ItemSprite
{
public:
	BrickItem(int X, int Y);
	void Interact(TrumpPlayerSprite *OtherSprite);
};
#endif
