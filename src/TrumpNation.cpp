#ifdef _WIN32
	#include <SDL.h>
#else
	#include "SDL2/SDL.h"
#endif

#include "../inc/ResourceManager.h"
#include "../inc/Sprite.h"

ResourceManager *TheResourceManager;

int main(int argc, char ** argv)
{
	// variables
	
	TheResourceManager->ThingTest.Blah();
	bool quit = false;
	SDL_Event event;
	int x = 288;
	int y = 208;

	// init SDL
	Sprite TestSprite;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window * window = SDL_CreateWindow("SDL2 Keyboard/Mouse events",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	SDL_Renderer * Renderer = SDL_CreateRenderer(window, -1, 0);

	TheResourceManager = new ResourceManager(Renderer);

	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

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
			case SDLK_RIGHT: x+= TheResourceManager->ThingTest.Blah(); break;
			case SDLK_UP:    y--; break;
			case SDLK_DOWN:  y++; break;
			}
			break;
		}

		SDL_Rect dstrect = { x, y, 64, 64 };

		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, TheResourceManager->SpaceShipTexture, NULL, &dstrect);
		SDL_RenderPresent(Renderer);
	}

	// cleanup SDL
	
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}