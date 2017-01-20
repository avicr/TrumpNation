#ifdef _WIN32
	#include <SDL.h>
	#include <SDL_mixer.h>
	#include <SDL_ttf.h>
#else
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_mixer.h"
	#include "SDL2/SDL_ttf.h"
#endif

#include "../inc/ResourceManager.h"
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/Mexican1Sprite.h"
#include "../inc/ItemSprite.h"
#include "../inc/SpriteList.h"
#include "../inc/SwapItem.h"
#include "../inc/ScoreSprite.h"
#include "../inc/CloudSprite.h"
#include "../inc/SpeechBubble.h"
#include "../inc/CatSprite.h"
#include <algorithm>
#include <fstream>

struct HighScoreEntry
{
	long Score;
	int Mile;
	char Name[4];
};

struct Glyph
{
	int Width;
	int Height;
	SDL_Texture *Texture;

	Glyph()
	{
		Width = 16;
		Height = 16;
		Texture = NULL;
	}

	~Glyph()
	{
		if (Texture)
		{
			SDL_DestroyTexture(Texture);
		}
	}
};

HighScoreEntry HighScores[10];
bool bFreezeSpawn = false;
bool bDoSpawnPop = false;
SDL_Window *GWindow;
SDL_Renderer *GRenderer;
ResourceManager *GResourceManager;
bool bSDLInitialized = false;
double BrickSpawnCountDown;
Uint64 TickFreq;
SpriteList Items;
SpriteList Mexicans;
SpriteList DecoSprites;

Mix_Chunk *MexicanSpawnedFX = NULL;
Mix_Chunk *MexicanEscapedFX = NULL;
Mix_Chunk *MexicanClimbFX = NULL;
Mix_Chunk *MexicanLandFX = NULL;
Mix_Chunk *BrickSpawnFX = NULL;
Mix_Chunk *MexicanJumpFX = NULL;
Mix_Chunk *TalkFX = NULL;
Mix_Chunk *PickUpItemFX = NULL;
Mix_Chunk *PlaceWallFX = NULL;
Mix_Chunk *MenuSound1FX = NULL;
Mix_Chunk *MenuSound2FX = NULL;
Mix_Chunk *StepFX = NULL;
Mix_Chunk *TitleConfirmFX = NULL;
Mix_Chunk *TrumpDieFX = NULL;
Mix_Chunk *LevelClearFX = NULL;
Mix_Music *TitleMusic = NULL;
Mix_Music *BGMusic = NULL;
Mix_Music *BGMusicFast = NULL;
Mix_Music *HatDanceMusic = NULL;
Game *TheGame;
TrumpPlayerSprite *ThePlayer;
Glyph FontSeg36[94];
Glyph FontSeg20[94];
Glyph FontSeg36White[94];
Glyph FontSeg20White[94];
Glyph FontSegSmallRed[94];
Glyph FontSegSmallWhite[94];
Glyph FontSegSmallBlue[94];
Glyph FontSegSmallYellow[94];

Glyph FontShadowedWhite[128];
Glyph FontShadowedYellow[128];
Glyph FontShadowedRed[128];
Glyph FontBlue[128];

vector <string> GreatFacts;

float StarScroll = 0;

SDL_Texture *BackBuffer;
int WindowWidth;
int WindowHeight;
bool bSwapSprites = false;
bool bRenderCollision = false;
double BombCountDown = 0;

bool GameLoop();
bool DoTitleScreen();
void Tick(double DeltaTime);
void Render();
void InitSDL();
void CleanUp();
void LoadBitMapFont(string FileName, Glyph *Glyphs);
void DrawHUD(SDL_Renderer *Renderer);
void LoadFont(const char *FontName, int Point, Glyph Glyphs[94], SDL_Color Color = { 0, 0, 0, 255 });
void CopyGlyph(Glyph &TheGlyph, SDL_Texture *FontTexture, int TextureStartX);
void PresentBackBuffer();
void SpawnRandomItem();
double GetSpawnTime();
void DoGameOver();
int GetHighScorePosition(long Score);
void DoDisplayHighScore(int EnterPosition = -1, long Score = 0, int Mile = 0);
void ReadHighScores();
void WriteHighScores();
void DisplayGreatFact();
void ReadGreatFacts();
void RenderStars(float DeltaTime);
void RenderBrickRectangle(SDL_Renderer *Renderer, SDL_Rect &DrawRect, bool bLevelBG = false, int R = -1, int G = -1, int B = -1);
void DoHowTo();
bool bUserQuit = false;

int main(int argc, char ** argv)
{
	
	// init SDL	
	InitSDL();
	LoadBitMapFont("letters_shadow.bmp", FontShadowedWhite);
	LoadBitMapFont("letters_shadow_yellow.bmp", FontShadowedYellow);
	LoadBitMapFont("letters_shadow_red.bmp", FontShadowedRed);
	LoadBitMapFont("letters_regular.bmp", FontBlue);
	srand(SDL_GetTicks());
	SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
	ReadGreatFacts();
	GResourceManager = new ResourceManager;	
	do
	{
		if (!DoTitleScreen())
		{
			break;
		}
	} while (GameLoop());
	
	CleanUp();	

	return 0;
}

void CleanUp()
{
	delete GResourceManager;

	Mix_FreeChunk(MenuSound1FX);
	Mix_FreeChunk(MenuSound2FX);
	Mix_FreeMusic(HatDanceMusic);
	Mix_FreeMusic(BGMusic);
	Mix_FreeMusic(BGMusicFast);
	Mix_FreeChunk(PlaceWallFX);
	Mix_FreeChunk(PickUpItemFX);	
	Mix_FreeChunk(StepFX);
	Mix_FreeChunk(TitleConfirmFX);
	Mix_FreeChunk(TrumpDieFX);
	Mix_FreeChunk(LevelClearFX);	
	Mix_FreeChunk(BrickSpawnFX);
	Mix_FreeChunk(MexicanJumpFX);
	Mix_FreeChunk(TalkFX);
	Mix_FreeChunk(MexicanLandFX);
	Mix_FreeChunk(MexicanClimbFX);
	Mix_FreeChunk(MexicanEscapedFX);
	Mix_FreeChunk(MexicanSpawnedFX);

	TTF_Quit();

	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(GWindow);
	SDL_Quit();
}

