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
		SDL_Log("About to load: %s", Other.FileName.c_str());
		SDL_Surface * Image = SDL_LoadBMP((TEXTURE_PATH + FileName).c_str());
		SDL_Log("Loaded: %d", Image);
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
	DECLARE_TEXTURE_RESOURCE(BGTexture, "BG.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpSpriteSheet, "Trump.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpRedHatSpriteSheet, "Trump_hat.bmp");
	DECLARE_TEXTURE_RESOURCE(SombreroTexture, "sombrero.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpDamageSpriteSheet, "Trump_Angry.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpIntroSpriteSheet, "large trump.bmp");
	DECLARE_TEXTURE_RESOURCE(Mexican1SpriteSheet, "mexican.bmp");
	DECLARE_TEXTURE_RESOURCE(Mexican1PinkSpriteSheet, "MexicanPink.bmp");		
	DECLARE_TEXTURE_RESOURCE(WallGoldTexture, "WallAssembledGold.bmp");
	DECLARE_TEXTURE_RESOURCE(WallTexture, "WallAssembled.bmp");
	DECLARE_TEXTURE_RESOURCE(TrumpFaceTexture, "trumpface.bmp");
	DECLARE_TEXTURE_RESOURCE(WallOpaqueTexture, "WallAssembledOpaque.bmp");
	DECLARE_TEXTURE_RESOURCE(BrickGoldTexture, "brickgold.bmp");
	DECLARE_TEXTURE_RESOURCE(BrickTexture, "brick.bmp");
	DECLARE_TEXTURE_RESOURCE(SombreroSpriteSheet, "sombrerosheet.bmp");
	DECLARE_TEXTURE_RESOURCE(BombTexture, "Bomb.bmp");
	DECLARE_TEXTURE_RESOURCE(RedHatTexture, "Redhat.bmp");
	DECLARE_TEXTURE_RESOURCE(RedHatSpriteSheet, "hatsheet.bmp");
	DECLARE_TEXTURE_RESOURCE(BombSpriteSheet, "bombsheet.bmp");
	DECLARE_TEXTURE_RESOURCE(HUDTexture, "GUI.bmp");
	DECLARE_TEXTURE_RESOURCE(TitleScreenTexture, "TitleScreen.bmp");
	DECLARE_TEXTURE_RESOURCE(StarBGTexture, "starBG.bmp");
	DECLARE_TEXTURE_RESOURCE(CloudTexture, "cloud.bmp");
	DECLARE_TEXTURE_RESOURCE(SingleBrickTexture, "singlebrick.bmp")
	DECLARE_TEXTURE_RESOURCE(SmallBrickTileTexture, "smallbricktile.bmp")
	DECLARE_TEXTURE_RESOURCE(TrumpDamageHatSpriteSheet, "Trump_hat-Angry.bmp")
	//DECLARE_TEXTURE_RESOURCE(InfoTexture, "Info.bmp");	
	DECLARE_TEXTURE_RESOURCE(MexicanFaceTexture, "MexicanFace.bmp");
	DECLARE_TEXTURE_RESOURCE(Tiles, "tiles2x.bmp");
	DECLARE_TEXTURE_RESOURCE(BitmapFont, "numbers.bmp");
	DECLARE_TEXTURE_RESOURCE(SpeechBubble1, "speech1.bmp");
	DECLARE_TEXTURE_RESOURCE(SpeechBubble2, "speech2.bmp");


	BEGIN_DECLARE_ANIMATION_RESORCE(RedHatAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 32;
			int SpriteHeight = 32;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::RedHatSpriteSheet->Texture, SrcRect, 0.1));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(RedHatAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(SombreroAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 32;
			int SpriteHeight = 32;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::SombreroSpriteSheet->Texture, SrcRect, 0.1));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(SombreroAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(BombAnimation)
	{
		for (int i = 0; i < 4; i++)
		{
			int SpriteWidth = 32;
			int SpriteHeight = 32;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::BombSpriteSheet->Texture, SrcRect, 0.1));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(BombAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 32;
			int SpriteHeight = 32;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpSpriteSheet->Texture, SrcRect, 0.05));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpRedHatAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 32;
			int SpriteHeight = 32;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpRedHatSpriteSheet->Texture, SrcRect, 0.03));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpRedHatAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpIntroAnimation)
	{
		for (int i = 0; i < 6; i++)
		{
			int SpriteWidth = 64;
			int SpriteHeight = 64;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpIntroSpriteSheet->Texture, SrcRect, 0.03));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpIntroAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpDamageAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 32;
			int SpriteHeight = 32;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpDamageSpriteSheet->Texture, SrcRect, 0.02));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpDamageAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(TrumpDamageHatAnimation)
	{
		for (int i = 0; i < 8; i++)
		{
			int SpriteWidth = 32;
			int SpriteHeight = 32;
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };

			Frames.push_back(new Frame(ResourceManager::TrumpDamageHatSpriteSheet->Texture, SrcRect, 0.02));
		}
	}
	END_DECLARE_ANIMATION_RESOURCE(TrumpDamageHatAnimation)

	BEGIN_DECLARE_ANIMATION_RESORCE(Mexican1Animation)
	{
		int SpriteWidth = 32;
		int SpriteHeight = 32;

		for (int i = 0; i < 8; i++)
		{
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };
			Frames.push_back(new Frame(ResourceManager::Mexican1SpriteSheet->Texture, SrcRect, 0.03));
		}

	}
	END_DECLARE_ANIMATION_RESOURCE(Mexican1Animation)

	BEGIN_DECLARE_ANIMATION_RESORCE(Mexican1PinkAnimation)
	{
		int SpriteWidth = 128;
		int SpriteHeight = 136;

		for (int i = 0; i < 3; i++)
		{
			SDL_Rect SrcRect = { i * SpriteWidth, 0, SpriteWidth, SpriteHeight };
			Frames.push_back(new Frame(ResourceManager::Mexican1PinkSpriteSheet->Texture, SrcRect, 0.08));
		}

	}
	END_DECLARE_ANIMATION_RESOURCE(Mexican1PinkAnimation)

public:
	ResourceManager();
	~ResourceManager();
	static SDL_Texture *SpaceShipTexture;	

private:
	static vector <AssetResource*> AllResources;
};

#endif
