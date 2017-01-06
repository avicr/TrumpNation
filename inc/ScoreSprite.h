#ifndef SCORESPRITE_H
#define SCORESPRITE_H

#include "../inc/Sprite.h"

class ScoreSprite : public Sprite
{	
public:
	ScoreSprite(int X, int Y, int InScore);
	void Render(SDL_Renderer *Renderer);
	void Tick(double DeltaTime);

protected:
	int Score;
	double CountDown;
};
#endif