bool GameLoop()
{
	double LevelStartedCountDown = 1;
	bool bLevelComplete = false;	
	SDL_Event TheEvent;
	TickFreq = SDL_GetPerformanceFrequency();
	bool bGameComplete = false;
	ThePlayer = new TrumpPlayerSprite();
	ThePlayer->PlayAnimation(ResourceManager::TrumpAnimation);
	TheGame = new Game();
	TheGame->SetLevel(1);
	SDL_SetRenderTarget(GRenderer, NULL);
	SDL_SetRenderDrawColor(GRenderer, 0, 0, 0, 255);
	SDL_RenderClear(GRenderer);
	SDL_SetRenderTarget(GRenderer, BackBuffer);

	//DoHowTo();
	while (!bGameComplete)
	{
		if (!Mix_PlayingMusic())
		{
			Mix_PlayMusic(BGMusic, -1);
		}
		ePlayerState PreviousPlayerState = ThePlayer->GetPlayerState();
		ThePlayer->Reset();
		
		if (bLevelComplete && PreviousPlayerState != StateDead)
		{
			int Pick = rand() % 2;
			if (Pick == 0)
			{
				ThePlayer->Say(2, "TREMENDOUS");
			}
			if (Pick == 1)
			{
				ThePlayer->Say(2, " WINNING");
			}
		}		

		BrickItem *FirstBrick = NULL;
		BrickSpawnCountDown = BRICK_SPAWN_RATE;

		if (ThePlayer->GetNumBricks() == 0)
		{
			FirstBrick = new BrickItem(true);
			FirstBrick->SetPosition(490, 570);
			Items.push_back(FirstBrick);
			BrickSpawnCountDown += BRICK_FIRST_SPAWN_PENALITY;
		}
		
		SDL_Log("Mile: %d, Rate: %f", TheGame->GetLevelNumber(), GetSpawnTime());
		Uint64 StartTime = SDL_GetPerformanceCounter();
		Uint64 CurrentTime = SDL_GetPerformanceCounter();
		double DeltaTime;
		bLevelComplete = false;

		for (int i = 0; i < NUM_CLOUDS; i++)
		{
			DecoSprites.push_back(new CloudSprite());
		}

		SDL_SetRenderTarget(GRenderer, NULL);
		SDL_SetRenderDrawColor(GRenderer, 0, 0, 0, 255);
		SDL_RenderClear(GRenderer);
		SDL_SetRenderTarget(GRenderer, BackBuffer);
		while (!bGameComplete && !bLevelComplete)
		{			
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
			{
				bGameComplete = true;
				bUserQuit = true;
			}

			/*if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_2])
			{
				TheGame->SetLevel(TheGame->GetLevelNumber()+1);
				while (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_2]) { SDL_PollEvent(&TheEvent); }
				SDL_Log("Mile: %d, Rate: %f", TheGame->GetLevelNumber(), GetSpawnTime());
				SDL_SetRenderTarget(GRenderer, NULL);
				SDL_SetRenderDrawColor(GRenderer, 0, 0, 0, 255);
				SDL_RenderClear(GRenderer);
				SDL_SetRenderTarget(GRenderer, BackBuffer);
			}*/

			StartTime = CurrentTime;
			CurrentTime = SDL_GetPerformanceCounter();
			DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());
			Tick(DeltaTime * (double)0.001);

			// Handle swap item done
			if (bSwapSprites && !Mix_PlayingMusic())
			{
				ThePlayer->DoSwap(false);
			}

			if (ThePlayer->GetPlayerState() == StateDying && Mix_PlayingMusic())
			{
				ThePlayer->DoSwap(false);
				Mix_HaltMusic();
				Mix_PlayChannel(3, TrumpDieFX, 0);
			}

			if (TheGame->LevelComplete() || ThePlayer->GetPlayerState() == StateDead)
			{
				bLevelComplete = true;				
			}

			Render();

			//Handle events on queue
			while (SDL_PollEvent(&TheEvent) != 0)
			{
				if (TheEvent.type == SDL_KEYDOWN)
				{					
					//if (TheEvent.key.keysym.scancode == SDL_SCANCODE_0)
					//{
					//	bDoSpawnPop = !bDoSpawnPop;
					//}

					//if (TheEvent.key.keysym.scancode == SDL_SCANCODE_3)
					//{
					//	//Mix_PlayChannel(-1, LevelClearFX, 0);
					//	SpawnRandomItem();
					//}

					//if (TheEvent.key.keysym.scancode == SDL_SCANCODE_5)
					//{
					//	//Mix_PlayChannel(-1, LevelClearFX, 0);
					//	Items.push_back(new CatSprite());
					//}

					//if (TheEvent.key.keysym.scancode == SDL_SCANCODE_1)
					//{
					//	Mix_PlayChannel(-1, LevelClearFX, 0);
					//	bRenderCollision = !bRenderCollision;
					//}

					//if (TheEvent.key.keysym.scancode == SDL_SCANCODE_4)
					//{						
					//	ThePlayer->Say(2, "GO BACK TO\nUNIVISION!");
					//}
				}
if (TheEvent.type == SDL_JOYAXISMOTION)
{
	int Axis = TheEvent.jaxis.axis;
	SDL_Log("Axis: %d, %d", Axis, TheEvent.jaxis.value);
}
//SDL_Log("Event Type: %d", TheEvent.type);
if (TheEvent.type == SDL_MOUSEBUTTONDOWN)
{
	ThePlayer->SetPosition(TheEvent.button.x, TheEvent.button.y);
}
//User requests quit
if (TheEvent.type == SDL_QUIT)
{
	bGameComplete = true;
	bUserQuit = true;
}
			}
		}

		if (ThePlayer->GetNumLives() < 0)
		{
			bGameComplete = true;
		}
		Mexicans.DeleteAll();
		Items.DeleteAll();
		DecoSprites.DeleteAll();
	}

	if (!bUserQuit)
	{
		DoGameOver();
	}

	delete ThePlayer;
	delete TheGame;
	ThePlayer = NULL;
	TheGame = NULL;

	return !bUserQuit;
}

void Tick(double DeltaTime)
{
	static double SpawnCountdown = GetSpawnTime();
	static double ItemSpawnCountdown = ITEM_RATE;
	static int ItemChance = ITEM_SPAWN_PERCENT;

	if (BombCountDown >= 0)
	{
		BombCountDown -= DeltaTime;
	}

	//if (ItemSprite::NumNonBrickItems == 0)
	{
		ItemSpawnCountdown -= DeltaTime;
	}

	SpawnCountdown -= DeltaTime;
	BrickSpawnCountDown -= DeltaTime;

	if (SpawnCountdown <= 0 && !bFreezeSpawn && ThePlayer->GetPlayerState() != StateDying)
	{
		/*if (rand() % (100 / CAT_SPAWN_PERCENT) == 0)
		{
			Mexicans.push_back(new CatSprite());
		}
		else*/
		{
			Mexicans.push_back(new Mexican1Sprite());
		}
		SpawnCountdown = GetSpawnTime();
	}

	if (BrickSpawnCountDown <= 0)
	{
		bool bForceBrickSpawn = ThePlayer->GetNumBricks() == 0 && Items.size() - ItemSprite::NumNonBrickItems == 0;
		BrickSpawnCountDown = BRICK_SPAWN_RATE;

		if (ThePlayer->GetPlayerState() != StateDying && (bForceBrickSpawn || (ThePlayer->GetNumBricks() + Items.size() - ItemSprite::NumNonBrickItems < MAX_BRICKS && rand() % (100 / BRICK_SPAWN_PERCENT) == 0)))
		{
			if (TheGame->GetLevelNumber() >= GOLD_BRICK_MILE_START && rand() % (100 / GOLD_BRICK_SPAWN_PERCENT) == 0)
			{
				Items.push_back(new BrickGoldItem());
			}
			else
			{
				Items.push_back(new BrickItem());
			}

			if (!bForceBrickSpawn)
			{
				ItemSpawnCountdown += ITEM_ON_BRICK_SPAWN_PENALITY;
				BrickSpawnCountDown += BRICK_ON_BRICK_SPAWN_PENALITY;
			}
		}
	}

	if (ItemSpawnCountdown <= 0)
	{
		ItemSpawnCountdown = ITEM_RATE;		
		if (rand() % (100 / ItemChance) == 0)
		{
			ItemChance = ITEM_SPAWN_PERCENT;
			SpawnRandomItem();
		}
		else
		{
			ItemChance += 4;
			if (ItemChance > 100)
			{
				ItemChance = 100;
			}
			SDL_Log("Item miss, chance is now: 1 in %d", 100 / ItemChance);
		}
	}

	ThePlayer->Tick(DeltaTime);
	
	if (ThePlayer->GetPlayerState() != StateDead && ThePlayer->GetPlayerState() != StateDying)
	{
		DecoSprites.Tick(DeltaTime);
		Mexicans.Tick(DeltaTime);
		Items.Tick(DeltaTime);
		
	}
}

void Render()
{
	// Clear to ground color
	/*SDL_SetRenderDrawColor(GRenderer, 217, 201, 124, 255);
	SDL_RenderClear(GRenderer);*/

	SDL_Rect Rect = { 0, 0, 1024, 600 };
	SDL_RenderCopy(GRenderer, ResourceManager::BGTexture->Texture, &Rect, &Rect);

	Rect.x = 0;
	Rect.y = 192;
	Rect.w = 1024;
	Rect.h = 448;
	SDL_RenderCopy(GRenderer, TheGame->GetGroundTexture(), NULL, &Rect);

	// Render sky
	/*SDL_Rect Rect = { 0, 0, 1024, HORIZON };
	SDL_SetRenderDrawColor(GRenderer, 0, 162, 232, 255);
	SDL_RenderFillRect(GRenderer, &Rect);*/	

	for (int WallIndex = 0; WallIndex < 16; WallIndex++)
	{
		Rect.x = WallIndex * 64;
		Rect.y = WALL_TOP;
		Rect.w = 64;
		Rect.h = 160;

		if (TheGame->WallArray[WallIndex] == 1)
		{
			SDL_RenderCopy(GRenderer, ResourceManager::WallTexture->Texture, NULL, &Rect);
			
			/*if (TheGame->WallArray[WallIndex] > 1)
			{				
				Rect.y -= 160;				
				SDL_RenderCopy(GRenderer, ResourceManager::WallTexture->Texture, NULL, &Rect);				
			}*/
		}
		else if (TheGame->WallArray[WallIndex] == 2)
		{
			SDL_RenderCopy(GRenderer, ResourceManager::WallGoldTexture->Texture, NULL, &Rect);
		}
	}
	/*Rect = { 0, HORIZON + 65, 992, 600 - (600-HORIZON) - 21};
	SDL_SetRenderDrawColor(GRenderer, 255, 0, 0, 100);
	SDL_RenderFillRect(GRenderer, &Rect);*/
	
	//new BrickItem(rand() % 992, (rand() % (200) + HORIZON + 65)));
	
	DecoSprites.Render(GRenderer);
	Items.Render(GRenderer);
	Mexicans.Render(GRenderer);

	ThePlayer->Render(GRenderer);	

	DrawHUD(GRenderer);

	if (BombCountDown > 0 && (int)round(BombCountDown * 100) % 4 >= 2)
	{
		SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
		SDL_RenderClear(GRenderer);
	}
	PresentBackBuffer();
}

