#include "../inc/Sprite.h"

const double Sprite::MoveRate = 400;
const double MaxVelocity = 255;

Sprite::Sprite() :
	Texture(NULL)
{	
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = 80;
	Rect.h = 80;

	VelX = 0;
	VelY = 0;
	AccelX = 0;
	AccelY = 0;

	PosX = 0;
	PosY = 0;
}

void Sprite::Tick(double DeltaTime)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	
	if (state[SDL_SCANCODE_UP]) 
	{
		VelY += DeltaTime * -MoveRate;

		if (VelY > MaxVelocity)
		{
			VelY = MaxVelocity;
		}
	}
	else if (state[SDL_SCANCODE_RIGHT])
	{
		VelX += DeltaTime * MoveRate;

		if (VelX > MaxVelocity)
		{
			VelX = MaxVelocity;
		}
	}
	else if (state[SDL_SCANCODE_LEFT])
	{
		VelX += DeltaTime * -MoveRate;

		if (VelX > MaxVelocity)
		{
			VelX = MaxVelocity;
		}
	}
	else if (state[SDL_SCANCODE_DOWN])
	{
		VelY += DeltaTime * MoveRate;

		if (VelY > MaxVelocity)
		{
			VelY = MaxVelocity;
		}
	}
	else
	{
		AnimData.CurrentFrameIndex = 0;
		Frame *CurFrame = AnimData.Anim->GetFrame(0);
		if (CurFrame)
		{
			AnimData.CountDown = CurFrame->GetFrameTime();
			Texture = CurFrame->GetTexture();
		}
	}

	Rect.x += VelX * DeltaTime;
	Rect.y += VelY * DeltaTime;

	if (AnimData.Anim)
	{
		AnimData.CountDown -= DeltaTime;

		if (AnimData.CountDown <= 0)
		{
			AnimData.CurrentFrameIndex++;
			if (AnimData.CurrentFrameIndex >= AnimData.Anim->GetFrameCount())
			{
				AnimData.CurrentFrameIndex = 0;
			}

			AnimData.CountDown = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex)->GetFrameTime();

			Frame *CurFrame = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex);
			if (CurFrame)
			{
				AnimData.CountDown = CurFrame->GetFrameTime();
				Texture = CurFrame->GetTexture();
				/*Rect.w = CurFrame->GetSrcRect().w;
				Rect.h = CurFrame->GetSrcRect().h;*/
			}
		}
	}
}

void Sprite::SetTexture(SDL_Texture* InTexture)
{
	Texture = InTexture;

	if (Texture != NULL)
	{
		Uint32 Format;
		int Access;

		SDL_QueryTexture(Texture, &Format, &Access, &Rect.w, &Rect.h);
	}
}

void Sprite::SetPosition(int NewX, int NewY)
{
	Rect.x = NewX;
	Rect.y = NewY;
}

void Sprite::Render(SDL_Renderer* Renderer)
{
	if (AnimData.Anim)
	{
		Frame *CurFrame = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex);
		if (CurFrame)
		{		
			SDL_RenderCopyEx(Renderer, Texture, &CurFrame->GetSrcRect(), &Rect, 0, NULL, SDL_FLIP_NONE);
		}		
	}
}

void Sprite::PlayAnimation(AnimationResource *Anim)
{
	if (Anim)
	{
		AnimData.Anim = Anim;
		AnimData.CurrentFrameIndex = 0;
		Frame *CurFrame = Anim->GetFrame(0);
		if (CurFrame)
		{
			AnimData.CountDown = CurFrame->GetFrameTime();
			Texture = CurFrame->GetTexture();
			//Rect.w = CurFrame->GetSrcRect().w;
			//Rect.h = CurFrame->GetSrcRect().h;
		}
	}
}

void Sprite::StopAnimation()
{
	AnimData.Anim = NULL;
}