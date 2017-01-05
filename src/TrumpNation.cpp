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

struct Glyph
{
	int Width;
	int Height;
	SDL_Texture *Texture;
};

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
Mix_Music *TitleMusic = NULL;
Game *TheGame;
TrumpPlayerSprite *ThePlayer;
Glyph Numerals36[10];
Glyph Numerals20[10];

void GameLoop();
void DoTitleScreen();
void Tick(double DeltaTime);
void Render();
void InitSDL();
void CleanUp();
void DrawHUD(SDL_Renderer *Renderer);
void LoadNumerals(const char *FontName, int Point, Glyph Glyphs[10]);

int main(int argc, char ** argv)
{
	// variables		

	// init SDL	
	InitSDL();

	SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
	
	GResourceManager = new ResourceManager;

	DoTitleScreen();
	GameLoop();
	
	CleanUp();	

	return 0;
}

void CleanUp()
{
	delete GResourceManager;

	Mix_FreeChunk(PlaceWallFX);
	Mix_FreeChunk(PickUpItemFX);	
	Mix_FreeChunk(StepFX);

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

void GameLoop()
{
	SDL_Event TheEvent;
	TickFreq = SDL_GetPerformanceFrequency();
	bool bGameComplete = false;
	ThePlayer = new TrumpPlayerSprite();
	ThePlayer->PlayAnimation(ResourceManager::TrumpAnimation);
	TheGame = new Game();
	TheGame->SetLevel(1);

	while (!bGameComplete)
	{
		ThePlayer->Reset();
		Items.push_back(new BrickItem(470, 570));
		SDL_SetTextureAlphaMod(ResourceManager::ShadowTexture->Texture, 128);
		SDL_Log("Mile: %d, Rate: %f", TheGame->GetLevelNumber(), MEXICAN_SPAWN_RATE / (TheGame->GetLevelNumber() / (double)4));
		Uint64 StartTime = SDL_GetPerformanceCounter();
		Uint64 CurrentTime = SDL_GetPerformanceCounter();
		double DeltaTime;
		bool bDone = false;

		while (!bGameComplete && !bDone)
		{
			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
			{
				bGameComplete = true;
				
			}

			if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
			{
				TheGame->SetLevel(TheGame->GetLevelNumber()+1);
				while (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) { SDL_PollEvent(&TheEvent); }
			}

			StartTime = CurrentTime;
			CurrentTime = SDL_GetPerformanceCounter();
			DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());
			Tick(DeltaTime * (double)0.001);

			if (TheGame->LevelComplete() || ThePlayer->GetPlayerState() == StateDead)
			{
				bDone = true;
			}

			Render();

			//Handle events on queue
			while (SDL_PollEvent(&TheEvent) != 0)
			{
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
}

void Tick(double DeltaTime)
{	
	static double SpawnCountdown = MEXICAN_SPAWN_RATE / (TheGame->GetLevelNumber() / (double)4);// 0.15;
	SpawnCountdown -= DeltaTime;

	if (SpawnCountdown <= 0)
	{
		Mexicans.push_back(new Mexican1Sprite());
		SpawnCountdown = MEXICAN_SPAWN_RATE / (TheGame->GetLevelNumber() / (double)4);// 0.15;
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

	for (int WallIndex = 0; WallIndex < 16; WallIndex++)
	{
		Rect.x = WallIndex * 64;
		Rect.y = WALL_TOP;
		Rect.w = 64;
		Rect.h = 160;

		if (TheGame->WallArray[WallIndex])
		{
			SDL_RenderCopy(GRenderer, ResourceManager::WallTexture->Texture, NULL, &Rect);
		}
	}

	Items.Render(GRenderer);
	Mexicans.Render(GRenderer);

	ThePlayer->Render(GRenderer);
	DrawHUD(GRenderer);
	SDL_RenderPresent(GRenderer);

}

void DoTitleScreen()
{
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
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN])
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
		SDL_RenderPresent(GRenderer);

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
		}

		GWindow = SDL_CreateWindow("Trump Nation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, SDL_WINDOW_OPENGL);
		GRenderer = SDL_CreateRenderer(GWindow, -1, 0);
		SDL_Log("After create window");
		LoadNumerals("resource/fonts/segoeuib.ttf", 36, Numerals36);
		LoadNumerals("resource/fonts/segoeuib.ttf", 28, Numerals20);
		SDL_Log("After load numerals");
		bSDLInitialized = true;
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

void DrawText(string Text, int X, int Y, int SizeX, int SizeY, SDL_Renderer *Renderer, Glyph Glyphs[10])
{
	for (int i = 0; i < Text.size(); i++)
	{
		char CharToRender = Text.at(i) - '0';
		if (CharToRender >= 0 && CharToRender < 10)
		{
			SDL_Rect SrcRect = { 0, 0, Glyphs[CharToRender].Width, Glyphs[CharToRender].Height };
			SDL_Rect DstRect = { X + i * Glyphs[CharToRender].Width, Y, Glyphs[CharToRender].Width, Glyphs[CharToRender].Height };

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

void LoadNumerals(const char *FontName, int Point, Glyph Glyphs[10])
{
	SDL_Color Black = { 0, 0, 0, 0 };	
	TTF_Font *Font = TTF_OpenFont(FontName, Point);
	
	for (int i = 0; i < 10; i++)
	{
		Uint32 OutFormat;
		int OutAccess;

		SDL_Surface *NumeralSurface = TTF_RenderGlyph_Blended(Font, '0' + i, Black);
		Glyphs[i].Texture = SDL_CreateTextureFromSurface(GRenderer, NumeralSurface);
		SDL_QueryTexture(Glyphs[i].Texture, &OutFormat, &OutAccess, &Glyphs[i].Width, &Glyphs[i].Height);
		SDL_FreeSurface(NumeralSurface);
	}

	TTF_CloseFont(Font);
}
