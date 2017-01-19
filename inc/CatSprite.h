#ifndef CATSPRITE_H
#define CATSPRITE_H

#include "Sprite.h"
#include "Mexican1Sprite.h"

class CatSprite : public Mexican1Sprite
{
protected:

public:
	CatSprite();
	virtual void Interact(TrumpPlayerSprite *OtherSprite);
};

#endif