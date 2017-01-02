#include "../inc/Sprite.h"

Sprite::Sprite() :
	Texture(NULL)
{	
	Flip = SDL_FLIP_NONE;
	Rect.x = 0;
	Rect.y = 0;
	Rect.w = 0;
	Rect.h = 0;
	
	VelX = 0;
	VelY = 0;
	AccelX = 0;
	AccelY = 0;
	PosX = 0;
	PosY = 0;

	MoveRate = 444;
	MaxVelocity = 333;
	TransitionSpeed = 7;
	StopSpeed = 16;
	bPendingDelete = false;

	MovingFlags = MOVING_NONE;
}

void Sprite::Tick(double DeltaTime)
{
	HandleInput(DeltaTime);
	if (MovingFlags & MOVING_UP)
	{
		VelY = VelY * (1 - DeltaTime * TransitionSpeed) + -MaxVelocity * (DeltaTime * TransitionSpeed);
	}
	else if (MovingFlags & MOVING_DOWN)
	{
		VelY = VelY * (1 - DeltaTime * TransitionSpeed) + MaxVelocity * (DeltaTime * TransitionSpeed);
	}
	else
	{
		VelY = VelY * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}

	if (MovingFlags & MOVING_RIGHT)
	{
		VelX = VelX * (1 - DeltaTime * TransitionSpeed) + MaxVelocity * (DeltaTime * TransitionSpeed);

		if (abs(VelX) > MaxVelocity * 0.350)
		{
			Flip = SDL_FLIP_NONE;
		}
	}
	else if (MovingFlags & MOVING_LEFT)
	{
		VelX = VelX * (1 - DeltaTime * TransitionSpeed) + -MaxVelocity * (DeltaTime * TransitionSpeed);

		if (abs(VelX) > MaxVelocity * 0.350)
		{
			Flip = SDL_FLIP_HORIZONTAL;
		}
	}
	else
	{
		VelX = VelX * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}

	if (abs(VelX) > MaxVelocity)
	{
		VelX = MaxVelocity;
	}

	if (abs(VelY) > MaxVelocity)
	{
		VelY = MaxVelocity;
	}

	PosX += VelX * DeltaTime;
	PosY += VelY * DeltaTime;

	Rect.x = round(PosX);
	Rect.y = round(PosY);

	TickAnimation(DeltaTime);
}

void Sprite::TickAnimation(double DeltaTime)
{
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

			UpdateAnimationData();
		}
	}
}

void Sprite::HandleInput(double DeltaTime)
{

}

void Sprite::CheckCollision(TrumpPlayerSprite *OtherSprite)
{

}
void Sprite::Interact(TrumpPlayerSprite *OtherSprite)
{
}

SDL_Rect Sprite::GetCollisionRect()
{
	return Rect;
}

void Sprite::UpdateAnimationData()
{
	Frame *CurFrame = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex);
	if (CurFrame)
	{
		AnimData.CountDown = CurFrame->GetFrameTime();
		Texture = CurFrame->GetTexture();
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

void Sprite::SetWidth(int NewWidth)
{
	Rect.w = NewWidth;
}

void Sprite::SetHeight(int NewHeight)
{
	Rect.h = NewHeight;
}

void Sprite::Render(SDL_Renderer* Renderer)
{
	SDL_Rect SrcRect = { 0, 0, Rect.w, Rect.h };
	if (AnimData.Anim)
	{
		Frame *CurFrame = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex);
		if (CurFrame)
		{
			SrcRect = CurFrame->GetSrcRect();
			/*SDL_Rect ShadowRect;
			SDL_Rect ShadowSrcRect = { 0, 0, 68, 51 };
			ShadowRect.x = Rect.x - (Rect.w * 0.6) * 0.5 + 40;
			ShadowRect.y = Rect.y + Rect.h - Rect.h * 0.15 + 6;
			ShadowRect.w = (double)Rect.w * 0.6;
			ShadowRect.h = (double)Rect.h * 0.15;

			SDL_RenderCopyEx(Renderer, ResourceManager::ShadowTexture->Texture, &ShadowSrcRect, &ShadowRect, 0, NULL, SDL_FLIP_NONE);*/
			SDL_RenderCopyEx(Renderer, Texture, &SrcRect, &Rect, 0, NULL, Flip);
		}		
	}
	else if (Texture)
	{
		SDL_RenderCopyEx(Renderer, Texture, &SrcRect, &Rect, 0, NULL, Flip);
	}
}

void Sprite::PlayAnimation(AnimationResource *Anim)
{
	if (Anim)
	{
		AnimData.Anim = Anim;
		AnimData.CurrentFrameIndex = 0;
		UpdateAnimationData();
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

bool Sprite::GetPendingDelete()
{
	return bPendingDelete;
}

void Sprite::Delete()
{
	bPendingDelete = true;
}