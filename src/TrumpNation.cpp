#ifdef _WIN32
	#include <SDL.h>
#else
	#include "SDL2/SDL.h"
#endif

#include "../inc/ResourceManager.h"
#include "../inc/Sprite.h"

ResourceManager *ResourceManager::Instance = NULL;
SDL_Renderer * GRenderer;

void GameLoop();
void Tick(double DeltaTime);
void Render();

int main(int argc, char ** argv)
{
	// variables	

	// init SDL
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("SDL2 Keyboard/Mouse events", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	GRenderer = SDL_CreateRenderer(window, -1, 0);	
	
	SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
	
	GameLoop();
	
	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void GameLoop()
{
	bool bDone = false;
	double StartTime = 0;
	StartTime = SDL_GetTicks();
	SDL_Event TheEvent;

	while (!bDone)
	{
		double DeltaTime = SDL_GetTicks() - StartTime;
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
}

void Render()
{
	Sprite Test;	
	Test.SetFrame(ResourceManager::GetInstance()->Blah.Texture);
	Test.SetPosition(20, 20);
	SDL_RenderClear(GRenderer);	
	Test.Render(GRenderer);
	SDL_RenderPresent(GRenderer);

}