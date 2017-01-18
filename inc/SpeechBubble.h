#ifndef SPEECHBUBBLE_H
#define SPEECHBUBBLE_H
#include "../inc/Sprite.h"

class SpeechBubble : public Sprite
{
protected:
	string Phrase;

public:
	SpeechBubble(int SpeechBubbleType, string InPhrase);

	void Tick(double DeltaTime);
	void Render(SDL_Renderer *Renderer);
};

#endif
