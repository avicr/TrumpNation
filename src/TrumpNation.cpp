#ifdef _WIN32
	#include <SDL.h>
#else
	#include "SDL2/SDL.h"
#endif

#include "../inc/ResourceManager.h"
#include "../inc/Sprite.h"

ResourceManager *TheResourceManager;
SDL_Renderer * GRenderer;

int main(int argc, char ** argv)
{
	// variables
		
	bool quit = false;
	SDL_Event event;
	int x = 288;
	int y = 208;

	// init SDL
	Sprite TestSprite;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("SDL2 Keyboard/Mouse events",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	GRenderer = SDL_CreateRenderer(window, -1, 0);

	TheResourceManager = new ResourceManager(GRenderer);

	SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);

	// handle events

	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:  x--; break;
			case SDLK_RIGHT: x++; break;
			case SDLK_UP:    y--; break;
			case SDLK_DOWN:  y++; break;
			}
			break;
		}

		SDL_Rect dstrect = { x, y, 64, 64 };

		SDL_RenderClear(GRenderer);
		SDL_RenderCopy(GRenderer, TheResourceManager->Blah.Texture, NULL, &dstrect);
		SDL_RenderPresent(GRenderer);
	}

	// cleanup SDL
	
	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}