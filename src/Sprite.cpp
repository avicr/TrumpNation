#include "../inc/Sprite.h"

const double Sprite::MoveRate = 444;
const double MaxVelocity = 333;

Sprite::Sprite() :
	Texture(NULL)
{	
	Flip = SDL_FLIP_NONE;
	Rect.x = 0;
	Rect.y = 0;
	/*Rect.w = 80;
	Rect.h = 80;*/
	Rect.w = 48;
	Rect.h = 69;
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
	const double transition_speed = 7;
	const double StopSpeed = 16;

	if (DeltaTime <= 0)
	{
		SDL_Log("DELTA TIME SUCKS");
	}
	if (state[SDL_SCANCODE_UP]) 
	{
		VelY = VelY * (1 - DeltaTime * transition_speed) + -MaxVelocity * (DeltaTime * transition_speed);
	}
	else if (state[SDL_SCANCODE_DOWN])
	{
		VelY = VelY * (1 - DeltaTime * transition_speed) + MaxVelocity * (DeltaTime * transition_speed);
	}
	else
	{
		VelY = VelY * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}
	
	if (state[SDL_SCANCODE_RIGHT])
	{
		VelX = VelX * (1 - DeltaTime * transition_speed) + MaxVelocity * (DeltaTime * transition_speed);

		if (abs(VelX) > MaxVelocity * 0.50)
		{
			Flip = SDL_FLIP_NONE;
		}
	}
	else if (state[SDL_SCANCODE_LEFT])
	{
		VelX = VelX * (1 - DeltaTime * transition_speed) + -MaxVelocity * (DeltaTime * transition_speed);
		
		if (abs(VelX) > MaxVelocity * 0.50)
		{
			Flip = SDL_FLIP_HORIZONTAL;
		}
	}
	else
	{
		VelX = VelX * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}

	if (abs(VelX) <= StopSpeed*2 && abs(VelY) <= StopSpeed*2)
	{
		//VelX = 0;
		//VelY = 0;
		AnimData.CurrentFrameIndex = 0;
		Frame *CurFrame = AnimData.Anim->GetFrame(0);
		if (CurFrame)
		{
			AnimData.CountDown = CurFrame->GetFrameTime();
			Texture = CurFrame->GetTexture();
		}
	}

	if (abs(VelX) > MaxVelocity)
	{
		VelX = MaxVelocity;
	}

	if (VelY > MaxVelocity)
	{
		VelY = MaxVelocity;
	}

	/*SDL_Log("VelX: %f", VelX * DeltaTime);
	SDL_Log("VelX: %f", VelY * DeltaTime);*/
	PosX += VelX * DeltaTime;
	PosY += VelY * DeltaTime;

	double Magnitude = sqrt(VelX * VelX + VelY * VelY);
	SDL_Log("MAG %f", Magnitude);
	SetAnimationPlayRate(Magnitude / MaxVelocity * 1.75);

	Rect.x = round(PosX);
	Rect.y = round(PosY);
	if (AnimData.Anim)
	{
		AnimData.CountDown -= DeltaTime * AnimData.PlayRate;

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
	PosX = NewX;
	PosY = NewY;
}

void Sprite::Render(SDL_Renderer* Renderer)
{
	if (AnimData.Anim)
	{
		Frame *CurFrame = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex);
		if (CurFrame)
		{		
			SDL_RenderCopyEx(Renderer, Texture, &CurFrame->GetSrcRect(), &Rect, 0, NULL, Flip);
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

void Sprite::SetAnimationPlayRate(double Rate)
{
	AnimData.PlayRate = Rate;
}

void Sprite::StopAnimation()
{
	AnimData.Anim = NULL;
}