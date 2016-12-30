#ifndef MEXICAN1SPRITE_H
#define MEXICAN1SPRITE_H

#include "../inc/Sprite.h"

class Mexican1Sprite : public Sprite
{
public:
	Mexican1Sprite();
	void Render(SDL_Renderer *Renderer);

protected:
	double Growth;
	bool bClimbingWall;
	void HandleInput(double DeltaTime);
};

#endif