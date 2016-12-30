#ifdef _WIN32
	#include <SDL.h>
#else
	#include "SDL2/SDL.h"
#endif

#include "../inc/ResourceManager.h"
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/Mexican1Sprite.h"

SDL_Window *GWindow;
SDL_Renderer *GRenderer;
ResourceManager *GResourceManager;
bool bSDLInitialized = false;
TrumpPlayerSprite *TestSprite;
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

	TestSprite = new TrumpPlayerSprite();
	TestSprite->PlayAnimation(ResourceManager::TrumpAnimation);

	SDL_SetTextureAlphaMod(ResourceManager::ShadowTexture->Texture, 128);

	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = SDL_GetPerformanceCounter();
	double DeltaTime;
	while (!bDone)
	{
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
		{
			bDone = true;
		}
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

vector <Mexican1Sprite*> Mexicans;
void Tick(double DeltaTime)
{	
	static double SpawnCountdown = 1;
	SpawnCountdown -= DeltaTime;

	if (SpawnCountdown <= 0)
	{
		Mexicans.push_back(new Mexican1Sprite());
		SpawnCountdown = 1;
	}

	TestSprite->Tick(DeltaTime);
	
	for (int i = Mexicans.size() - 1; i >= 0; i--)
	{
		Mexicans[i]->Tick(DeltaTime);
	}
}

void Render()
{
	SDL_SetRenderDrawColor(GRenderer, 217, 201, 124, 255);
	
	SDL_Rect Rect = { 0, 0, 1024, 264 };
	//SDL_RenderCopy(GRenderer, ResourceManager::BackgroundTexture->Texture, &Rect, &Rect);
	SDL_RenderClear(GRenderer);
	SDL_SetRenderDrawColor(GRenderer, 0, 162, 232, 255);
	SDL_RenderFillRect(GRenderer, &Rect);
	TestSprite->Render(GRenderer);
	for (int i = Mexicans.size() - 1; i >= 0; i--)
	{
		Mexicans[i]->Render(GRenderer);
	}
	//SDL_RenderCopy(GRenderer, ResourceManager::ShadowTexture->Texture, &Rect, &Rect);
	if (Mexicans.size())
	{
		Mexicans[0]->SetPosition(0, 600 - 136 * 0.58);
		Mexicans[0]->Render(GRenderer);
	}
	SDL_RenderPresent(GRenderer);

}

void InitSDL()
{
	if (!bSDLInitialized)
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
		GWindow = SDL_CreateWindow("Trump Nation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, SDL_WINDOW_OPENGL);
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
