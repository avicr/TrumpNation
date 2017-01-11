#include "../inc/TrumpPlayerSprite.h"
#include "../inc/Mexican1Sprite.h"
#include "../inc/ItemSprite.h"
#include "../inc/SpriteList.h"
#include "../inc/ScoreSprite.h"

TrumpPlayerSprite::TrumpPlayerSprite()
{
	memset(bButtonPreviouslyPressed, 0, sizeof(bButtonPreviouslyPressed));

	SetWidth(80);
	SetHeight(80);

	NumBombs = 2;
	NumLives = 2;
	MoveRate = 444;
	MaxVelocity = TRUMP_DEFAULT_MAX_VELOCITY;
	TransitionSpeed = 7;
	
	StopSpeed = 16;

	DyingCountDown = 2;
	PlayerState = StatePlaying;
	Score = 0;
	PosX = 445;
	PosY = 340;
	Joy = SDL_JoystickOpen(0);
	bHasWall = false;
	bHasRedHat = false;
	bPlayingStepFX = false;
}

void TrumpPlayerSprite::Tick(double DeltaTime)
{
	Sprite::Tick(DeltaTime);
	
	if (PlayerState == StateDying)
	{
		DyingCountDown -= DeltaTime;

		if (DyingCountDown <= 0)
		{
			bHasRedHat = false;
			MaxVelocity = TRUMP_DEFAULT_MAX_VELOCITY;
			DyingCountDown = 2;
			NumLives--;
			SDL_Log("Lives: %d", NumLives);
			TheGame->SetLevel(TheGame->GetLevelNumber());
			PlayerState = StateDead;
			NumBombs = 2;
		}
	}
	else if(bHasRedHat)
	{
		RedHatCountDown -= DeltaTime;

		if (RedHatCountDown <= 0)
		{
			MaxVelocity = TRUMP_DEFAULT_MAX_VELOCITY;			

			bHasRedHat = false;

			if (!bSwapSprites)
			{
				PlayAnimation(ResourceManager::TrumpAnimation);
			}
			else
			{
				PlayAnimation(ResourceManager::Mexican1Animation);
			}
		}
	}


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
	
}

