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
	TEXTURE_RESOURCE_##RESOURCE_NAME() : TextureResource(FILE_NAME) {} \
	~TEXTURE_RESOURCE_##RESOURCE_NAME() {}; \
}; \
public : \
TEXTURE_RESOURCE_##RESOURCE_NAME RESOURCE_NAME; 

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
		SDL_Surface * Image = SDL_LoadBMP((TEXTURE_PATH + FileName).c_str());
		SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 0xFF, 0, 0xFF));
		Texture = SDL_CreateTextureFromSurface(GRenderer, Image);
		SDL_FreeSurface(Image);		
	}

	~TextureResource()
	{
		SDL_DestroyTexture(Texture);
	}
};

class ResourceManager
{
	DECLARE_TEXTURE_RESORCE(Blah, "spaceship.bmp");

private:
	static ResourceManager *Instance;
	ResourceManager();

public:
	static SDL_Texture *SpaceShipTexture;	

	static ResourceManager *GetInstance();
};

#endif
