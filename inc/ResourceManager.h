#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif
#include <string>
#include "Globals.h"
#include "AssetResource.h"
#include "Animation.h"

using namespace std;

#define DECLARE_TEXTURE_RESOURCE(RESOURCE_NAME, FILE_NAME) \
class TEXTURE_RESOURCE_##RESOURCE_NAME : public TextureResource \
{ \
	public: \
	TEXTURE_RESOURCE_##RESOURCE_NAME() : TextureResource(FILE_NAME) {ResourceManager::RESOURCE_NAME = new TEXTURE_RESOURCE_##RESOURCE_NAME(*this);} \
	~TEXTURE_RESOURCE_##RESOURCE_NAME() {}; \
}; \
public : \
static TEXTURE_RESOURCE_##RESOURCE_NAME *RESOURCE_NAME;  \
private : \
static TEXTURE_RESOURCE_##RESOURCE_NAME _##RESOURCE_NAME;  

#define INIT_TEXTURE_RESOURCE(RESOURCE_NAME) \
ResourceManager::TEXTURE_RESOURCE_##RESOURCE_NAME ResourceManager::_##RESOURCE_NAME; \
ResourceManager::TEXTURE_RESOURCE_##RESOURCE_NAME *ResourceManager::RESOURCE_NAME = NULL; 

class TextureResource : public AssetResource
{	
public:
	SDL_Texture* Texture;
	SDL_Rect SrcRect;

public:
	
	TextureResource(string InFileName) : AssetResource(InFileName) { }

	TextureResource(const TextureResource &Other) :
		AssetResource(Other.FileName)
	{		
		SDL_Surface * Image = SDL_LoadBMP((TEXTURE_PATH + FileName).c_str());
		SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 0xFF, 0, 0xFF));
		Texture = SDL_CreateTextureFromSurface(GetRenderer(), Image);
		SrcRect = { 0, 0, Image->w, Image->h };
		SDL_FreeSurface(Image);
		AddReferenceToManager();
	}

	~TextureResource()
	{
		SDL_DestroyTexture(Texture);
	}
};

class ResourceManager
{
	friend AssetResource;

public:
	DECLARE_TEXTURE_RESOURCE(TrumpSpriteSheet, "Trump512x256.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpRedHatSpriteSheet, "TrumpHat512x256.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpDamageSpriteSheet, "TrumpDamage512x256.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpIntroSpriteSheet, "TrumpTie512x256.bmp");
	DECLARE_TEXTURE_RESOURCE(Mexican1SpriteSheet, "MexicanGreen.bmp");
	DECLARE_TEXTURE_RESOURCE(BackgroundTexture, "BG3.bmp");
	DECLARE_TEXTURE_RESOURCE(ShadowTexture, "Shadow.bmp");
	DECLARE_TEXTURE_RESOURCE(WallTexture, "WallAssembled.bmp");
	DECLARE_TEXTURE_RESOURCE(BrickTexture, "brick.bmp");
	DECLARE_TEXTURE_RESOURCE(HatTexture, "Hat.bmp");
	DECLARE_TEXTURE_RESOURCE(BombTexture, "Bomb.bmp");
	DECLARE_TEXTURE_RESOURCE(RedHatTexture, "Redhat.bmp");
	DECLARE_TEXTURE_RESOURCE(HUDTexture, "MockupHUD.bmp");
	DECLARE_TEXTURE_RESOURCE(TitleScreenTexture, "TitleScreen.bmp");
	DECLARE_TEXTURE_RESOURCE(FontTexture, "Font512x512.bmp");
	DECLARE_TEXTURE_RESOURCE(InfoTexture, "Info.bmp");
	DECLARE_TEXTURE_RESOURCE(MexicanFaceTexture, "MexicanFace.bmp");

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 128;
			int SpriteHeight = 128;
			SDL_Rect SrcRect = { i % 4 * SpriteWidth, i / 4 * SpriteHeight, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpSpriteSheet->Texture, SrcRect, 0.05));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpRedHatAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 128;
			int SpriteHeight = 128;
			SDL_Rect SrcRect = { i % 4 * SpriteWidth, i / 4 * SpriteHeight, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpRedHatSpriteSheet->Texture, SrcRect, 0.02));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpRedHatAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpIntroAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 128;
			int SpriteHeight = 128;
			SDL_Rect SrcRect = { i % 4 * SpriteWidth, i / 4 * SpriteHeight, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpIntroSpriteSheet->Texture, SrcRect, 0.03));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpIntroAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpDamageAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 128;
			int SpriteHeight = 128;
			SDL_Rect SrcRect = { i % 4 * SpriteWidth, i / 4 * SpriteHeight, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpDamageSpriteSheet->Texture, SrcRect, 0.02));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpDamageAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(Mexican1Animation)
	{
		int SpriteWidth = 128;
		int SpriteHeight = 136;

		for (int i = 0; i < 3; i++)
		{
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };
			Frames.push_back(new Frame(ResourceManager::Mexican1SpriteSheet->Texture, SrcRect, 0.07));
		}

	}
	END_DECLARE_ANIMATION_RESOURCE(Mexican1Animation)

public:
	ResourceManager();
	~ResourceManager();
	static SDL_Texture *SpaceShipTexture;	

private:
	static vector <AssetResource*> AllResources;
};

#endif
