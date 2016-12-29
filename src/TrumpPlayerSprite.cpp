#include "../inc/TrumpPlayerSprite.h"

TrumpPlayerSprite::TrumpPlayerSprite()
{
	SetWidth(80);
	SetHeight(80);

	MoveRate = 444;
	MaxVelocity = 333;
	TransitionSpeed = 7;
	StopSpeed = 16;
}

void TrumpPlayerSprite::Tick(double DeltaTime)
{
	Sprite::Tick(DeltaTime);
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

void TrumpPlayerSprite::HandleInput(double DeltaTime)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	MovingFlags = MOVING_NONE;
	
	if (state[SDL_SCANCODE_UP])
	{
		MovingFlags |= MOVING_UP;
	}
	else if (state[SDL_SCANCODE_DOWN])
	{
		MovingFlags |= MOVING_DOWN;
	}
	else
	{
		VelY = VelY * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}

	if (state[SDL_SCANCODE_RIGHT])
	{
		MovingFlags |= MOVING_RIGHT;
	}
	else if (state[SDL_SCANCODE_LEFT])
	{
		MovingFlags |= MOVING_LEFT;
	}
	else
	{
		VelX = VelX * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);
	}
}