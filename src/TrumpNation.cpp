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

struct Glyph
{
	int Width;
	int Height;
	SDL_Texture *Texture;
};

bool bFreezeSpawn = false;
SDL_Window *GWindow;
SDL_Renderer *GRenderer;
ResourceManager *GResourceManager;
bool bSDLInitialized = false;
Uint64 TickFreq;
SpriteList Items;
SpriteList Mexicans;
Mix_Chunk *PickUpItemFX = NULL;
Mix_Chunk *PlaceWallFX = NULL;
Mix_Chunk *StepFX = NULL;
Mix_Chunk *TitleConfirmFX = NULL;
Mix_Chunk *TrumpDieFX = NULL;
Mix_Music *TitleMusic = NULL;
Mix_Music *BGMusic = NULL;
Mix_Music *HatDanceMusic = NULL;
Game *TheGame;
TrumpPlayerSprite *ThePlayer;
Glyph Numerals36[10];
Glyph Numerals20[10];
Glyph NumeralsWhite20[10];
SDL_Texture *BackBuffer;
int WindowWidth;
int WindowHeight;
bool bSwapSprites = false;
double BombCountDown = 0;

bool GameLoop();
bool DoTitleScreen();
void Tick(double DeltaTime);
void Render();
void InitSDL();
void CleanUp();
void DrawHUD(SDL_Renderer *Renderer);
void LoadNumerals(const char *FontName, int Point, Glyph Glyphs[10], SDL_Color Color = { 0, 0, 0, 255 });
void PresentBackBuffer();
void SpawnRandomItem();
double GetSpawnTime();

int main(int argc, char ** argv)
{
	// variables		

	// init SDL	
	InitSDL();

	SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
	
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

	Mix_FreeMusic(HatDanceMusic);
	Mix_FreeMusic(BGMusic);
	Mix_FreeChunk(PlaceWallFX);
	Mix_FreeChunk(PickUpItemFX);	
	Mix_FreeChunk(StepFX);
	Mix_FreeChunk(TitleConfirmFX);
	Mix_FreeChunk(TrumpDieFX);

	for (int i = 0; i < 10; i++)
	{
		SDL_DestroyTexture(Numerals20[i].Texture);
		SDL_DestroyTexture(Numerals36[i].Texture);
	}

	TTF_Quit();

	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(GWindow);
	SDL_Quit();
}

bool GameLoop()
{
	bool bUserQuit = false;
	SDL_Event TheEvent;
	TickFreq = SDL_GetPerformanceFrequency();
	bool bGameComplete = false;
	ThePlayer = new TrumpPlayerSprite();
	ThePlayer->PlayAnimation(ResourceManager::TrumpAnimation);
	TheGame = new Game();
	TheGame->SetLevel(1);

	while (!bGameComplete)
	{
		if (!Mix_PlayingMusic())
		{
			Mix_PlayMusic(BGMusic, -1);
		}

		ThePlayer->Reset();
		BrickItem *FirstBrick = new BrickItem();		
		FirstBrick->SetPosition(470, 570);
		Items.push_back(FirstBrick);		
		SDL_SetTextureAlphaMod(ResourceManager::ShadowTexture->Texture, 128);
		SDL_Log("Mile: %d, Rate: %f", TheGame->GetLevelNumber(), GetSpawnTime());
		Uint64 StartTime = SDL_GetPerformanceCounter();
		Uint64 CurrentTime = SDL_GetPerformanceCounter();
		double DeltaTime;
		bool bLevelComplete = false;

		while (!bGameComplete && !bLevelComplete)
		{
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
			{
				bGameComplete = true;
				bUserQuit = true;
			}

			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
			{
				TheGame->SetLevel(TheGame->GetLevelNumber()+1);
				while (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) { SDL_PollEvent(&TheEvent); }
				SDL_Log("Mile: %d, Rate: %f", TheGame->GetLevelNumber(), GetSpawnTime());
			}

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
				//SDL_Log("Event Type: %d", TheEvent.type);
				if (TheEvent.type == SDL_MOUSEBUTTONDOWN)
				{
					ThePlayer->SetPosition(TheEvent.button.x, TheEvent.button.y);
				}
				//User requests quit
				if (TheEvent.type == SDL_QUIT)
				{
					bGameComplete = true;
				}
			}
		}
		
		if (ThePlayer->GetNumLives() < 0)
		{
			bGameComplete = true;
		}
		Mexicans.DeleteAll();
		Items.DeleteAll();
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

	if (ItemSprite::NumNonBrickItems == 0)
	{
		ItemSpawnCountdown -= DeltaTime;
	}
	SpawnCountdown -= DeltaTime;

	if (SpawnCountdown <= 0 && !bFreezeSpawn && ThePlayer->GetPlayerState() != StateDying)
	{						
		Mexicans.push_back(new Mexican1Sprite());
		SpawnCountdown = GetSpawnTime();
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
				ItemChance == 100;
			}
			SDL_Log("Item miss, chance is now: 1 in %d", 100 / ItemChance);
		}
	}

	ThePlayer->Tick(DeltaTime);
	
	if (ThePlayer->GetPlayerState() != StateDead && ThePlayer->GetPlayerState() != StateDying)
	{
		Mexicans.Tick(DeltaTime);
		Items.Tick(DeltaTime);
	}
}

