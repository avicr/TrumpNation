#include "../inc/TrumpPlayerSprite.h"
#include "../inc/Mexican1Sprite.h"
#include "../inc/ItemSprite.h"

TrumpPlayerSprite::TrumpPlayerSprite()
{
	SetWidth(80);
	SetHeight(80);

	MoveRate = 444;
	MaxVelocity = 333;
	TransitionSpeed = 7;
	StopSpeed = 16;

	PosX = 445;
	PosY = 340;
	Joy = SDL_JoystickOpen(0);
	bHasWall = false;
	bPlayingStepFX = false;
}

void TrumpPlayerSprite::Tick(double DeltaTime)
{
	Sprite::Tick(DeltaTime);

	if (PosX < 0 - 20)
	{
		PosX = -20;
	}

	if (PosY < WALL_TOP + 100)
	{
		PosY = WALL_TOP + 100;
	}

	if (PosX + Rect.w > 1024)
	{
		PosX = 1024 - Rect.w;
	}

	if (PosY + Rect.h > 600)
	{
		PosY = 600 - Rect.h;
	}

	if (abs(VelX) <= StopSpeed * 2 && abs(VelY) <= StopSpeed * 2)
	{
		AnimData.CurrentFrameIndex = 0;
		UpdateAnimationData();
	}
}

void TrumpPlayerSprite::TickAnimation(double DeltaTime)
{
	double Magnitude = sqrt(VelX * VelX + VelY * VelY);
	SetAnimationPlayRate(Magnitude / MaxVelocity * 1.65);

	Sprite::TickAnimation(DeltaTime);
}

SDL_Rect TrumpPlayerSprite::GetCollisionRect()
{
	SDL_Rect CollisionRect = { Rect.x + 26, Rect.y + 32, 30, 45 };
	return CollisionRect;
}

void TrumpPlayerSprite::SetHasWall(bool bInHasWall)
{
	bHasWall = bInHasWall;
}

void TrumpPlayerSprite::HandleInput(double DeltaTime)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	SDL_JoystickUpdate();
	MovingFlags = MOVING_NONE;
	Sint16 Test = SDL_JoystickGetAxis(Joy, 1);
	bool bPlayerMoving = false;
	bPlayerMoving = false;

	if (state[SDL_SCANCODE_UP] || (Joy && SDL_JoystickGetAxis(Joy, 1) <= -32767))
	{
		MovingFlags |= MOVING_UP;
		bPlayerMoving = true;
	}
	else if (state[SDL_SCANCODE_DOWN] || (Joy && SDL_JoystickGetAxis(Joy, 1) == 32767))
	{
		MovingFlags |= MOVING_DOWN;
		bPlayerMoving = true;
	}
	else
	{
		VelY = VelY * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);		
	}
	
	if (state[SDL_SCANCODE_RIGHT] || (Joy && SDL_JoystickGetAxis(Joy, 0) == 32767))
	{
		MovingFlags |= MOVING_RIGHT;
		bPlayerMoving = true;
	}
	else if (state[SDL_SCANCODE_LEFT] || (Joy && SDL_JoystickGetAxis(Joy, 0) <= -32767))
	{
		MovingFlags |= MOVING_LEFT;
		bPlayerMoving = true;
	}
	else
	{
		VelX = VelX * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);		
	}

	if (bPlayerMoving && !bPlayingStepFX)
	{
		Mix_PlayChannel(0, StepFX, -1);
		bPlayingStepFX = true;
		SDL_Log("PLAYING OMG");
	}
	else if (!bPlayerMoving && bPlayingStepFX)
	{
		Mix_HaltChannel(0);
		bPlayingStepFX = false;
		SDL_Log("STOPING OMG");
	}

	if (bHasWall && (state[SDL_SCANCODE_RETURN] || (Joy && SDL_JoystickGetButton(Joy, 0))))
	{
		if (PosY >= WALL_TOP + 100 && PosY <= WALL_TOP + 130)
		{
			int WallIndex = (int)round(PosX / 128);

			if (!WallArray[WallIndex * 2])
			{
				Mix_PlayChannel(-1, PlaceWallFX, 0);
				WallArray[WallIndex * 2] = true;
				WallArray[WallIndex * 2 + 1] = true;

				bHasWall = false;
				Items.push_back(new BrickItem(rand() % 1024 - 32, (rand() % (200) + HORIZON + 65)));

				for (int i = 0; i < Mexicans.size(); i++)
				{
					Mexicans[i]->HandleWallPlaced(WallIndex * 2);
					Mexicans[i]->HandleWallPlaced(WallIndex * 2 + 1);
				}
			}
		}
	}
}