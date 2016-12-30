#ifndef TRUMPPLAYERSPRITE_H
#define TRUMPPLAYERSPRITE_H

#include "../inc/Sprite.h"

class TrumpPlayerSprite : public Sprite
{
public:
	TrumpPlayerSprite();
	virtual void Tick(double DeltaTime);
	virtual void TickAnimation(double DeltaTime);

protected:
	SDL_Joystick *Joy;

	void HandleInput(double DeltaTime);
};

#endif