bool DoTitleScreen()
{	
	bool bDone = false;
	bool bScrollDone = true;
	bool bSpawnNew = true;
	double PosY = 0;
	double ScrollCountDown = TITLE_SCROLL_TIME;
	int NumIntrosPlayed = 0;	
	//double TitleFadeInCount = 0;
	int HighScoresCount = 0;

	SDL_Event TheEvent;
	TitleMusic = Mix_LoadMUS("resource/sounds/GroovinInSpace.wav");
	Mix_PlayMusic(TitleMusic, 0);
	Sprite *TrumpIntroSprite = new Sprite();	
	TrumpIntroSprite->PlayAnimation(ResourceManager::TrumpIntroAnimation);
	TrumpIntroSprite->SetWidth(128);
	TrumpIntroSprite->SetHeight(128);
	
	double MexicanSpawnCountDown = 0.25;
	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = SDL_GetPerformanceCounter();
	double DeltaTime;
	
	SDL_SetRenderTarget(GRenderer, NULL);
	SDL_SetRenderDrawColor(GRenderer, 0, 0, 0, 255);
	SDL_RenderClear(GRenderer);
	SDL_SetRenderTarget(GRenderer, BackBuffer);
	
	Sprite CatTest;
	
	CatTest.PlayAnimation(ResourceManager::CatAnimation);	
	
	while (!bDone && !bUserQuit)
	{		

		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
		{
			bDone = true;
			bUserQuit = true;
		}
		
		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());
		DeltaTime *= (double)0.001;

		if (bSpawnNew == true)
		{
			Mexican1Sprite *Test = new Mexican1Sprite;
			Test->SetWidth(32);
			Test->SetHeight(32);
			Mexicans.push_back(Test);

			CatTest.SetWidth(32);
			CatTest.SetHeight(32);			
			bSpawnNew = false;
		}		

		TrumpIntroSprite->Tick(DeltaTime);		
		
		ScrollCountDown -= DeltaTime;
		MexicanSpawnCountDown -= DeltaTime;

		/*if (MexicanSpawnCountDown <= 0)
		{
			MexicanSpawnCountDown = 0.25;

			if (rand() % 5 == 0)
			{
				Mexican1Sprite *Test = new Mexican1Sprite;;
				Test->SetWidth(32);
				Test->SetHeight(32);				
				Mexicans.push_back(Test);
			}
		}*/
		//TitleFadeInCount += DeltaTime * 0.5;
		
		if (!bScrollDone && ScrollCountDown <= TITLE_SCROLL_TIME - 21.25)
		{	
			PosY += DeltaTime * 80;
		}

		/*if (PosY >= ResourceManager::InfoTexture->SrcRect.h - 100)
		{			
			PosY = 0;
			bScrollDone = true;
		}*/

		
		if (ScrollCountDown <= 0)
		{
			//if (rand() % 4 != 0)
			{				
				Mexicans.DeleteAll();
				Uint64 PauseStart = SDL_GetPerformanceCounter();
				DoDisplayHighScore();				
				Uint64 PauseTime = SDL_GetPerformanceCounter() - PauseStart;
				CurrentTime += PauseTime;
				PauseTime *= 1000 / (double)SDL_GetPerformanceFrequency();
				ScrollCountDown = TITLE_SCROLL_TIME;
				NumIntrosPlayed++;
#ifndef PARTY
				if (NumIntrosPlayed % 4 == 0)
				{
					Mix_PlayMusic(TitleMusic, 0);
				}
#endif
				if (NumIntrosPlayed == 8)
				{
					for (int i = 0; i < 1; i++)
					{
						Sprite *MexicanIntroSprite = new Sprite();
						MexicanIntroSprite->PlayAnimation(ResourceManager::Mexican1Animation);
						MexicanIntroSprite->SetAnimationPlayRate(0.5);
						MexicanIntroSprite->SetWidth(64);
						MexicanIntroSprite->SetHeight(64);
						int NewX = 264 + (rand() % (int)(700 - 264 + 1));
						int NewY = 200 + (rand() % (int)(380 - 200 + 1));
						MexicanIntroSprite->SetPosition(NewX, NewY);
						Mexicans.push_back(MexicanIntroSprite);

					}
				}

				if (NumIntrosPlayed == 9)
				{
					for (int i = 0; i < 10; i++)
					{
						Sprite *MexicanIntroSprite = new Sprite();
						MexicanIntroSprite->PlayAnimation(ResourceManager::Mexican1Animation);
						MexicanIntroSprite->SetAnimationPlayRate(0.5);
						MexicanIntroSprite->SetWidth(64);
						MexicanIntroSprite->SetHeight(64);
						int NewX = 264 + (rand() % (int)(700 - 264 + 1));
						int NewY = 200 + (rand() % (int)(380 - 200 + 1));
						MexicanIntroSprite->SetPosition(NewX, NewY);
						Mexicans.push_back(MexicanIntroSprite);
						MexicanIntroSprite->Tick(DeltaTime * i);

					}
				}

				if (NumIntrosPlayed == 10)
				{
					for (int i = 0; i < 70; i++)
					{
						Sprite *MexicanIntroSprite = new Sprite();
						MexicanIntroSprite->PlayAnimation(ResourceManager::Mexican1Animation);
						MexicanIntroSprite->SetAnimationPlayRate(0.5);
						MexicanIntroSprite->SetWidth(64);
						MexicanIntroSprite->SetHeight(64);
						int NewX = 264 + (rand() % (int)(700 - 264 + 1));
						int NewY = 200 + (rand() % (int)(380 - 200 + 1));
						MexicanIntroSprite->SetPosition(NewX, NewY);
						Mexicans.push_back(MexicanIntroSprite);
					}
				}

				if (NumIntrosPlayed == 15)
				{
					Sprite *MexicanIntroSprite = new Sprite();
					MexicanIntroSprite->PlayAnimation(ResourceManager::Mexican1Animation);
					MexicanIntroSprite->SetAnimationPlayRate(0.5);
					MexicanIntroSprite->SetWidth(128);
					MexicanIntroSprite->SetHeight(128);
					MexicanIntroSprite->SetPosition(454, 255 - PosY);
					Mexicans.push_back(MexicanIntroSprite);
				}

				if (NumIntrosPlayed == 1)
				{
					CatTest.SetPosition(2080, 408);					
					CatTest.SetMovingFlags(MOVING_LEFT);
				}
				else
				{
					CatTest.SetMovingFlags(MOVING_NONE);
					CatTest.SetPosition(0, 0);
				}
			}			
		}


		if (NumIntrosPlayed == NUM_INTRO_CYCLES)
		{	
			bSpawnNew = true;
			Mix_PlayMusic(TitleMusic, 0);
			//bScrollDone = false;
			NumIntrosPlayed = 0;
			ScrollCountDown = TITLE_SCROLL_TIME;			
		}

		RenderStars(DeltaTime);

		//SDL_Log("Info texture: %d", ResourceManager::InfoTexture->Texture);
		TrumpIntroSprite->SetPosition(454, 255 - PosY);		
		
		//SDL_Rect BackBufferRect = { 0, 0, 1024, fmin(600,ResourceManager::TitleScreenTexture->SrcRect.h - PosY) };
		SDL_Rect BackBufferRect = { 0, 0, 1024, 600 };
		SDL_RenderCopy(GRenderer, ResourceManager::TitleScreenTexture->Texture, NULL, &BackBufferRect);
		//SDL_SetRenderDrawBlendMode(GRenderer, SDL_BLENDMODE_BLEND);
		//SDL_SetTextureBlendMode(ResourceManager::TitleScreenTexture->Texture, SDL_BLENDMODE_BLEND);
		//SDL_SetTextureAlphaMod(ResourceManager::TitleScreenTexture->Texture, TitleFadeInCount * 255);

		if (NumIntrosPlayed < 8 || NumIntrosPlayed > 10 && NumIntrosPlayed != 15)
		{		
			TrumpIntroSprite->Render(GRenderer);
		}
		Mexicans.Tick(DeltaTime);
		Mexicans.Render(GRenderer);
		
		CatTest.Tick(DeltaTime);

		if (CatTest.GetScreenSpaceCollisionRect().x > 260 && CatTest.GetMovingFlags() == MOVING_LEFT && CatTest.GetScreenSpaceCollisionRect().x <= 730)
		{											
			CatTest.Render(GRenderer);						
			CatTest.SetMovingFlags(MOVING_LEFT);
			//CatTest.SetMaxVelocity(100);			
		}
		
		DrawText("CREATED BY CRISPIN AVILA\n    ART BY IVAN DIXON", 326, 550, 16, 16, GRenderer, FontShadowedWhite, 1, 1.65);
		PresentBackBuffer();

		while (SDL_PollEvent(&TheEvent) != 0)
		{
			////User requests quit
			//if (TheEvent.type == SDL_KEYDOWN)
			//{
			//	bDone = true;
			//}

			if (TheEvent.type == SDL_KEYDOWN)
			{
				if (TheEvent.key.keysym.scancode == SDL_SCANCODE_RETURN || TheEvent.key.keysym.scancode == SDL_SCANCODE_SPACE)
				{
					bDone = true;
				}
			}

			if (TheEvent.type == SDL_QUIT)
			{
				bUserQuit = true;
				bDone = true;
			}
		}
	}
	RenderStars(DeltaTime);
	TrumpIntroSprite->SetPosition(454, 255);
	//SDL_Rect TitleRect = { 0, 0, 1024, 600 };
	SDL_Rect BackBufferRect = { 0, 0, 1024, 600 };
	SDL_RenderCopy(GRenderer, ResourceManager::TitleScreenTexture->Texture, NULL, &BackBufferRect);
	TrumpIntroSprite->Render(GRenderer);
	DrawText("CREATED BY CRISPIN AVILA\n    ART BY IVAN DIXON", 326, 550, 16, 16, GRenderer, FontShadowedWhite, 1, 1.65);
	//Mexicans.Render(GRenderer);
	PresentBackBuffer();
	
	Mix_HaltMusic();	
	Mix_FreeMusic(TitleMusic);

	delete TrumpIntroSprite;
	Mexicans.DeleteAll();

	int PlayingChannel = Mix_PlayChannel(-1, TitleConfirmFX, 0);

	while (Mix_Playing(PlayingChannel) && !bUserQuit)
	{
		while (SDL_PollEvent(&TheEvent) != 0)
		{
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
			{
				bDone = true;
				bUserQuit = true;
				break;
			}
			if (TheEvent.type == SDL_QUIT)
			{
				bDone = true;
				bUserQuit = true;
				break;
			}
		}
		
	}
	SDL_Delay(500);
	return !bUserQuit;
}

