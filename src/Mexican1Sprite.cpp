#include "../inc/Mexican1Sprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/Globals.h"

int Mexican1Sprite::NumClimbingSoundsPlaying = 0;

Mexican1Sprite::Mexican1Sprite()
{
	GrowthRate = 0.5;
	bIsJumping = false;
	AttackCountDown = 0.5;
	JumpCountDown = 0.15;
	bHasPlayedSpawnSound = false;
	//float scale = 2.58;//0.58f;
	ClimbChannel = -1;
	SDL_Rect WallRect;
	SDL_Rect CollisionRect;
	SDL_Rect ResultRect;
	bool bBehindWall = false;
	bool bGood = false;
	int WallIndex = 0;
	MovingFlags = 0;

	CollisionRenderColor.a = 128;
	CollisionRenderColor.r = 255;
	CollisionRenderColor.g = 0;
	CollisionRenderColor.b = 0;

	SetWidth(int(32 * GLOBAL_SCALE));
	SetHeight(int(32 * GLOBAL_SCALE));
	
	MoveRate = 444;
	if (rand() % 100 == 101)
	{
		MaxVelocity = 120;
		if (!bSwapSprites)
		{
			
		}
		else
		{
			PlayAnimation(ResourceManager::TrumpAnimation);
		}

	}
	else
	{
		MaxVelocity = 222;
		if (!bSwapSprites)
		{
			PlayAnimation(ResourceManager::Mexican1Animation);
		}
		else
		{
			PlayAnimation(ResourceManager::TrumpAnimation);
		}
	}
	
	TransitionSpeed = 2;
	StopSpeed = 4;
	PosY = HORIZON;	
	VelX = 0;
	VelY = 0;
	Growth = 0;
	if (TheGame)
	{
		do
		{
			PosX = rand() % (1024 - Rect.w);
			bClimbingWall = false;
			CollisionRect = { 0, 0, 0, 0 };
			bBehindWall = false;
			Rect.x = (int)PosX;
			Rect.y = (int)PosY;
			bGood = false;

			WallIndex = (int)round(PosX / 64);
			if (TheGame->WallArray[WallIndex])
			{
				CollisionRect = { Rect.x + 18, Rect.y - 22, Rect.w - 30, Rect.h };
				bClimbingWall = true;
				WallRect = { WallIndex * 64, WALL_TOP, 64, 160 };
				bBehindWall = SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect);

				// Not behind wall, we are good
				if (!bBehindWall)
				{
					bGood = true;
				}
				// If we are totally covered by the first wall, we are good
				else if (SDL_RectEquals(&ResultRect, &CollisionRect))
				{
					bGood = true;
				}
				// If we are partially covered by the first wall, and there is a second wall next to it, we are fully covered and good
				else if ((WallIndex < 15 && TheGame->WallArray[WallIndex + 1]))
				{
					bGood = true;
				}
				else
				{
					bGood = false;
				}
			}
			// Not covered by first wall index, but are by second wall index
			else if (WallIndex < 15 && TheGame->WallArray[WallIndex + 1])
			{
				CollisionRect = { Rect.x + 18, Rect.y - 22, Rect.w - 30, Rect.h };
				WallRect = { (WallIndex + 1) * 64, WALL_TOP, 64, 160 };
				bBehindWall = SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect);

				if (!bBehindWall)
				{
					bGood = true;
				}
				else
				{
					PosX = (int)round(PosX / 64) * 64;
					bGood = true;
					bBehindWall = false;
				}
			}
			else
			{
				bGood = true;
			}
		} while (!bGood);

		// If we spawn with a wall to the left of us
		if (!bBehindWall)
		{
			CollisionRect = { Rect.x, Rect.y - 22, Rect.w, Rect.h };
			WallRect = { (WallIndex - 1) * 64, WALL_TOP, 64, 160 };

			if (SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect))
			{
				PosX = WallIndex * 64 + 5;
			}
		}

		if (bBehindWall)
		{
			if (TheGame->WallArray[WallIndex] > 1 || rand() % 4 != 0)
			{
				bPendingDelete = true;
				return;
			}

			bClimbingWall = true;
			//if (NumClimbingSoundsPlaying < MAX_CLIMBING_SOUNDS)
			{
				ClimbChannel = Mix_PlayChannel(CHAN_MEXICAN_CLIMB, MexicanClimbFX, -1);
				//Mix_Volume(ClimbChannel, 128 - 24 * NumClimbingSoundsPlaying);
				NumClimbingSoundsPlaying++;
			}
			MoveRate = 333;
			//PosX = WallIndex * 64;
		}
	}
	else
	{
		Growth = 1;

		//if (rand() % 2 == 0)
		{
			MovingFlags = MOVING_RIGHT;
			SetPosition(-425, 414);
			MaxVelocity = 50;
		}
		/*else
		{
			MovingFlags = MOVING_LEFT;
			SetPosition(1024, 300);
		}*/
	}
	
	
}

