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
	double StartTime = 0;
	StartTime = SDL_GetTicks();
	SDL_Event TheEvent;

	TestSprite.PlayAnimation(ResourceManager::TrumpAnimation);

	while (!bDone)
	{
		double DeltaTime = SDL_GetTicks() - StartTime;
		if (DeltaTime == 0)
		{
			SDL_Delay(12);
		}
		DeltaTime = SDL_GetTicks() - StartTime;
		StartTime = SDL_GetTicks();
		Tick(DeltaTime / 1000);
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
	SDL_Log("DeltaTime %f", DeltaTime);
	TestSprite.Tick(DeltaTime);
}

void Render()
{
	
	SDL_RenderClear(GRenderer);	
	
	TestSprite.Render(GRenderer);
	//SDL_Rect Rect = { 32, 32, 32, 32 };
//	SDL_RenderCopy(GRenderer, ResourceManager::TrumpSpriteSheet->Texture, NULL, &Rect);
	SDL_RenderPresent(GRenderer);

}

void InitSDL()
{
	if (!bSDLInitialized)
	{
		SDL_Init(SDL_INIT_VIDEO);
		GWindow = SDL_CreateWindow("SDL2 Keyboard/Mouse events", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, 0);
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