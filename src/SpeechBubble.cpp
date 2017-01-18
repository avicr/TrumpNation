#include "../inc/SpeechBubble.h"

SpeechBubble::SpeechBubble(int SpeechBubble, string InPhrase)
{
	if (SpeechBubble == 1)
	{
		SetTexture(ResourceManager::SpeechBubble1->Texture);
	}
	else if (SpeechBubble == 2)
	{
		SetTexture(ResourceManager::SpeechBubble2->Texture);
	}

	Phrase = InPhrase;

	CountDown = 1.5;

	Mix_PlayChannel(CHAN_TALK, TalkFX, 0);
}

void SpeechBubble::Tick(double DeltaTime)
{
	CountDown -= DeltaTime;

	if (CountDown <= 0)
	{
		bPendingDelete = true;
	}
	
}

void SpeechBubble::Render(SDL_Renderer *Renderer)
{
	Sprite::Render(GRenderer);	
	int OffsetY = 16;

	if (Phrase.find('\n') == -1)
	{
		OffsetY += 8;
	}
	DrawText(Phrase, Rect.x + 16, Rect.y + OffsetY, 16, 16, GRenderer, FontBlue, 1, 1);
}