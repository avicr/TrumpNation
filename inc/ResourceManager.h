#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif
#include <string>

using namespace std;

// In a hurry here, no time to be fancy (should be a singleton but whateves). 
// This class holds hardcoded references to each asset.
class ResourceManager
{
	class Test
	{
	public:
		int Blah()
		{
			return 100;
		}
	};
public:
	static SDL_Texture *SpaceShipTexture;
	Test ThingTest;
	ResourceManager(SDL_Renderer *Renderer);
	~ResourceManager();

protected:
	static void LoadResources(SDL_Renderer *Renderer);
	static void LoadImage(string FileName, SDL_Texture **Texture, SDL_Renderer *Renderer);
};

extern ResourceManager *TheResourceManager;
#endif
