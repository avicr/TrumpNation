#include "../inc/TrumpPlayerSprite.h"
#include "../inc/Mexican1Sprite.h"
#include "../inc/ItemSprite.h"
#include "../inc/SpriteList.h"
#include "../inc/ScoreSprite.h"

TrumpPlayerSprite::TrumpPlayerSprite()
{
	CurrentSpeech = NULL;
	CollisionRenderColor.a = 128;
	CollisionRenderColor.r = 0;
	CollisionRenderColor.g = 255;
	CollisionRenderColor.b = 0;

	memset(bButtonPreviouslyPressed, 0, sizeof(bButtonPreviouslyPressed));

	SetWidth(32 * GLOBAL_SCALE);
	SetHeight(32 * GLOBAL_SCALE);

	NumBombs = 2;
	NumLives = 2;
	MoveRate = 444;
	MaxVelocity = TRUMP_DEFAULT_MAX_VELOCITY;
	TransitionSpeed = 7;
	
	StopSpeed = 16;

	DyingCountDown = 2;
	PlayerState = StatePlaying;
	Score = 0;
	PosX = 500;
	PosY = 340;	
	NumBricks = 0;
	bHasRedHat = false;	
	StepChannel = -1;

	CollisionRect = {18, 30, 32, 39 };
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
			NumBricks = 0;
			BrickInventory.clear();
			NumBombs = 2;
		}
	}
	else if(bHasRedHat)
	{
		RedHatCountDown -= DeltaTime;

		if (RedHatCountDown < 1 && (int)round(RedHatCountDown * 1000) % 40 >= 20)
		{
			int FrameIndex = AnimData.CurrentFrameIndex;
			double FrameCountDown = AnimData.CountDown;
			PlayAnimation(ResourceManager::TrumpAnimation);
			AnimData.CurrentFrameIndex = FrameIndex;
			AnimData.CountDown = FrameCountDown;
		}
		else
		{
			int FrameIndex = AnimData.CurrentFrameIndex;
			double FrameCountDown = AnimData.CountDown;
			PlayAnimation(ResourceManager::TrumpRedHatAnimation);
			AnimData.CurrentFrameIndex = FrameIndex;
			AnimData.CountDown = FrameCountDown;
		}

		if (RedHatCountDown <= 0)
		{
			Mix_PlayMusic(BGMusic, -1);
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

	if (PosY < WALL_TOP + 64)
	{
		PosY = WALL_TOP + 64;
	}

	if (PosX + Rect.w > 320)
	{
		PosX = 320 - Rect.w;
	}

	if (PosY + Rect.h > 240)
	{
		PosY = 240 - Rect.h;
	}

	if (CurrentSpeech && CurrentSpeech->GetPendingDelete())
	{
		delete CurrentSpeech;

		CurrentSpeech = NULL;
	}
	else if (CurrentSpeech)
	{
		CurrentSpeech->Tick(DeltaTime);
		/*if (Flip == SDL_FLIP_HORIZONTAL)
		{
			CurrentSpeech->SetPosition(PosX - 23, PosY - 70);
		}
		else*/
		{
			CurrentSpeech->SetPosition(PosX + 23, PosY - 70);
		}
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

SDL_Rect TrumpPlayerSprite::GetScreenSpaceCollisionRect()
{
	SDL_Rect CollisionRect = { Rect.x + 20, Rect.y + 27, 32, 43 };
	return CollisionRect;
}

void TrumpPlayerSprite::AddBrick(eBrickType BrickType)
{
	SDL_Log("Adding brick %d", BrickType);
	NumBricks++;
	BrickInventory.push_back(BrickType);
}

int TrumpPlayerSprite::GetNumBricks()
{
	return NumBricks;
}

void TrumpPlayerSprite::AddToScore(int Amount)
{
	Score += Amount;
}

void TrumpPlayerSprite::AddLives(int Amount)
{
	NumLives+= Amount;
}

long TrumpPlayerSprite::GetScore()
{
	return Score;
}

void TrumpPlayerSprite::HandleInput(double DeltaTime)
{	
	if (PlayerState == StateDying || PlayerState == StateDead)
	{		
		if (StepChannel != -1)
		{
			Mix_HaltChannel(StepChannel);
			StepChannel = -1;
		}		
		
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

	if (bPlayerMoving && StepChannel == -1)
	{
		StepChannel = Mix_PlayChannel(-1, StepFX, -1);		
	}
	else if (!bPlayerMoving && StepChannel != -1)
	{
		Mix_HaltChannel(StepChannel);
		StepChannel = -1;		
	}

	/*if (!bButtonPreviouslyPressed[1] && NumBombs > 0 && (state[SDL_SCANCODE_LALT] || (Joy && SDL_JoystickGetButton(Joy, 1))))
	{
		KillEverything();
		NumBombs--;
	}*/

	bool bPressingButton1 = state[SDL_SCANCODE_SPACE] || state[SDL_SCANCODE_RETURN] || (Joy && SDL_JoystickGetButton(Joy, 0));
	if (!bButtonPreviouslyPressed[0] && NumBricks && bPressingButton1)
	{
		int TotalScore = PLACE_WALL_SCORE * (bSwapSprites ? 2 : 1);

		if (BrickInventory.front() == BrickGold)
		{
			TotalScore = PLACE_GOLD_WALL_SCORE * (bSwapSprites ? 2 : 1);
		}
		//bFreezeSpawn = true;
		
		if (PosY >= WALL_TOP + 100 && PosY <= WALL_TOP + WALL_PLACE_ZONE)
		{
			int WallIndex = (int)round((PosX - Rect.w / 2) / 20);

			if (TheGame->WallArray[WallIndex * 2] < 1)
			{	
				Mix_PlayChannel(-1, PlaceWallFX, 0);
				TheGame->WallArray[WallIndex * 2] = BrickInventory.front() + 1;

				if (TheGame->WallArray[WallIndex * 2] > 2)
				{
					TheGame->WallArray[WallIndex * 2] = 2;
				}
				TheGame->WallArray[WallIndex * 2 + 1] = TheGame->WallArray[WallIndex * 2];

				BrickInventory.pop_front();
				NumBricks--;
				
				// If there are no bricks left on the field spawn a new one
				if (NumBricks == 0 && Items.size() - ItemSprite::NumNonBrickItems == 0)
				{
					BrickSpawnCountDown -= BRICK_FORCE_TIME_BONUS;

					if (BrickSpawnCountDown < 0.10)
					{
						BrickSpawnCountDown = 0.10;
					}
				}

				int NumMexicansKilled = 0;
				for (int i = 0; i < Mexicans.size(); i++)
				{
					if (Mexicans[i]->HandleWallPlaced(WallIndex * 2) || Mexicans[i]->HandleWallPlaced(WallIndex * 2 + 1))
					{
						NumMexicansKilled++;
						TotalScore += Mexicans[i]->GetScoreWorth();
					}					
				}				

				if (NumMexicansKilled >= 4)
				{
					if (!bSwapSprites)
					{
						ThePlayer->Say(2, "GO BACK TO\nUNIVISION!");
					}
					else
					{
						ThePlayer->Say(2, "REGRESA A\nUNIVISION!");
					}
				}

				DecoSprites.insert(DecoSprites.begin(), new ScoreSprite(WallIndex * 128 + 42, WALL_TOP - 38, TotalScore));
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
	int Pick = rand() % 3;
	
	if ( Pick == 0)
	{
		ThePlayer->Say(2, "  RIGGED");
	}
	else if (Pick == 1)
	{
		ThePlayer->Say(2, "  UNFAIR");
	}
	else if (Pick == 2)
	{
		ThePlayer->Say(2, " NOT NICE");
	}

	Mix_HaltChannel(-1);
	
	if (!bHasRedHat)
	{
		PlayAnimation(ResourceManager::TrumpDamageAnimation);
	}
	else
	{
		PlayAnimation(ResourceManager::TrumpDamageHatAnimation);
	}
}

void TrumpPlayerSprite::Reset()
{	
	//NumBricks = 0; //MAX_BRICKS - 1;
	SetPosition(470, 340);
	PlayerState = StatePlaying;

	if (CurrentSpeech)
	{
		delete CurrentSpeech;
		CurrentSpeech = NULL;
	}

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
	if (NumBricks && PosY >= WALL_TOP + 100 && PosY <= WALL_TOP + WALL_PLACE_ZONE)
	{
		int WallIndex = (int)round((PosX - Rect.w/2) / 128);
		TextureResource *TextureToUse = ResourceManager::WallTexture;

		if (BrickInventory.front() == BrickGold)
		{
			TextureToUse = ResourceManager::WallGoldTexture;
		}

		WallIndex *= 2;
		SDL_Rect DstRect = { WallIndex * 64, WALL_TOP, TextureToUse->SrcRect.w, TextureToUse->SrcRect.h };

		/*if (TheGame->WallArray[WallIndex] > 0)
		{
			DstRect.y -= 160;
		}*/
		
		
		SDL_SetTextureAlphaMod(TextureToUse->Texture, 64);
		SDL_RenderCopy(Renderer, TextureToUse->Texture, &TextureToUse->SrcRect, &DstRect);
		DstRect.x += 64;
		SDL_RenderCopy(Renderer, TextureToUse->Texture, &TextureToUse->SrcRect, &DstRect);
		SDL_SetTextureAlphaMod(TextureToUse->Texture, 255);
	}

	Sprite::Render(Renderer);
	if (CurrentSpeech)
	{
		CurrentSpeech->Render(Renderer);
	}
	/*SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(Renderer, &GetScreenSpaceCollisionRect());*/

	/*if (NumBricks)
	{
		SDL_Rect BrickDstRect = { PosX + 22, PosY + 35, ResourceManager::BrickTexture->SrcRect.w, ResourceManager::BrickTexture->SrcRect.h };
		SDL_RenderCopy(Renderer, ResourceManager::BrickTexture->Texture, &ResourceManager::BrickTexture->SrcRect, &BrickDstRect);
	}*/
	

}

void TrumpPlayerSprite::DoSwap(bool bSwap)
{
	bSwapSprites = bSwap;
	if (bSwap)
	{
		RedHatCountDown = 0;
		MaxVelocity = TRUMP_DEFAULT_MAX_VELOCITY;
		bHasRedHat = false;
		Mix_PlayMusic(HatDanceMusic, 0);
		MaxVelocity = TRUMP_DEFAULT_MAX_VELOCITY;
		for (int i = 0; i < Mexicans.size(); i++)
		{
			Mexicans[i]->PlayAnimation(ResourceManager::TrumpAnimation);
		}

		if (PlayerState != StateDying)
		{
			Say(2, "  I LOVE\nHISPANICS!");
			ThePlayer->PlayAnimation(ResourceManager::Mexican1Animation);
		}
	}
	else 
	{
		if (!bHasRedHat)
		{
			Mix_FadeInMusic(BGMusic, -1, 500);
		}
		else
		{
			Mix_PlayMusic(BGMusicFast, -1);
		}
		for (int i = 0; i < Mexicans.size(); i++)
		{
			Mexicans[i]->PlayAnimation(ResourceManager::Mexican1Animation);
		}

		if (PlayerState != StateDying)
		{
			if (!bHasRedHat)
			{
				ThePlayer->PlayAnimation(ResourceManager::TrumpAnimation);
			}
			else
			{
				ThePlayer->PlayAnimation(ResourceManager::TrumpRedHatAnimation);
			} 
		}		
	}	
}

void TrumpPlayerSprite::PickupRedHat()
{
	if (bSwapSprites)
	{
		DoSwap(false);
	}

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
		SDL_Rect MexiRect = Mexicans[i]->GetScreenSpaceCollisionRect();

		DecoSprites.insert(DecoSprites.begin(), new ScoreSprite(MexiRect.x, MexiRect.y, Mexicans[i]->GetScoreWorth()));
		AddToScore(Mexicans[i]->GetScoreWorth());
	}
	
	Mexicans.DeleteAll();
}

int TrumpPlayerSprite::GetNumBombs()
{
	return NumBombs;
}

deque <eBrickType> TrumpPlayerSprite::GetBrickInvetory()
{
	return BrickInventory;
}

bool TrumpPlayerSprite::HasRedHat()
{
	return bHasRedHat;
}

void TrumpPlayerSprite::Say(int SpeechType, string Phrase)
{
	if (CurrentSpeech)
	{
		delete CurrentSpeech;
	}	

	CurrentSpeech = new SpeechBubble(SpeechType, Phrase);
	CurrentSpeech->SetPosition(PosX + 23, PosY - 70);
}