Mexican1Sprite::~Mexican1Sprite()
{
	StopSounds();
}

bool Mexican1Sprite::HandleWallPlaced(int WallIndex)
{
	SDL_Rect CollisionRect = { Rect.x + 10, Rect.y + 50, 35, 20 };
	SDL_Rect WallRect = { WallIndex * 64, WALL_TOP, 64, 160 };
	SDL_Rect ResultRect;
	
	/*if (PosY > WALL_TOP + 100)
	{
		return;
	}*/

	if (bPendingDelete)
	{
		return false;
	}

	if (SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect))
	{
		bPendingDelete = true;

		return true;
	}
	/*else if (WallIndex < 15)
	{
		CollisionRect = { Rect.x + 18, Rect.y + 60, 20, 20 };
		WallRect = { (WallIndex+1) * 64, WALL_TOP, 64, 160 };
		if (SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect))
		{
			bPendingDelete = true;
			return true;
		}
	}*/
	else
	{
		/*CollisionRect = { Rect.x + 10, Rect.y - 22, Rect.w, Rect.h };
		WallRect = { (WallIndex - 1) * 64, WALL_TOP, 64, 160 };

		if (SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect))
		{
			bPendingDelete = true;
		}*/
	}

	return false;
}

SDL_Rect Mexican1Sprite::GetScreenSpaceCollisionRect()
{
	SDL_Rect CollisionRect = { Rect.x + 20, Rect.y + 20, Rect.w - 40, Rect.h - 20 };

	return CollisionRect;
}

void Mexican1Sprite::Render(SDL_Renderer *Renderer)
{
	if (AnimData.Anim)
	{
		Frame *CurFrame = AnimData.Anim->GetFrame(AnimData.CurrentFrameIndex);
		if (CurFrame)
		{
			SDL_Rect SrcRect = CurFrame->GetSrcRect();

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
				if (Growth > 0.5)
				{
					//RenderRect.x += (Rect.w - RenderRect.w) / 2;
					//RenderRect.y += (Rect.h - RenderRect.h) / 2;
				}
				else
				{
					//RenderRect.x += (Rect.w - RenderRect.w) * Growth;
				}
			}
			SDL_RenderCopyEx(Renderer, Texture, &SrcRect, &RenderRect, 0, NULL, Flip);
		}
	}

	if (bRenderCollision)
	{
		RenderCollision(Renderer);
	}
	
}