void Render()
{
	// Clear to ground color
	SDL_SetRenderDrawColor(GRenderer, 217, 201, 124, 255);
	SDL_RenderClear(GRenderer);

	//SDL_RenderCopy(GRenderer, ResourceManager::BackgroundTexture->Texture, &Rect, &Rect);

	// Render sky
	SDL_Rect Rect = { 0, 0, 1024, HORIZON };
	SDL_SetRenderDrawColor(GRenderer, 0, 162, 232, 255);
	SDL_RenderFillRect(GRenderer, &Rect);

	DrawHUD(GRenderer);

	for (int WallIndex = 0; WallIndex < 16; WallIndex++)
	{
		Rect.x = WallIndex * 64;
		Rect.y = WALL_TOP;
		Rect.w = 64;
		Rect.h = 160;

		if (TheGame->WallArray[WallIndex])
		{
			SDL_RenderCopy(GRenderer, ResourceManager::WallTexture->Texture, NULL, &Rect);
			
			/*if (TheGame->WallArray[WallIndex] > 1)
			{				
				Rect.y -= 160;				
				SDL_RenderCopy(GRenderer, ResourceManager::WallTexture->Texture, NULL, &Rect);				
			}*/
		}
	}
	/*Rect = { 0, HORIZON + 65, 992, 600 - (600-HORIZON) - 21};
	SDL_SetRenderDrawColor(GRenderer, 255, 0, 0, 100);
	SDL_RenderFillRect(GRenderer, &Rect);*/
	
	//new BrickItem(rand() % 992, (rand() % (200) + HORIZON + 65)));
	Items.Render(GRenderer);
	Mexicans.Render(GRenderer);

	ThePlayer->Render(GRenderer);	

	if (BombCountDown > 0 && (int)round(BombCountDown * 100) % 4 >= 2)
	{
		SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
		SDL_RenderClear(GRenderer);
	}
	PresentBackBuffer();
}

