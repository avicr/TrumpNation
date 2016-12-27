#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif
#include <string>
#include "Globals.h"

using namespace std;

#define TEXTURE_PATH "resource/textures/"
#define DECLARE_TEXTURE_RESORCE(RESOURCE_NAME, FILE_NAME) \
class TEXTURE_RESOURCE_##RESOURCE_NAME : public TextureResource \
{ \
	public: \
	TEXTURE_RESOURCE_##RESOURCE_NAME() : TextureResource(TEXTURE_PATH##FILE_NAME) {} \
	~TEXTURE_RESOURCE_##RESOURCE_NAME() {}; \
}; \
public : \
TEXTURE_RESOURCE_##RESOURCE_NAME RESOURCE_NAME

class TextureResource
{	
public:
	SDL_Texture* Texture;

protected:
	string FileName;

public:
	

	TextureResource(string InFileName) :
		FileName(InFileName)
	{
		SDL_Surface * Image = SDL_LoadBMP(FileName.c_str());
		SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 0xFF, 0, 0xFF));
		Texture = SDL_CreateTextureFromSurface(GRenderer, Image);
		SDL_FreeSurface(Image);		
	}

	~TextureResource()
	{
		SDL_DestroyTexture(Texture);
	}
};

// In a hurry here, no time to be fancy (should be a singleton but whateves). 
// This class holds hardcoded references to each asset.
class ResourceManager
{
	DECLARE_TEXTURE_RESORCE(Blah, "spaceship.bmp");


public:
	static SDL_Texture *SpaceShipTexture;
	ResourceManager(SDL_Renderer *Renderer);
	~ResourceManager();

protected:
	static void LoadResources(SDL_Renderer *Renderer);
	static void LoadImage(string FileName, SDL_Texture **Texture, SDL_Renderer *Renderer);
};

#endif