void Mexican1Sprite::HandleInput(double DeltaTime)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	
	if (!TheGame)
	{		
		if (Rect.x >= 215 && Rect.y >= 200)
		{
			MovingFlags = MOVING_UP;
			MaxVelocity = 20;
			TransitionSpeed = 1;
		}
		else if (Rect.x <= 760 && Rect.y < 100)
		{
			MovingFlags = MOVING_RIGHT;
			MaxVelocity = 50;
			TransitionSpeed = 6;
		}
		else if (Rect.x >= 760 && Rect.y < 100)
		{
			MovingFlags = MOVING_DOWN;
			MaxVelocity = 2000;
			TransitionSpeed = 20;
		}

		/*if (Rect.x >= 240 && MovingFlags == MOVING_RIGHT)
		{
			bPendingDelete = true;
		}
		else if (Rect.x <= 770 && MovingFlags == MOVING_LEFT)
		{
			bPendingDelete = true;
		}*/
		return;
	}
	if (bDoSpawnPop && bHasPlayedSpawnSound)
	{
		GrowthRate *= 5.5;
	}

	if (Rect.y > 600)
	{
		if (TheGame)
		{
			TheGame->OnMexicanEscaped();
			bPendingDelete = true;
		}
		else
		{
			bPendingDelete = true;
		}
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
		TransitionSpeed = 0.5;
		MoveRate = 0.5;
		StopSpeed = 105;		
		MaxVelocity = 500;
		VelY = 0;

		MovingFlags = MOVING_DOWN;		
		bClimbingWall = false;		
		bIsJumping = false;
		StopSounds();		
		Mix_PlayChannel(CHAN_MEXICAN_LAND, MexicanLandFX, 0);		
	}

	if (Growth == 1)
	{
		AttackCountDown -= DeltaTime;
		
		if (bIsJumping && AttackCountDown > 0)
		{
			PlayAnimation(AnimData.Anim);
			
			if ((AttackCountDown <= 0.35 && AttackCountDown > 0.1))
			{
				Flip = SDL_FLIP_HORIZONTAL;
			}
			else
			{
				Flip = SDL_FLIP_NONE;
			}
		}
		if (AttackCountDown <= 0)
		{
			AttackCountDown = 0;
		}

		if (AttackCountDown == 0 && MovingFlags != MOVING_DOWN)
		{
			if (!bIsJumping)
			{
				MovingFlags = MOVING_DOWN;
			}

			if (bIsJumping && MovingFlags != MOVING_UP)
			{				
				Mix_HaltChannel(99);
				Mix_PlayChannel(99, MexicanJumpFX, 0);
				SDL_Log("PLAYING SOUND");
			}
		}

		if (bIsJumping && AttackCountDown == 0 && JumpCountDown > 0)
		{
			TransitionSpeed = 20;
			MaxVelocity = 333;
			MoveRate = 333;
			StopSpeed = 100;				

			MovingFlags = MOVING_UP;
			JumpCountDown -= DeltaTime;

			if (JumpCountDown <= 0)
			{				
				MovingFlags = MOVING_DOWN;
			}
		}		
	}

	if (Growth < 1)
	{
		Growth += DeltaTime * GrowthRate;
	
		if (Growth > 1)
		{
			Growth = 1;
		}

		if (bClimbingWall)
		{
			PosY = WALL_TOP - Rect.h * Growth + 1;
			StopSpeed = 0.5;
			TransitionSpeed = 33;
			MaxVelocity = 500;						
			bIsJumping = true;
			
			if (Growth == 1)
			{
				StopSounds();				
			}
		}
		else
		{	
			if (bDoSpawnPop)
			{
				if (Growth >= 0.50 && !bHasPlayedSpawnSound)
				{
					Mix_PlayChannel(-1, MexicanSpawnedFX, 0);
					bHasPlayedSpawnSound = true;
				}
			}
			else
			{
				AttackCountDown = 0;
				bHasPlayedSpawnSound = true;
			}
			PosY = HORIZON - Rect.h * Growth + 1;
		}
	}
}

void Mexican1Sprite::CheckCollision(TrumpPlayerSprite *OtherSprite)
{
	if (Growth < 1 || !TheGame)
	{
		return;
	}
	SDL_Rect TrumpCollision = OtherSprite->GetScreenSpaceCollisionRect();
	SDL_Rect ResultRect;

	SDL_Rect CollisionRect = GetScreenSpaceCollisionRect();
	if (SDL_IntersectRect(&TrumpCollision, &CollisionRect, &ResultRect))
	{
		Interact(OtherSprite);
	}
}

void Mexican1Sprite::Interact(TrumpPlayerSprite *OtherSprite)
{
	OtherSprite->TakeDamage();
}

int Mexican1Sprite::GetScoreWorth()
{
	int Score = MEXICAN_BLOCK_SCORE;	

	Score *= bSwapSprites ? 2 : 1;

	return Score;
}

void Mexican1Sprite::StopSounds()
{
	if (ClimbChannel != -1 && Mix_Playing(ClimbChannel))
	{
		Mix_Volume(ClimbChannel, 128);
		Mix_HaltChannel(ClimbChannel);
		ClimbChannel = -1;
		NumClimbingSoundsPlaying--;

		if (NumClimbingSoundsPlaying < 0)
		{
			SDL_Log("HOW");
		}
	}
}