void InitSDL()
{
	if (!bSDLInitialized)
	{				

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
		SDL_Log("SDL INIT");
		TTF_Init();
		SDL_Log("After TTF INIT");
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{

		}
		else
		{
			MenuSound2FX = Mix_LoadWAV("resource/sounds/menusound1.wav");
			MenuSound1FX = Mix_LoadWAV("resource/sounds/mexicanclimb.wav");
			PickUpItemFX = Mix_LoadWAV("resource/sounds/Pickupitem.wav");
			PlaceWallFX = Mix_LoadWAV("resource/sounds/Placewall.wav");
			StepFX = Mix_LoadWAV("resource/sounds/Step.wav");
			TitleConfirmFX = Mix_LoadWAV("resource/sounds/Titleconfirm.wav");
			TrumpDieFX = Mix_LoadWAV("resource/sounds/Trumpdie.wav");
			LevelClearFX = Mix_LoadWAV("resource/sounds/Levelclear.wav");			
			BrickSpawnFX = Mix_LoadWAV("resource/sounds/brickspawn3.wav");
			MexicanJumpFX = Mix_LoadWAV("resource/sounds/mexicanjump.wav");
			TalkFX = Mix_LoadWAV("resource/sounds/talk.wav");
			MexicanLandFX = Mix_LoadWAV("resource/sounds/mexicanland.wav");
			MexicanClimbFX = Mix_LoadWAV("resource/sounds/mexicanclimb.wav");
			MexicanEscapedFX = Mix_LoadWAV("resource/sounds/step7.wav");
			MexicanSpawnedFX = Mix_LoadWAV("resource/sounds/step3.wav");
			
			Mix_VolumeChunk(MexicanSpawnedFX, 48);
			Mix_VolumeChunk(StepFX, 60);
			Mix_VolumeChunk(BrickSpawnFX, 54);
			Mix_VolumeChunk(MexicanJumpFX, 90);
			Mix_VolumeChunk(PickUpItemFX, 90);
			Mix_VolumeChunk(MexicanLandFX, 110);
			Mix_VolumeChunk(MexicanClimbFX, 100);
			Mix_VolumeChunk(MexicanEscapedFX, 48);
		}

		GWindow = SDL_CreateWindow("Trump Nation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, SDL_WINDOW_OPENGL /*| SDL_WINDOW_FULLSCREEN_DESKTOP*/);
		SDL_GetWindowSize(GWindow, &WindowWidth, &WindowHeight);
		GRenderer = SDL_CreateRenderer(GWindow, -1, 0);
		BackBuffer = SDL_CreateTexture(GRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 600);
		SDL_SetRenderTarget(GRenderer, BackBuffer);
		SDL_Log("After create window");
		/*LoadFont("resource/fonts/segoeuib.ttf", 36, FontSeg36);
		LoadFont("resource/fonts/segoeuib.ttf", 28, FontSeg20);
		LoadFont("resource/fonts/segoeuib.ttf", 36, FontSeg36White, { 255, 255, 255, 255 });
		LoadFont("resource/fonts/segoeuib.ttf", 28, FontSeg20White, { 255, 255, 255, 255 });
		LoadFont("resource/fonts/segoeuib.ttf", 18, FontSegSmallRed, { 255, 0, 0, 255 });
		LoadFont("resource/fonts/segoeuib.ttf", 18, FontSegSmallWhite, { 255, 255, 255, 255 });
		LoadFont("resource/fonts/segoeuib.ttf", 18, FontSegSmallBlue, { 0, 0, 255, 255 });
		LoadFont("resource/fonts/segoeuib.ttf", 18, FontSegSmallYellow, { 255, 255, 0, 255 });		*/

		Mix_AllocateChannels(100);
		SDL_Log("After load numerals");
		bSDLInitialized = true;
		SDL_ShowCursor(SDL_DISABLE);
		BGMusic = Mix_LoadMUS("resource/sounds/PuttingUpWalls.wav");
		BGMusicFast = Mix_LoadMUS("resource/sounds/PuttingUpWallsFast.mp3");
		HatDanceMusic = Mix_LoadMUS("resource/sounds/Trumphatdance.ogg");
	}
}

SDL_Renderer *GetRenderer()
{
	if (GRenderer == NULL)
	{
		InitSDL();
	}

	return GRenderer;
}



void DrawText(string Text, int X, int Y, int SizeX, int SizeY, SDL_Renderer *Renderer, Glyph Glyphs[127], float SpaceScaleX, float SpaceScaleY, bool bRightJustify)
{
	double PosX = X;
	double PosY = Y;

	if (!bRightJustify)
	{
		for (int i = 0; i < Text.size(); i++)
		{

			char CharToRender = Text.at(i);

			if (!Glyphs[CharToRender].Texture && CharToRender != '\n')
			{
				continue;
			}

			if (CharToRender == '\n')
			{
				
				PosY += SizeY * SpaceScaleY;
				PosX = X;
			}
			else if (CharToRender >= 0 && CharToRender < 94)
			{				
				SDL_Rect SrcRect = { 0, 0, Glyphs[CharToRender].Width, Glyphs[CharToRender].Height };
				SDL_Rect DstRect = { PosX, PosY, SizeX, SizeY};

				SDL_RenderCopy(Renderer, Glyphs[CharToRender].Texture, &SrcRect, &DstRect);

				PosX += SizeX * SpaceScaleX;				
			}
		}
	}
	else
	{
		PosX -= SizeX;
		for (int i = Text.size() - 1; i >= 0; i--)
		{
			char CharToRender = Text.at(i);
			
			if (!Glyphs[CharToRender].Texture && CharToRender != '\n')
			{
				continue;
			}

			SDL_Rect SrcRect = { 0, 0, Glyphs[CharToRender].Width, Glyphs[CharToRender].Height };
			SDL_Rect DstRect = { PosX, Y, SizeX, SizeY };

			SDL_RenderCopy(Renderer, Glyphs[CharToRender].Texture, &SrcRect, &DstRect);

			PosX -= SizeX * SpaceScaleX;			
		}
	}
}

void DrawHUD(SDL_Renderer *Renderer)
{
	SDL_Rect HUDRect = { 0, 0, 1024, 64 };
	//SDL_RenderCopy(GRenderer, ResourceManager::HUDTexture->Texture, &HUDRect, &HUDRect);
	//DrawText(std::to_string(TheGame->GetLevelNumber()), 535, 17, 32, 32, Renderer, FontSeg36);


	//DrawText(std::to_string(ThePlayer->GetScore()), 918, 3, 18, 32, Renderer, FontSeg20);
	string ScoreString = std::to_string(ThePlayer->GetScore());
	string PadString;
	int NumZeros = 6 - ScoreString.size();
	for (int i = 0; i < NumZeros; i++)
	{
		PadString += "0";
	}

	SDL_Rect DstRect = { 12, 11, 36, 36 };
	SDL_RenderCopy(GRenderer, ResourceManager::TrumpFaceTexture->Texture, NULL, &DstRect);

	if (ThePlayer->HasRedHat())
	{
		DstRect.x = 16;
		DstRect.y = 14;
		DstRect.w = ResourceManager::RedHatTexture->SrcRect.w * 2.6;
		DstRect.h = ResourceManager::RedHatTexture->SrcRect.h * 2.6;

		SDL_RenderCopy(GRenderer, ResourceManager::RedHatTexture->Texture, NULL, &DstRect);
	}
	else if (bSwapSprites)
	{
		DstRect.x = 7;
		DstRect.y = 7;
		DstRect.w = ResourceManager::SombreroTexture->SrcRect.w * 2.7;
		DstRect.h = ResourceManager::SombreroTexture->SrcRect.h * 2.7;

		SDL_RenderCopy(GRenderer, ResourceManager::SombreroTexture->Texture, NULL, &DstRect);
	}
	DrawText("X", 48, 22, 24, 24, Renderer, FontBlue, 0.75, 0.75);
	DrawText(std::to_string(ThePlayer->GetNumLives()), 70, 15, 32, 32, Renderer, FontShadowedWhite, 0.75, 0.75);

	for (int i = 0; i < ThePlayer->GetNumBricks(); i++)
	{
		DstRect = ResourceManager::BrickTexture->SrcRect;
		DstRect.x = 110 + i * 35;
		DstRect.y = 24;
		DstRect.w *= 2.25;
		DstRect.h *= 2.25;

		if (ThePlayer->GetBrickInvetory()[i] == BrickRegular)
		{
			SDL_RenderCopy(GRenderer, ResourceManager::BrickTexture->Texture, NULL, &DstRect);
		}
		else if (ThePlayer->GetBrickInvetory()[i] == BrickGold)
		{
			SDL_RenderCopy(GRenderer, ResourceManager::BrickGoldTexture->Texture, NULL, &DstRect);
		}

	}

	DrawText("MILE", 444, 17, 32, 32, Renderer, FontBlue, 0.75, 0.75);
	
	DrawText(std::to_string(TheGame->GetLevelNumber()), 540, 15, 32, 32, Renderer, FontShadowedWhite, 0.75, 0.75);
	
	
	DrawText("SCORE", 740, 17, 32, 32, Renderer, FontBlue, 0.75, 0.75);
	DrawText(PadString, 866, 15, 32, 32, Renderer, FontShadowedWhite, 0.75, 0.75);
	DrawText(ScoreString, 1017, 15, 32, 32, Renderer, FontShadowedWhite, 0.75, 0.75, true);	
	
	//DrawText(std::to_string(ThePlayer->GetNumLives()), 98, 3, 18, 32, Renderer, FontSeg20);	
}



void LoadFont(const char *FontName, int Point, Glyph Glyphs[94], SDL_Color Color)
{	
	TTF_Font *Font = TTF_OpenFont(FontName, Point);
	
	for (int i = 0; i < 94; i++)
	{
		Uint32 OutFormat;
		int OutAccess;

		SDL_Surface *NumeralSurface = TTF_RenderGlyph_Blended(Font, 32 + i, Color);
		Glyphs[i].Texture = SDL_CreateTextureFromSurface(GRenderer, NumeralSurface);
		SDL_QueryTexture(Glyphs[i].Texture, &OutFormat, &OutAccess, &Glyphs[i].Width, &Glyphs[i].Height);
		SDL_FreeSurface(NumeralSurface);
	}

	TTF_CloseFont(Font);
}

void LoadBitMapFont(string FileName, Glyph *Glyphs)
{
	SDL_SetRenderTarget(GRenderer, NULL);

	SDL_Surface *Image = SDL_LoadBMP((TEXTURE_PATH + FileName).c_str());
	SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 0xFF, 0, 0xFF));
	SDL_Texture *FontTexture = SDL_CreateTextureFromSurface(GRenderer, Image);	

	// Numbers start at X 16 in the texture
	for (int i = 48; i < 58; i++)
	{
		CopyGlyph(Glyphs[i], FontTexture, 16 + (i - 48) * 16);
	}
	
	// A starts at X 176
	for (int i = 65; i < 91; i++)
	{
		CopyGlyph(Glyphs[i], FontTexture, 176 + (i - 65) * 16);
	}

	CopyGlyph(Glyphs[' '], FontTexture, 0);
	CopyGlyph(Glyphs['.'], FontTexture, 592);
	CopyGlyph(Glyphs['-'], FontTexture, 608);
	CopyGlyph(Glyphs['!'], FontTexture, 624);
	CopyGlyph(Glyphs['?'], FontTexture, 640);

	//SDL_Rect DstRect = { 0, 0, 16, 16 };
	//
	//
	//while (!SDL_GetKeyboardState(NULL)[SDL_SCANCODE_0])
	//{		
	//	//PresentBackBuffer();
	//	SDL_Event TheEvent;
	//	while (SDL_PollEvent(&TheEvent) != 0)
	//	{
	//		DrawText("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG\n0123456789.-!?", 0, 0, 32, 32, GRenderer, FontShadowed, 1, 1, false);
	//		//SDL_RenderCopy(GRenderer, Glyphs['A'].Texture, NULL, &DstRect);
	//		PresentBackBuffer();
	//	}		
	//}

	SDL_FreeSurface(Image);
	SDL_DestroyTexture(FontTexture);
}

