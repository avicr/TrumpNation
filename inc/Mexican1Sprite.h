#ifndef MEXICAN1SPRITE_H
#define MEXICAN1SPRITE_H

#include "../inc/Sprite.h"

class Mexican1Sprite : public Sprite
{
public:
	Mexican1Sprite();

protected:
	double Growth;
	void HandleInput(double DeltaTime);
};

#endif