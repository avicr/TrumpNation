#include "../inc/Sprite.h"

Sprite::Sprite() :
	CurrentFrame(NULL)
{
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = 0;
	Rect.h = 0;
}

void Sprite::SetFrame(SDL_Texture* Texture)
{
	CurrentFrame = Texture;

	if (CurrentFrame != NULL)
	{
		Uint32 Format;
		int Access;

		SDL_QueryTexture(CurrentFrame, &Format, &Access, &Rect.w, &Rect.h);
	}
}

void Sprite::SetPosition(int NewX, int NewY)
{
	Rect.x = NewX;
	Rect.y = NewY;
}

void Sprite::Render(SDL_Renderer* Renderer)
{
	SDL_RenderCopy(Renderer, CurrentFrame, NULL, &Rect);
}