void CopyGlyph(Glyph &TheGlyph, SDL_Texture *FontTexture, int TextureStartX)
{
	SDL_Rect SrcRect = { TextureStartX, 0, 16, 16 };
	TheGlyph.Texture = SDL_CreateTexture(GRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 16, 16);

	SDL_SetRenderTarget(GRenderer, TheGlyph.Texture);	
	SDL_SetTextureBlendMode(TheGlyph.Texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(GRenderer, 255, 0, 255, 0);
	SDL_RenderClear(GRenderer);
	SDL_Rect DstRect = { 0, 0, 16, 16 };
	SDL_RenderCopy(GRenderer, FontTexture, &SrcRect, &DstRect);
	SDL_SetRenderTarget(GRenderer, NULL);
}

void PresentBackBuffer()
{
	SDL_SetRenderTarget(GRenderer, NULL);
	SDL_Rect BackBufferRect = { 0, 0, 1024, 600 };
	SDL_Rect BackBufferDstRect = { WindowWidth / 2 - 512, WindowHeight / 2 - 300, 1024, 600 };
	SDL_RenderCopy(GRenderer, BackBuffer, &BackBufferRect, &BackBufferDstRect);
	SDL_RenderPresent(GRenderer);
	SDL_SetRenderTarget(GRenderer, BackBuffer);
}

void SpawnRandomItem()
{
	int CurrentLevel = TheGame->GetLevelNumber();
	int Roll = rand() % 100;
	ItemSprite *NewItem = NULL;
	
	if (ThePlayer->GetPlayerState() == StateDying)
	{
		return;
	}

	if (Roll < 2 && CurrentLevel >= EXTRA_LIFE_MILE_START)
	{
		NewItem = new ExtraLifeItem();
	}
	else if (Roll < 25 && CurrentLevel >= SOMBRERO_MILE_START && !bSwapSprites && !ThePlayer->HasRedHat())
	{
		NewItem = new SwapItem();
	}
	else if (Roll < 55)
	{
		NewItem = new BombItem();
	}
	else if (!bSwapSprites && !ThePlayer->HasRedHat())
	{
		NewItem = new RedHatItem();
	}	

	if (NewItem)
	{
		Items.push_back(NewItem);
	}
}

double GetSpawnTime()
{
	float Variance = -SPAWN_VARIABLE_TIME + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (SPAWN_VARIABLE_TIME - -SPAWN_VARIABLE_TIME)));
	if (TheGame->GetLevelNumber() == 1)
	{
		return 1 + Variance;
	}
	else if (TheGame->GetLevelNumber() == 2)
	{
		return 0.85 + Variance;
	}
	else if (TheGame->GetLevelNumber() == 3)
	{
		return 0.75 + Variance;
	}
	else if (TheGame->GetLevelNumber() == 4)
	{
		return 0.65 + Variance;
	}
	else if (TheGame->GetLevelNumber() == 5)
	{
		return 0.50 + Variance;
	}
	else if (TheGame->GetLevelNumber() == 6)
	{
		return 0.35 + Variance;
	}
	else if (TheGame->GetLevelNumber() == 7)
	{
		return 0.20 + Variance;
	}
	else
	{
		return (1 / log1p(TheGame->GetLevelNumber() * TheGame->GetLevelNumber() * TheGame->GetLevelNumber())) + Variance;
	}
}

