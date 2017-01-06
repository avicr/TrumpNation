#ifndef MEXICAN1SPRITE_H
#define MEXICAN1SPRITE_H

#include "../inc/Sprite.h"

class Mexican1Sprite : public Sprite
{
public:
	Mexican1Sprite();
	bool HandleWallPlaced(int WallIndex);
	SDL_Rect GetCollisionRect();
	void Render(SDL_Renderer *Renderer);
	void CheckCollision(TrumpPlayerSprite *OtherSprite);
	void Interact(TrumpPlayerSprite *OtherSprite);

protected:
	double Growth;
	bool bClimbingWall;
	void HandleInput(double DeltaTime);
};

#endif