#ifdef _WIN32
	#include <SDL.h>
#else
	#include "SDL2/SDL.h"
#endif

#include "../inc/ResourceManager.h"
#include "../inc/Sprite.h"

SDL_Window *GWindow;
SDL_Renderer *GRenderer;
ResourceManager *GResourceManager;
bool bSDLInitialized = false;
Sprite TestSprite;
Sprite TestMexicanSprite;
Uint64 TickFreq;

void GameLoop();
void Tick(double DeltaTime);
void Render();
void InitSDL();

int main(int argc, char ** argv)
{
	// variables		

	// init SDL	
	InitSDL();

	SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
	
	GResourceManager = new ResourceManager;

	GameLoop();
	
	delete GResourceManager;
	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(GWindow);
	SDL_Quit();

	return 0;
}

void GameLoop()
{
	bool bDone = false;
	SDL_Event TheEvent;
	TickFreq = SDL_GetPerformanceFrequency();
	TestSprite.PlayAnimation(ResourceManager::TrumpAnimation);
	TestMexicanSprite.SetPosition(100, 100);
	TestMexicanSprite.PlayAnimation(ResourceManager::Mexican1Animation);

	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = 0;
	double DeltaTime;
	while (!bDone)
	{
		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());

		
		Tick(DeltaTime * (double)0.001);
		Render();	

		//Handle events on queue
		while (SDL_PollEvent(&TheEvent) != 0)
		{
			//User requests quit
			if (TheEvent.type == SDL_QUIT)
			{
				bDone = true;
			}
		}
	}
}

void Tick(double DeltaTime)
{	
	TestSprite.Tick(DeltaTime);
	TestMexicanSprite.Tick(DeltaTime);
}

void Render()
{
	SDL_SetRenderDrawColor(GRenderer, 64, 64, 64, 255);
	SDL_RenderClear(GRenderer);	
	
	TestSprite.Render(GRenderer);
	TestMexicanSprite.Render(GRenderer);
	//SDL_Rect Rect = { 32, 32, 32, 32 };
//	SDL_RenderCopy(GRenderer, ResourceManager::TrumpSpriteSheet->Texture, NULL, &Rect);
	SDL_RenderPresent(GRenderer);

}

void InitSDL()
{
	if (!bSDLInitialized)
	{
		SDL_Init(SDL_INIT_VIDEO);
		GWindow = SDL_CreateWindow("Trump Nation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, 0);
		GRenderer = SDL_CreateRenderer(GWindow, -1, 0);
		
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