void DoGameOver()
{
	enum eGameOverState {GameOverInit = 0, GameOverPlayerScore, GameOverMiles, GameOverMexicans, GameOverTotal} 
	GameOverState = GameOverInit;
	long TotalScore = 0;	
	bool bDone = false;
	double GameOverCountDown = 10;
	bool bCountFinished = false;

	SDL_Event TheEvent;	

	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = SDL_GetPerformanceCounter();
	double DeltaTime;

	while (!bDone)
	{
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
		{
			bDone = true;
			bUserQuit = true;
		}

		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());
		DeltaTime *= (double)0.001;

		GameOverCountDown -= DeltaTime;

		if (GameOverCountDown < 0)
		{
			bCountFinished = true;
		}
		SDL_Rect BackBufferRect = { 0, 0, 1024, 600 };
		SDL_RenderCopy(GRenderer, ResourceManager::StarBGTexture->Texture, NULL, &BackBufferRect);

		DrawText("GAME OVER", 380, 16, 32, 32, GRenderer, FontShadowedRed, 0.95, 0.95);

		SDL_Rect WallRect = { 118, 157, 769, 304 };	

		RenderBrickRectangle(GRenderer, WallRect, true, 0, 11, 42);

		if (GameOverState == GameOverInit)
		{
			GameOverState = GameOverPlayerScore;
			Mix_PlayChannel(-1, PlaceWallFX, 0);
		}
		DrawText("PLAYER SCORE", 152, 180, 24, 24, GRenderer, FontShadowedYellow);
		DrawText(std::to_string(ThePlayer->GetScore()), 869, 180, 24, 24, GRenderer, FontShadowedWhite, 1, 1, true);
		
		if (GameOverCountDown <= 9)
		{
			if (GameOverState == GameOverPlayerScore)
			{
				GameOverState = GameOverMiles;
				Mix_PlayChannel(-1, PlaceWallFX, 0);
			}
			DrawText("MILES COMPLETED   2000 X", 152, 220, 24, 24, GRenderer, FontShadowedYellow, 1, 1);
			DrawText(std::to_string(TheGame->GetLevelNumber() - 1), 869, 220, 24, 24, GRenderer, FontShadowedWhite, 1, 1, true);
		}

		if (GameOverCountDown <= 8)
		{
			if (GameOverState == GameOverMiles)
			{
				GameOverState = GameOverMexicans;
				Mix_PlayChannel(-1, PlaceWallFX, 0);
			}

			SDL_Rect DstRect = { 152, 255, ResourceManager::MexicanFaceTexture->SrcRect.w, ResourceManager::MexicanFaceTexture->SrcRect.h };

			DstRect.w *= 1.25;
			DstRect.h *= 1.25;

			SDL_RenderCopy(GRenderer, ResourceManager::MexicanFaceTexture->Texture, NULL, &DstRect);
			DrawText("  CROSSED BORDER   -50 X", 152, 260, 24, 24, GRenderer, FontShadowedYellow, 1, 1);
			DrawText(std::to_string(TheGame->GetNumMexicansEscaped()), 869, 260, 24, 24, GRenderer, FontShadowedWhite, 1, 1, true);
		}

		if (GameOverCountDown <= 7)
		{
			if (GameOverState == GameOverMexicans)
			{
				bCountFinished = true;
				GameOverState = GameOverTotal;
				Mix_PlayChannel(-1, PlaceWallFX, 0);
			}

			SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
			SDL_RenderDrawLine(GRenderer, 148, 358, 875, 358);
			SDL_RenderDrawLine(GRenderer, 148, 359, 875, 359);
			SDL_RenderDrawLine(GRenderer, 148, 360, 875, 360);

			TotalScore = ThePlayer->GetScore() + (TheGame->GetLevelNumber() - 1) * LEVEL_CLEAR_POINTS + TheGame->GetNumMexicansEscaped() * MEXICAN_ESCAPED_POINTS;
			DrawText("TOTAL", 152, 384, 24, 24, GRenderer, FontShadowedYellow, 1, 1);
			DrawText(std::to_string(TotalScore), 869, 384, 24, 24, GRenderer, FontShadowedWhite, 1, 1, true);
			DrawText("PRESS BUTTON", 665, 500, 24, 24, GRenderer, FontShadowedWhite, 1, 1, true);
		}
		/*if (GameOverCountDown > 0 && (int)round(GameOverCountDown * 100) % 40 >= 20)
		{
			DrawText("GAME OVER", 0, 0, 0, 0, GRenderer, FontSeg20White);
		}				*/		
		PresentBackBuffer();

		while (SDL_PollEvent(&TheEvent) != 0)
		{
			if (TheEvent.type == SDL_KEYDOWN)
			{
				if (TheEvent.key.keysym.scancode == SDL_SCANCODE_RETURN || TheEvent.key.keysym.scancode == SDL_SCANCODE_SPACE)
				{
					if (!bCountFinished)
					{
						GameOverCountDown = 0;
					}
					else
					{
						bDone = true;
					}
				}
			}
			if (TheEvent.type == SDL_QUIT)
			{
				bDone = true;
				bUserQuit = true;
				return;
			}
		}
	}	

	ReadHighScores();
	DoDisplayHighScore(GetHighScorePosition(TotalScore), TotalScore, TheGame->GetLevelNumber()-1);
}

void DrawTexture(SDL_Renderer *Renderer, SDL_Texture *Texture, int X, int Y, int Width, int Height)
{
	SDL_Rect Rect = { X, Y, Width, Height };
	SDL_RenderCopy(Renderer, Texture, NULL, &Rect);
}
void DoHowTo()
{
	enum eHowToState { HowToBricks = 0, HowToWalls, HowToMexicans, HowToPussy, HowToDestroy, HowToFinish }
	HowToState = HowToBricks;
	long TotalScore = 0;	
	bool bDone = false;
	double GameOverCountDown = 10;

	SDL_Event TheEvent;

	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = SDL_GetPerformanceCounter();
	double DeltaTime;

	while (!bDone)
	{
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
		{
			bDone = true;
			bUserQuit = true;
		}

		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());
		DeltaTime *= (double)0.001;

		GameOverCountDown -= DeltaTime;

		if (GameOverCountDown < 0)
		{
			bDone = true;
		}
		SDL_Rect BackBufferRect = { 0, 0, 1024, 600 };
		SDL_RenderCopy(GRenderer, ResourceManager::StarBGTexture->Texture, NULL, &BackBufferRect);

		DrawText("HOW TO PLAY", 300, 16, 32, 32, GRenderer, FontShadowedRed, 0.95, 0.95);

		SDL_Rect WallRect = { 118, 157, 769, 304 };

		RenderBrickRectangle(GRenderer, WallRect, true, 0, 11, 42);

		if (HowToState == HowToBricks)
		{
			HowToState = HowToWalls;
			Mix_PlayChannel(-1, PlaceWallFX, 0);
		}
		DrawText("COLLECT", 152, 180, 24, 24, GRenderer, FontShadowedWhite);
		DrawTexture(GRenderer, ResourceManager::BrickTexture->Texture, 360, 184, 24, 24);


		if (GameOverCountDown <= 9)
		{
			if (HowToState == HowToWalls)
			{
				HowToState = HowToMexicans;
				Mix_PlayChannel(-1, PlaceWallFX, 0);
			}
			DrawText("BUILD", 152, 220, 24, 24, GRenderer, FontShadowedWhite, 1, 1);
			DrawTexture(GRenderer, ResourceManager::WallTexture->Texture, 360, 216, 32, 32);
		}

		if (GameOverCountDown <= 8)
		{
			if (HowToState == HowToMexicans)
			{
				HowToState = HowToPussy;
				Mix_PlayChannel(-1, PlaceWallFX, 0);
			}

			DrawText("AVOID", 152, 260, 24, 24, GRenderer, FontShadowedWhite, 1, 1);
			DrawTexture(GRenderer, ResourceManager::MexicanFaceTexture->Texture, 360, 256, 32, 32);
		}

		if (GameOverCountDown <= 7)
		{
			if (HowToState == HowToMexicans)
			{
				HowToState = HowToPussy;
				Mix_PlayChannel(-1, PlaceWallFX, 0);
			}
			DrawText("GRAB", 152, 300, 24, 24, GRenderer, FontShadowedWhite, 1, 1);
			DrawTexture(GRenderer, ResourceManager::BrickTexture->Texture, 360, 296, 32, 32);

		}

		if (GameOverCountDown <= 6)
		{
			if (HowToState == HowToPussy)
			{
				HowToState = HowToDestroy;
				Mix_PlayChannel(-1, PlaceWallFX, 0);
			}

			/*DrawText("DESTROY", 152, 370, 24, 24, GRenderer, FontShadowedRed, 1, 1);
			DrawText("        A NATION", 152, 370, 24, 24, GRenderer, FontShadowedWhite, 1, 1);
			DrawText("          NATION", 152, 370, 24, 24, GRenderer, FontBlue, 1, 1);
			DrawTexture(GRenderer, ResourceManager::BrickTexture->Texture, 300, 340, 32, 32);*/

		}

		/*if (GameOverCountDown > 0 && (int)round(GameOverCountDown * 100) % 40 >= 20)
		{
		DrawText("GAME OVER", 0, 0, 0, 0, GRenderer, FontSeg20White);
		}				*/
		PresentBackBuffer();

		while (SDL_PollEvent(&TheEvent) != 0)
		{
			////User requests quit
			//if (TheEvent.type == SDL_KEYDOWN)
			//{
			//	bDone = true;
			//}
			if (TheEvent.type == SDL_QUIT)
			{
				bDone = true;
			}
		}
	}
}

