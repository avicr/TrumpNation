#ifndef SPRITELIST_H
#define SPRITELIST_H

#include "../inc/Sprite.h"
#include <vector>

class SpriteList : public vector <Sprite*>
{
public:
	void Tick(double DeltaTime);
	void Render(SDL_Renderer *Renderer);
	void DeleteAll();
};

#endif