void TrumpPlayerSprite::TickAnimation(double DeltaTime)
{
	if (PlayerState != StateDying && PlayerState != StateDead)
	{
		double Magnitude = sqrt(VelX * VelX + VelY * VelY);
		SetAnimationPlayRate(Magnitude / MaxVelocity * 1.65);

		if (fabs(VelX) <= StopSpeed * 2 && fabs(VelY) <= StopSpeed * 2)
		{
			AnimData.CurrentFrameIndex = 0;
			UpdateAnimationData();
		}
	}
	else
	{
		SetAnimationPlayRate(1);
	}

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

void TrumpPlayerSprite::AddToScore(int Amount)
{
	Score += Amount;
}

long TrumpPlayerSprite::GetScore()
{
	return Score;
}

void TrumpPlayerSprite::HandleInput(double DeltaTime)
{	
	if (PlayerState == StateDying || PlayerState == StateDead)
	{		
		Mix_HaltChannel(0);
		bPlayingStepFX = false;
		VelX = 0;
		VelY = 0;
		return;
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	SDL_JoystickUpdate();
	MovingFlags = MOVING_NONE;
	Sint16 Test = SDL_JoystickGetAxis(Joy, 1);
	bool bPlayerMoving = false;
	bPlayerMoving = false;


	/*if (Joy)
	{
		SDL_Log("Joy axis 2: %d", SDL_JoystickGetAxis(Joy, 2));
		SDL_Log("Joy axis 1: %d", SDL_JoystickGetAxis(Joy, 1));
		SDL_Log("Joy axis 0: %d", SDL_JoystickGetAxis(Joy, 0));
	}*/


	if (state[SDL_SCANCODE_KP_8] || state[SDL_SCANCODE_UP] || (Joy && SDL_JoystickGetAxis(Joy, JOY_Y_AXIS) < -JOYSTICK_DEAD_ZONE))
	{		
		MovingFlags |= MOVING_UP;
		bPlayerMoving = true;
	}
	else if (state[SDL_SCANCODE_KP_2] || state[SDL_SCANCODE_DOWN] || (Joy && SDL_JoystickGetAxis(Joy, JOY_Y_AXIS) > JOYSTICK_DEAD_ZONE))
	{
		MovingFlags |= MOVING_DOWN;
		bPlayerMoving = true;
	}
	else
	{
		VelY = VelY * (1 - DeltaTime * StopSpeed) + 0 * (DeltaTime * StopSpeed);		
	}
	
	if (state[SDL_SCANCODE_KP_6] || state[SDL_SCANCODE_RIGHT] || (Joy && SDL_JoystickGetAxis(Joy, 0) > JOYSTICK_DEAD_ZONE))
	{
		MovingFlags |= MOVING_RIGHT;
		bPlayerMoving = true;
	}
	else if (state[SDL_SCANCODE_KP_4] || state[SDL_SCANCODE_LEFT] || (Joy && SDL_JoystickGetAxis(Joy, 0) <= -JOYSTICK_DEAD_ZONE))
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
	}
	else if (!bPlayerMoving && bPlayingStepFX)
	{
		Mix_HaltChannel(0);
		bPlayingStepFX = false;
	}

	/*if (!bButtonPreviouslyPressed[1] && NumBombs > 0 && (state[SDL_SCANCODE_LALT] || (Joy && SDL_JoystickGetButton(Joy, 1))))
	{
		KillEverything();
		NumBombs--;
	}*/

	bool bPressingButton1 = state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_RETURN] || (Joy && SDL_JoystickGetButton(Joy, 0));
	if (!bButtonPreviouslyPressed[0] && bHasWall && bPressingButton1)
	{
		int TotalScore = PLACE_WALL_SCORE * (bSwapSprites ? 2 : 1);
		//bFreezeSpawn = true;
		
		if (PosY >= WALL_TOP + 100 && PosY <= WALL_TOP + WALL_PLACE_ZONE)
		{
			int WallIndex = (int)round((PosX - Rect.w / 2) / 128);

			if (TheGame->WallArray[WallIndex * 2] < 1)
			{				
				Mix_PlayChannel(-1, PlaceWallFX, 0);
				TheGame->WallArray[WallIndex * 2]++;

				if (TheGame->WallArray[WallIndex * 2] > 2)
				{
					TheGame->WallArray[WallIndex * 2] = 2;
				}
				TheGame->WallArray[WallIndex * 2 + 1] = TheGame->WallArray[WallIndex * 2];

				bHasWall = false;								
				Items.push_back(new BrickItem());

				for (int i = 0; i < Mexicans.size(); i++)
				{
					if (Mexicans[i]->HandleWallPlaced(WallIndex * 2) || Mexicans[i]->HandleWallPlaced(WallIndex * 2 + 1))
					{
						TotalScore += Mexicans[i]->GetScoreWorth();
					}					
				}				

				Items.push_back(new ScoreSprite(WallIndex * 128 + 42, WALL_TOP - 38, TotalScore));
				Score += TotalScore;
			}
		}
	}

	if (state[SDL_SCANCODE_LALT] || (Joy && SDL_JoystickGetButton(Joy, 1)))
	{
		bButtonPreviouslyPressed[1] = true;
	}
	else
	{
		bButtonPreviouslyPressed[1] = false;
	}

	if (bPressingButton1)
	{
		bButtonPreviouslyPressed[0] = true;
	}
	else
	{
		bButtonPreviouslyPressed[0] = false;
	}
}

int TrumpPlayerSprite::GetNumLives()
{
	return NumLives;
}

void TrumpPlayerSprite::SetNumLives(int Amount)
{
	NumLives = Amount;
}

void TrumpPlayerSprite::TakeDamage()
{
	PlayerState = StateDying;
	PlayAnimation(ResourceManager::TrumpDamageAnimation);
}

void TrumpPlayerSprite::Reset()
{	
	bHasWall = false;
	SetPosition(445, 340);
	PlayerState = StatePlaying;

	if (!bSwapSprites && !bHasRedHat)
	{
		PlayAnimation(ResourceManager::TrumpAnimation);
	}
	else if (bHasRedHat)
	{
		PlayAnimation(ResourceManager::TrumpRedHatAnimation);
	}
	else
	{
		PlayAnimation(ResourceManager::Mexican1Animation);
	}
}