int GetHighScorePosition(long Score)
{
	for (int i = 0; i < 10; i++)
	{
		if (Score > HighScores[i].Score)
		{
			return i;
		}
	}
	return -1;
}

void DoDisplayHighScore(int EnterRank, long Score, int Mile)
{
	bool bInputtingName = EnterRank != -1;	
	bool bDone = false;
	double HighScoreCountDown = HIGH_SCORE_DISPLAY_COUNT;
	int BrickX = 57;
	int BrickY = 134;
	int MoveY = 0;
	int MoveX = 0;
	int NamePos = 0;
	int CharIndex = 1;
	char *CharArray = " ABCDEFGHIJKLMNOPQRSTUVWXYZ.-?!";

	SDL_Event TheEvent;

	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = SDL_GetPerformanceCounter();
	double DeltaTime;	
	bool bButtonPressed = false;
	SDL_Joystick *Joy = SDL_JoystickOpen(0);
	SDL_SetRenderTarget(GRenderer, NULL);
	SDL_SetRenderDrawColor(GRenderer, 0, 0, 0, 255);
	SDL_RenderClear(GRenderer);
	SDL_SetRenderTarget(GRenderer, BackBuffer);
	ReadHighScores();
	
	if (EnterRank != -1)
	{
		for (int i = 9; i > EnterRank; i--)
		{
			HighScores[i] = HighScores[i - 1];

			strcpy(HighScores[i].Name, HighScores[i - 1].Name);
		}

		strcpy(HighScores[EnterRank].Name, "AAA");
		HighScores[EnterRank].Mile = Mile;
		HighScores[EnterRank].Score = Score;
		HighScores[EnterRank].Name[3] = 0;
	}

	while (!bDone)
	{
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
		{
			bDone = true;
			bUserQuit = true;
		}

		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());
		DeltaTime *= (double)0.001;
		Mexicans.Tick(DeltaTime);

		HighScoreCountDown -= DeltaTime;

		if (HighScoreCountDown < 0)
		{
			if (!bInputtingName)
			{
				bDone = true;
			}
			else
			{
				HighScoreCountDown = HIGH_SCORE_DISPLAY_COUNT;
			}
		}

		RenderStars(DeltaTime);
		if (!bInputtingName)
		{
			DrawText("GREATEST AMERICANS", 244, 16, 32, 32, GRenderer, FontShadowedRed, 0.95, 0.95);
		}
		else
		{
			DrawText("INPUT INITIALS!", 268, 16, 32, 32, GRenderer, FontShadowedRed, 0.95, 0.95);
		}
		//SDL_Rect WallRect = { 120, 109, 769, 432 };
		//RenderBrickRectangle(GRenderer, WallRect, false, 0, 11, 42);
		
		//DrawText("ABCDEFGHIJ\nKLMNOPQRST\nUVWXYZ.-/?\n!@$&*+_ ", 64, 100, 50, 50, GRenderer, FontSeg20White, 1, 1);
		
		DrawText("RANK", 174, 134, 32, 32, GRenderer, FontShadowedYellow);
		DrawText("SCORE", 351, 134, 32, 32, GRenderer, FontShadowedYellow);
		DrawText("MILE", 551, 134, 32, 32, GRenderer, FontShadowedYellow);
		DrawText("NAME", 725, 134, 32, 32, GRenderer, FontShadowedYellow, 1, 1);

		//Glyph *FontToUse;
		for (int i = 0; i < 10; i++)
		{
			int PosY = i * 34;
			
			/*if (i == 0)
			{
				FontToUse = FontToUse;
			}
			else if (i > 0 && i < 5)
			{
				Font = FontSegSmallWhite;
			}
			else if (i > 5 && i < 10)
			{
				Font = FontSegSmallBlue;
			}*/

			if (bInputtingName && i == EnterRank && ((int)round(HighScoreCountDown * 100) % 70) >= 60 )
			{
				continue;
			}

			/*if (i == EnterRank)
			{
				SDL_SetRenderDrawColor(GRenderer, 255, 255, 0, 255);
				SDL_RenderDrawLine(GRenderer, 647, 195 + PosY, 658, 195 + PosY);
				SDL_RenderDrawLine(GRenderer, 647, 196 + PosY, 658, 196 + PosY);
				SDL_RenderDrawLine(GRenderer, 647, 197 + PosY, 658, 197 + PosY);
			}*/

			if (i != 0)
			{
				DrawText(std::to_string(i + 1), 236, 190 + PosY, 32, 32, GRenderer, FontShadowedWhite, 0.75, 0.75, true);
			}

			DrawText(std::to_string(HighScores[i].Score), 508, 190 + PosY, 32, 32, GRenderer, FontShadowedWhite, 0.75, 0.75, true);
			DrawText(std::to_string(HighScores[i].Mile), 679, 190 + PosY, 32, 32, GRenderer, FontShadowedWhite, 0.75, 0.75, true);

			if (i == EnterRank && bInputtingName)
			{
				char Name[4];
				strcpy(Name, HighScores[i].Name);
				Name[NamePos + 1] = 0;
				DrawText(Name, 769, 190 + PosY, 32, 32, GRenderer, FontShadowedWhite, 0.75, 0.75);
				
			}
			else
			{
				DrawText(HighScores[i].Name, 849, 190 + PosY, 32, 32, GRenderer, FontShadowedWhite, 0.75, 0.75, true);
			}
		}

		//SDL_Rect Rect = { BrickX, BrickY, ResourceManager::BrickTexture->SrcRect.w, ResourceManager::BrickTexture->SrcRect.h };
		//SDL_RenderCopy(GRenderer, ResourceManager::BrickTexture->Texture, NULL, &Rect);

		SDL_Rect Rect = { 206, 198, ResourceManager::RedHatTexture->SrcRect.w * 2.5, ResourceManager::RedHatTexture->SrcRect.h * 2.5};
		SDL_RenderCopy(GRenderer, ResourceManager::RedHatTexture->Texture, NULL, &Rect);
		
		PresentBackBuffer();

		while (SDL_PollEvent(&TheEvent) != 0)
		{
			if (bInputtingName)
			{
				MoveY = 0;
				MoveX = 0;
				bButtonPressed = false;
				if (TheEvent.type == SDL_KEYDOWN)
				{
					if (TheEvent.key.state == SDL_PRESSED && (TheEvent.key.keysym.scancode == SDL_SCANCODE_SPACE || TheEvent.key.keysym.scancode == SDL_SCANCODE_RETURN))
					{
						bButtonPressed = true;
					}

					if (TheEvent.key.state == SDL_PRESSED && (TheEvent.key.keysym.scancode == SDL_SCANCODE_KP_8 || TheEvent.key.keysym.scancode == SDL_SCANCODE_UP))
					{
						MoveY = -1;
					}

					if (TheEvent.key.state == SDL_PRESSED && (TheEvent.key.keysym.scancode == SDL_SCANCODE_KP_2 || TheEvent.key.keysym.scancode == SDL_SCANCODE_DOWN))
					{
						MoveY = 1;
					}

					if (TheEvent.key.state == SDL_PRESSED && (TheEvent.key.keysym.scancode == SDL_SCANCODE_KP_4 || TheEvent.key.keysym.scancode == SDL_SCANCODE_LEFT))
					{
						MoveX = -1;
					}

					if (TheEvent.key.state == SDL_PRESSED && (TheEvent.key.keysym.scancode == SDL_SCANCODE_KP_6 || TheEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT))
					{
						MoveX = 1;
					}
				}

				if (TheEvent.type == SDL_JOYBUTTONDOWN)
				{
					if (TheEvent.jbutton.button == 0)
					{
						bButtonPressed = true;
					}
				}

				if (TheEvent.type == SDL_JOYAXISMOTION)
				{
					if (TheEvent.jaxis.axis == JOY_Y_AXIS)
					{
						if (TheEvent.jaxis.value > JOYSTICK_DEAD_ZONE)
						{
							MoveY = 1;
						}
						else if (TheEvent.jaxis.value < -JOYSTICK_DEAD_ZONE)
						{
							MoveY = -1;
						}
					}
				}

				if (bButtonPressed)
				{
					if (CharArray[CharIndex] == '\1')
					{
						HighScores[EnterRank].Name[NamePos] = 'A';
						NamePos--;
						CharIndex = 0;
						HighScores[EnterRank].Name[NamePos] = 'A';
					}
					else
					{
						Mix_PlayChannel(4, MenuSound2FX, 0);
						CharIndex = 1;
						NamePos++;

						if (NamePos < 3)
						{
							HighScores[EnterRank].Name[NamePos] = 'A';
						}
					}
					SDL_Log("NamePos: %d", NamePos);

					if (NamePos > 2)
					{
						bInputtingName = false;
						HighScoreCountDown = HIGH_SCORE_DISPLAY_COUNT;
					}
				}

				if (MoveY != 0)
				{
					Mix_PlayChannel(3, MenuSound1FX, 0);
					CharIndex += MoveY;

					if (CharIndex < 0)
					{
						CharIndex = strlen(CharArray) - 1;
					}

					if (CharIndex >= strlen(CharArray))
					{
						CharIndex = 0;
					}

					HighScores[EnterRank].Name[NamePos] = CharArray[CharIndex];
				}

				/*if (MoveY != 0)
				{
					BrickY += MoveY * 50;

					if (BrickY < 134)
					{
						BrickY = 134 + 50 * 3;
					}

					if (BrickY > 134 + 50 * 3)
					{
						BrickY = 134;
					}
				}

				if (MoveX != 0)
				{
					BrickX += MoveX * 50;

					if (BrickX < 57)
					{
						BrickX = 57 + 50 * 9;
					}

					if (BrickX > 57 + 50 * 9)
					{
						BrickX = 57;
					}
				}*/
			}
			else
			{
				if (TheEvent.type == SDL_KEYDOWN)
				{
					if (TheEvent.key.keysym.scancode == SDL_SCANCODE_RETURN || TheEvent.key.keysym.scancode == SDL_SCANCODE_SPACE)
					{
						bDone = true;
					}
				}
			}
			if (TheEvent.type == SDL_QUIT)
			{
				bDone = true;
				bUserQuit = true;				
			}
		}
	}

	WriteHighScores();
	SDL_JoystickClose(Joy);
}

