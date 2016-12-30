#include "../inc/Mexican1Sprite.h"

Mexican1Sprite::Mexican1Sprite()
{
	float scale = 0.58;
	SetWidth(128 * scale);
	SetHeight(136 * scale);
	
	MoveRate = 444;
	MaxVelocity = 222;
	TransitionSpeed = 2;
	StopSpeed = 4;
	PosX =  rand() % (1024 - Rect.w);
	PosY = HORIZON;
	Rect.x = PosX;
	Rect.y = PosY;
	VelX = 0;
	VelY = 0;
	Growth = 0;
	bClimbingWall = false;

	int WallIndex = (int)round(PosX / 64);
	if (WallArray[WallIndex])
	{
		SDL_Rect WallRect = { WallIndex * 64, WALL_TOP, 64, 160 };
		SDL_Rect CollisionRect = { Rect.x + 18, Rect.y - 22, Rect.w - 30, Rect.h };
		SDL_Rect ResultRect;
		bool bBehindWall = SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect);

		if (!SDL_RectEquals(&ResultRect, &CollisionRect) || !bBehindWall)
		{
			PosX = rand() % (1024 - Rect.w);
		}
		MoveRate = 333;
		bClimbingWall = true;
		PosX = WallIndex * 64;
	}

	MovingFlags = 0;
	PlayAnimation(ResourceManager::Mexican1Animation);
}

void Mexican1Sprite::Render(SDL_Renderer *Renderer)
{
	if (AnimData.Anim)
	{
		Frame *CurFrame = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex);
		if (CurFrame)
		{
			SDL_Rect SrcRect = CurFrame->GetSrcRect();

			if (bClimbingWall)
			{

			}

			SDL_Rect RenderRect = Rect;

			if (bClimbingWall)
			{
				RenderRect.h = Rect.h * Growth;
				SrcRect.h *= Growth;
			}
			else
			{
				RenderRect.w *= Growth;
				RenderRect.h *= Growth;
			}
			SDL_RenderCopyEx(Renderer, Texture, &SrcRect, &RenderRect, 0, NULL, Flip);
		}
	}
}

void Mexican1Sprite::HandleInput(double DeltaTime)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	const double GrowthRate = 0.5;

	if (state[SDL_SCANCODE_RETURN] || Rect.y > 600)
	{
		bPendingDelete = true;
		return;
		/*Rect.y = 0;
		PosX = rand() % 900;
		PosY = 264;
		VelX = 0;
		VelY = 0;
		Growth = 0;
		MovingFlags = 0;*/
	}

	if (PosY >= 280 && bClimbingWall)
	{
		MaxVelocity = 444;
		VelY = 0;
		MovingFlags = MOVING_DOWN;
		TransitionSpeed = 1;
		bClimbingWall = false;
	}
	if (Growth < 1)
	{
		Growth += DeltaTime * GrowthRate;

		if (Growth > 1)
		{
			Growth = 1;
			MovingFlags = MOVING_DOWN;
		}

		if (bClimbingWall)
		{
			PosY = WALL_TOP - Rect.h * Growth + 1;
			StopSpeed = 0.5;
			TransitionSpeed = 33;
			MaxVelocity = 500;
		}
		else
		{
			PosY = HORIZON - Rect.h * Growth + 1;
		}
	}
}