bool DoTitleScreen()
{
	bool bUserQuit = false;
	bool bDone = false;
	SDL_Event TheEvent;
	TitleMusic = Mix_LoadMUS("resource/sounds/Title.wav");
	Mix_PlayMusic(TitleMusic, -1);
	Sprite *TrumpIntroSprite = new Sprite();
	TrumpIntroSprite->SetPosition(445, 300);
	TrumpIntroSprite->PlayAnimation(ResourceManager::TrumpIntroAnimation);
	TrumpIntroSprite->SetWidth(128);
	TrumpIntroSprite->SetHeight(128);

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

		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_SPACE])
		{
			bDone = true;
		}
		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());


		TrumpIntroSprite->Tick(DeltaTime * (double)0.001);

		SDL_Rect TitleRect = { 0, 0, 1024, 600 };
		SDL_RenderCopy(GRenderer, ResourceManager::TitleScreenTexture->Texture, &TitleRect, &TitleRect);
		TrumpIntroSprite->Render(GRenderer);
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

	
	Mix_HaltMusic();	
	Mix_FreeMusic(TitleMusic);

	delete TrumpIntroSprite;

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
			PickUpItemFX = Mix_LoadWAV("resource/sounds/Pickupitem.wav");
			PlaceWallFX = Mix_LoadWAV("resource/sounds/Placewall.wav");
			StepFX = Mix_LoadWAV("resource/sounds/Step.wav");
			TitleConfirmFX = Mix_LoadWAV("resource/sounds/Titleconfirm.wav");
			TrumpDieFX = Mix_LoadWAV("resource/sounds/Trumpdie.wav");
		}

		GWindow = SDL_CreateWindow("Trump Nation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_GetWindowSize(GWindow, &WindowWidth, &WindowHeight);
		GRenderer = SDL_CreateRenderer(GWindow, -1, 0);
		BackBuffer = SDL_CreateTexture(GRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 600);
		SDL_SetRenderTarget(GRenderer, BackBuffer);
		SDL_Log("After create window");
		LoadNumerals("resource/fonts/segoeuib.ttf", 36, Numerals36);
		LoadNumerals("resource/fonts/segoeuib.ttf", 28, Numerals20);
		LoadNumerals("resource/fonts/segoeuib.ttf", 28, NumeralsWhite20, { 255, 255, 255, 255 });
		SDL_Log("After load numerals");
		bSDLInitialized = true;
		SDL_ShowCursor(SDL_DISABLE);
		BGMusic = Mix_LoadMUS("resource/sounds/BGMusic.ogg");
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

void DrawText(string Text, int X, int Y, int SizeX, int SizeY, SDL_Renderer *Renderer, Glyph Glyphs[10], float ScaleX, float ScaleY)
{
	for (int i = 0; i < Text.size(); i++)
	{
		char CharToRender = Text.at(i) - '0';
		if (CharToRender >= 0 && CharToRender < 10)
		{
			SDL_Rect SrcRect = { 0, 0, Glyphs[CharToRender].Width, Glyphs[CharToRender].Height };
			SDL_Rect DstRect = { X + i * (Glyphs[CharToRender].Width * ScaleX), Y, Glyphs[CharToRender].Width * ScaleX, Glyphs[CharToRender].Height * ScaleY };

			SDL_RenderCopy(Renderer, Glyphs[CharToRender].Texture, &SrcRect, &DstRect);
		}
	}
}

void DrawHUD(SDL_Renderer *Renderer)
{
	SDL_Rect HUDRect = { 0, 0, 1024, 86 };
	SDL_RenderCopy(GRenderer, ResourceManager::HUDTexture->Texture, &HUDRect, &HUDRect);
	DrawText(std::to_string(TheGame->GetLevelNumber()), 535, 17, 32, 32, Renderer, Numerals36);
	DrawText(std::to_string(ThePlayer->GetScore()), 918, 3, 18, 32, Renderer, Numerals20);
	DrawText(std::to_string(ThePlayer->GetNumLives()), 98, 3, 18, 32, Renderer, Numerals20);
}

void LoadNumerals(const char *FontName, int Point, Glyph Glyphs[10], SDL_Color Color)
{	
	TTF_Font *Font = TTF_OpenFont(FontName, Point);
	
	for (int i = 0; i < 10; i++)
	{
		Uint32 OutFormat;
		int OutAccess;

		SDL_Surface *NumeralSurface = TTF_RenderGlyph_Blended(Font, '0' + i, Color);
		Glyphs[i].Texture = SDL_CreateTextureFromSurface(GRenderer, NumeralSurface);
		SDL_QueryTexture(Glyphs[i].Texture, &OutFormat, &OutAccess, &Glyphs[i].Width, &Glyphs[i].Height);
		SDL_FreeSurface(NumeralSurface);
	}

	TTF_CloseFont(Font);
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
	int Roll = rand() % 100;
	ItemSprite *NewItem;

	if (Roll < 20)
	{
		NewItem = new SwapItem();
	}
	else if (Roll < 55)
	{
		NewItem = new BombItem();
	}
	else 
	{
		NewItem = new RedHatItem();
	}	

	Items.push_back(NewItem);
}

double GetSpawnTime()
{
	if (TheGame->GetLevelNumber() == 1)
	{
		return 1;
	}
	else if (TheGame->GetLevelNumber() == 2)
	{
		return 0.85;
	}
	else if (TheGame->GetLevelNumber() == 3)
	{
		return 0.75;
	}
	else if (TheGame->GetLevelNumber() == 4)
	{
		return 0.65;
	}
	else if (TheGame->GetLevelNumber() == 5)
	{
		return 0.50;
	}
	else if (TheGame->GetLevelNumber() == 6)
	{
		return 0.35;
	}
	else if (TheGame->GetLevelNumber() == 7)
	{
		return 0.20;
	}
	else
	{
		return 1 / log1p(TheGame->GetLevelNumber() * TheGame->GetLevelNumber() * TheGame->GetLevelNumber());
	}
}