void ReadHighScores()
{
	fstream HighScoreFile;
	HighScoreFile.open("data.dat", ios_base::in);

	for (int i = 0; i < 10; i++)
	{
		string Line;
		char Dummy;
		
		HighScoreFile >>HighScores[i].Score >> HighScores[i].Mile;
		HighScoreFile.get();

		for (int j = 0; j < 3; j++)
		{
			HighScores[i].Name[j] = HighScoreFile.get();
		}
	}

	HighScoreFile.close();
}

void WriteHighScores()
{
	FILE *HighScoreFile = fopen("data.dat", "w");

	for (int i = 0; i < 10; i++)
	{
		fprintf(HighScoreFile, "%ld\n%d\n%s\n", HighScores[i].Score, HighScores[i].Mile, HighScores[i].Name);
	}

	fclose(HighScoreFile);
}

void ReadGreatFacts()
{
	/*fstream FactFile;

	SDL_Log("Before open fact file");
	FactFile.open("data2.dat");
	SDL_Log("After open fact file");
	while (!FactFile.eof())
	{
		string NewString;
		getline(FactFile, NewString);
		SDL_Log("Read line");
		if (NewString == "")
		{
			break;
		}
		GreatFacts.push_back(NewString);
	}
	
	SDL_Log("Before fact file close");
	FactFile.close();
	SDL_Log("After open fact file");*/
}

void DisplayGreatFact()
{	
	bool bDone = false;
	double FactCountDown = FACT_TIME;

	SDL_Event TheEvent;
	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = SDL_GetPerformanceCounter();
	double DeltaTime;
	int FactIndex = rand() % GreatFacts.size();

	while (!bDone)
	{
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
		{
			bDone = true;
			bUserQuit = true;
		}

		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());
		DeltaTime *= (double)0.001;

		FactCountDown -= DeltaTime;

		if (FactCountDown < 0)
		{
			bDone = true;
		}

		SDL_SetRenderDrawColor(GRenderer, 0, 0, 0, 255);
		SDL_RenderClear(GRenderer);
		DrawText("GREAT FACTS", 390, 16, 0, 0, GRenderer, FontSeg36White, 1, 1);
		DrawText(GreatFacts[FactIndex], 16, 160, 0, 0, GRenderer, FontSegSmallWhite, 1, 1);
		PresentBackBuffer();

		while (SDL_PollEvent(&TheEvent) != 0)
		{
			////User requests quit
			//if (TheEvent.type == SDL_KEYDOWN)
			//{
			//	bDone = true;
			//}
			if (TheEvent.type == SDL_QUIT)
			{
				bUserQuit = true;
				bDone = true;
			}
		}
	}
}

void RenderStars(float DeltaTime)
{
	StarScroll += DeltaTime * 150;

	if (StarScroll > 512)
	{
		StarScroll = 0;
	}

	SDL_Rect StarSrcRect = { StarScroll, 0, 512 - StarScroll, 300 };
	SDL_Rect StarDstRect = { 0, 0, StarSrcRect.w * 2, 600 };
	SDL_RenderCopy(GRenderer, ResourceManager::StarBGTexture->Texture, &StarSrcRect, &StarDstRect);

	StarSrcRect = { 0, 0, (int)round(StarScroll), 300 };
	StarDstRect = { (int)round(1024 - StarScroll * 2), 0, (int)round(StarSrcRect.w * 2), 600 };

	SDL_RenderCopy(GRenderer, ResourceManager::StarBGTexture->Texture, &StarSrcRect, &StarDstRect);
}

void RenderBrickRectangle(SDL_Renderer *Renderer, SDL_Rect &DrawRect, bool bLevelBG, int R, int G, int B)
{
	if (bLevelBG && DrawRect.h > 128)
	{
		SDL_Rect BGSrcRect = DrawRect;
		SDL_Rect BGDstRect = DrawRect;

		// Draw sky
		BGSrcRect.x = 0;
		BGSrcRect.y = 0;	
		BGSrcRect.h = 128;

		BGDstRect.h = 128;
		SDL_RenderCopy(Renderer, ResourceManager::BGTexture->Texture, &BGSrcRect, &BGDstRect);
		
		// Draw mountains and terrain
		BGSrcRect = DrawRect;
		BGDstRect = DrawRect;

		BGSrcRect.x = 0;
		BGSrcRect.y = 0;
		BGSrcRect.h -= 128;

		BGDstRect.y += 128;
		BGDstRect.h -= 128;
		
		SDL_Texture *BGTexture = TheGame->GetGroundTexture();
		SDL_RenderCopy(Renderer, BGTexture, &BGSrcRect, &BGDstRect);

		// Now draw the top of the hill....
		BGSrcRect = DrawRect;
		BGDstRect = DrawRect;
		BGSrcRect.x = 0;
		BGSrcRect.y = 256;
		BGSrcRect.h = 64;

		BGDstRect.y += 192;
		BGDstRect.h = 64;
		SDL_RenderCopy(Renderer, ResourceManager::BGTexture->Texture, &BGSrcRect, &BGDstRect);

		SDL_SetRenderDrawColor(Renderer, 0, 11, 42, 128);
		SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(Renderer, &DrawRect);
		SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);
	}
	else if (R != -1)
	{
		SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
		
		SDL_RenderFillRect(Renderer, &DrawRect);
	}
	
	int StartX = DrawRect.x;
	int StopX = DrawRect.x + DrawRect.w + 1;

	int StartY = DrawRect.y;
	int StopY = DrawRect.y + DrawRect.h ;
	
	for (int i = StartX; i < StopX; i += ResourceManager::SingleBrickTexture->SrcRect.w)
	{
		SDL_Rect DstRect = { i, StartY, ResourceManager::SingleBrickTexture->SrcRect.w, ResourceManager::SingleBrickTexture->SrcRect.h };
		SDL_RenderCopy(GRenderer, ResourceManager::SingleBrickTexture->Texture, NULL, &DstRect);

		DstRect.y += StopY - StartY;
		SDL_RenderCopy(GRenderer, ResourceManager::SingleBrickTexture->Texture, NULL, &DstRect);
	}

	for (int i = StartY; i < StopY; i += ResourceManager::SmallBrickTileTexture->SrcRect.h)
	{
		SDL_Rect DstRect = { StartX - 1, i, ResourceManager::SmallBrickTileTexture->SrcRect.w, ResourceManager::SmallBrickTileTexture->SrcRect.h };
		SDL_RenderCopy(GRenderer, ResourceManager::SmallBrickTileTexture->Texture, NULL, &DstRect);
		DstRect.x += StopX - StartX;
		SDL_RenderCopy(GRenderer, ResourceManager::SmallBrickTileTexture->Texture, NULL, &DstRect);
	}
}