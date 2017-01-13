#ifndef CLOUDSPRITE_H
#define CLOUDSPRITE_H

#include "../inc/Sprite.h"

class CloudSprite : public Sprite
{
protected:	
	double Speed;

public:
	CloudSprite();
	virtual void Tick(double DeltaTime);
};
#endif
