#include "../inc/Mexican1Sprite.h"
#include "../inc/TrumpPlayerSprite.h"

Mexican1Sprite::Mexican1Sprite()
{
	float scale = 0.58;
	SDL_Rect WallRect;
	SDL_Rect CollisionRect;
	SDL_Rect ResultRect;
	bool bBehindWall = false;
	bool bGood = false;
	int WallIndex = 0;

	SetWidth(128 * scale);
	SetHeight(136 * scale);
	
	MoveRate = 444;
	MaxVelocity = 222;
	TransitionSpeed = 2;
	StopSpeed = 4;
	PosY = HORIZON;	
	VelX = 0;
	VelY = 0;
	Growth = 0;
	

	do
	{
		PosX = rand() % (1024 - Rect.w);
		bClimbingWall = false;
		CollisionRect = { 0, 0, 0, 0 };
		bBehindWall = false;		
		Rect.x = PosX;
		Rect.y = PosY;
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
			WallRect = { (WallIndex+1) * 64, WALL_TOP, 64, 160 };
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
		if (rand() % 4 != 0)
		{
			bPendingDelete = true;
			SDL_Log("Abort spawn");
			return;
		}

		bClimbingWall = true;
		MoveRate = 333;
		//PosX = WallIndex * 64;
	}

	MovingFlags = 0;
	PlayAnimation(ResourceManager::TrumpAnimation);
}

void Mexican1Sprite::HandleWallPlaced(int WallIndex)
{
	SDL_Rect CollisionRect = { Rect.x + 18, Rect.y - 22, Rect.w - 40, Rect.h };
	SDL_Rect WallRect = { WallIndex * 64, WALL_TOP, 64, 160 };
	SDL_Rect ResultRect;

	if (PosY > WALL_TOP + 100)
	{
		return;
	}

	if (SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect))
	{
		bPendingDelete = true;
	}
	else if (WallIndex < 15 && TheGame->WallArray[WallIndex + 1])
	{
		CollisionRect = { Rect.x + 18, Rect.y - 22, Rect.w - 40, Rect.h };
		WallRect = { (WallIndex + 1) * 64, WALL_TOP, 64, 160 };
		if (SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect))
		{
			bPendingDelete = true;
		}
	}
	else
	{
		/*CollisionRect = { Rect.x + 10, Rect.y - 22, Rect.w, Rect.h };
		WallRect = { (WallIndex - 1) * 64, WALL_TOP, 64, 160 };

		if (SDL_IntersectRect(&WallRect, &CollisionRect, &ResultRect))
		{
			bPendingDelete = true;
		}*/
	}
}

SDL_Rect Mexican1Sprite::GetCollisionRect()
{
	SDL_Rect CollisionRect = { Rect.x + 20, Rect.y + 20, Rect.w - 40, Rect.h - 30 };

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
				//RenderRect.x -= (Rect.w / 2) * Growth;
			}
			SDL_RenderCopyEx(Renderer, Texture, &SrcRect, &RenderRect, 0, NULL, Flip);
		}
	}
}


void Mexican1Sprite::HandleInput(double DeltaTime)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	const double GrowthRate = 0.5;

	if (Rect.y > 600)
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

void Mexican1Sprite::CheckCollision(TrumpPlayerSprite *OtherSprite)
{
	if (Growth < 1)
	{
		return;
	}
	SDL_Rect TrumpCollision = OtherSprite->GetCollisionRect();
	SDL_Rect ResultRect;

	SDL_Rect CollisionRect = GetCollisionRect();
	if (SDL_IntersectRect(&TrumpCollision, &CollisionRect, &ResultRect))
	{
		OtherSprite->TakeDamage();
	}
}

void Mexican1Sprite::Interact(TrumpPlayerSprite *OtherSprite)
{
}