#include "../inc/Mexican1Sprite.h"

Mexican1Sprite::Mexican1Sprite()
{
	SetWidth(39);
	SetHeight(69);

	MoveRate = 444;
	MaxVelocity = 555;
	TransitionSpeed = 2;
	StopSpeed = 4;
}

void Mexican1Sprite::HandleInput(double DeltaTime)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_RETURN] || Rect.y > 600)
	{
		Rect.y = 0;
		PosX = rand() % 900;
		PosY = -70;
		VelX = 0;
		VelY = 0;
		MovingFlags = MOVING_DOWN;
	}

}