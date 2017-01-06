#ifndef ITEMSPRITE_H
#define ITEMSPRITE_H
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"

class ItemSprite : public Sprite
{
public:
	ItemSprite(SDL_Texture *InTexture);
	ItemSprite(int X, int Y);
	virtual void CheckCollision(TrumpPlayerSprite *OtherSprite);
	virtual void Interact(TrumpPlayerSprite *OtherSprite) {};
	virtual void RandomizePosition();
	virtual bool HitTest(TrumpPlayerSprite *OtherSprite);
};

class BrickItem : public ItemSprite
{
public:
	BrickItem();	
	virtual void Interact(TrumpPlayerSprite *OtherSprite);
};
#endif