ePlayerState TrumpPlayerSprite::GetPlayerState()
{
	return PlayerState;
}

void TrumpPlayerSprite::Render(SDL_Renderer *Renderer)
{
	if (bHasWall && PosY >= WALL_TOP + 100 && PosY <= WALL_TOP + WALL_PLACE_ZONE)
	{
		int WallIndex = (int)round((PosX - Rect.w/2) / 128);
		WallIndex *= 2;
		SDL_Rect DstRect = { WallIndex * 64, WALL_TOP,ResourceManager::WallTexture->SrcRect.w, ResourceManager::WallTexture->SrcRect.h };

		/*if (TheGame->WallArray[WallIndex] > 0)
		{
			DstRect.y -= 160;
		}*/

		SDL_SetTextureAlphaMod(ResourceManager::WallTexture->Texture, 64);
		SDL_RenderCopy(Renderer, ResourceManager::WallTexture->Texture, &ResourceManager::WallTexture->SrcRect, &DstRect);
		DstRect.x += 64;;
		SDL_RenderCopy(Renderer, ResourceManager::WallTexture->Texture, &ResourceManager::WallTexture->SrcRect, &DstRect);
		SDL_SetTextureAlphaMod(ResourceManager::WallTexture->Texture, 255);
	}

	Sprite::Render(Renderer);

	/*if (bHasWall)
	{
		SDL_Rect BrickDstRect = { PosX + 22, PosY + 35, ResourceManager::BrickTexture->SrcRect.w, ResourceManager::BrickTexture->SrcRect.h };
		SDL_RenderCopy(Renderer, ResourceManager::BrickTexture->Texture, &ResourceManager::BrickTexture->SrcRect, &BrickDstRect);
	}*/
	

}

void TrumpPlayerSprite::DoSwap(bool bSwap)
{
	if (bSwap)
	{
		RedHatCountDown = 0;
		Mix_PlayMusic(HatDanceMusic, 0);
		MaxVelocity = TRUMP_DEFAULT_MAX_VELOCITY;
		for (int i = 0; i < Mexicans.size(); i++)
		{
			Mexicans[i]->PlayAnimation(ResourceManager::TrumpAnimation);
		}

		ThePlayer->PlayAnimation(ResourceManager::Mexican1Animation);
	}
	else if (bSwapSprites)
	{
		Mix_FadeInMusic(BGMusic, -1, 500);
		for (int i = 0; i < Mexicans.size(); i++)
		{
			Mexicans[i]->PlayAnimation(ResourceManager::Mexican1Animation);
		}

		if (!bHasRedHat)
		{
			ThePlayer->PlayAnimation(ResourceManager::TrumpAnimation);
		}
		else
		{
			ThePlayer->PlayAnimation(ResourceManager::TrumpRedHatAnimation);
		}
	}
	bSwapSprites = bSwap;
}

void TrumpPlayerSprite::PickupRedHat()
{
	DoSwap(false);
	
	bHasRedHat = true;
	RedHatCountDown = RED_HAT_TIME;
	PlayAnimation(ResourceManager::TrumpRedHatAnimation);
	MaxVelocity = TRUMP_RED_HAT_MAX_VELOCITY;
}

void TrumpPlayerSprite::AddBombs(int NumToAdd)
{
	NumBombs += NumToAdd;
}

void TrumpPlayerSprite::KillEverything(bool bBecauseBomb)
{
	int ScorePerMexican = MEXICAN_BLOCK_SCORE * (bSwapSprites ? 2 : 1);
	BombCountDown = BOMB_FLASH_TIME;

	if (bBecauseBomb)
	{
		Mix_PlayChannel(-1, TrumpDieFX, 0);
	}
	bPendingDelete = true;

	for (int i = 0; i < Mexicans.size(); i++)
	{
		SDL_Rect MexiRect = Mexicans[i]->GetCollisionRect();

		Items.push_back(new ScoreSprite(MexiRect.x, MexiRect.y, Mexicans[i]->GetScoreWorth()));
		AddToScore(Mexicans[i]->GetScoreWorth());
	}
	
	Mexicans.DeleteAll();
}

int TrumpPlayerSprite::GetNumBombs()
{
	return NumBombs;
}