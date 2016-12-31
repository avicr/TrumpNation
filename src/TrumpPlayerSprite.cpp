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
	
	if (state[SDL_SCANCODE_UP] || (Joy && SDL_JoystickGetAxis(Joy, 1) == -32768))
	{
		MovingFlags |= MOVING_UP;
	}
	else if (state[SDL_SCANCODE_DOWN] || (Joy && SDL_JoystickGetAxis(Joy, 1) == 32767))
	{
		MovingFlags |= MOVING_DOWN;
	}
	else
	{
		VelY = VelY * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}
	
	if (state[SDL_SCANCODE_RIGHT] || (Joy && SDL_JoystickGetAxis(Joy, 0) == 32767))
	{
		MovingFlags |= MOVING_RIGHT;
	}
	else if (state[SDL_SCANCODE_LEFT] || (Joy && SDL_JoystickGetAxis(Joy, 0) == -32768))
	{
		MovingFlags |= MOVING_LEFT;
	}
	else
	{
		VelX = VelX * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}

	if (bHasWall && (state[SDL_SCANCODE_RETURN] || (Joy && SDL_JoystickGetButton(Joy, 0))))
	{
		if (PosY >= WALL_TOP + 100 && PosY <= WALL_TOP + 130)
		{
			int WallIndex = (int)round(PosX / 64);

			if (!WallArray[WallIndex])
			{
				WallArray[WallIndex] = true;
				bHasWall = false;
				Items.push_back(new BrickItem(470, 570));

				for (int i = 0; i < Mexicans.size(); i++)
				{
					Mexicans[i]->HandleWallPlaced(WallIndex);
				}
			}
		}
	}
}