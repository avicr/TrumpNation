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
	PosX = rand() % (1024 - Rect.w);
	Rect.y = 0;
	PosY = 264;
	VelX = 0;
	VelY = 0;
	Growth = 0;
	bClimbingWall = false;

	int WallIndex = (int)round(PosX / 64);
	if (WallArray[WallIndex])
	{
		MoveRate = 333;
		PosY = 160;
		bClimbingWall = true;
		PosX = WallIndex * 64;
	}

	
	
	MovingFlags = 0;
	PlayAnimation(ResourceManager::Mexican1Animation);
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
			PosY = 158 - 128 * 0.58 * Growth;
			StopSpeed = 0.5;
			TransitionSpeed = 33;
			MaxVelocity = 500;
		}
		else
		{
			PosY = 264 - 128 * 0.58 * Growth;
			Rect.w = 128 * 0.58 * Growth;
			Rect.h = 136 * 0.58 * Growth;
		}
	}
}
