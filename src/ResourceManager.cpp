#include "../inc/ResourceManager.h"

SDL_Texture *ResourceManager::SpaceShipTexture;
vector <AssetResource*> ResourceManager::AllResources;

INIT_TEXTURE_RESOURCE(BGTexture)
INIT_TEXTURE_RESOURCE(TrumpSpriteSheet)
INIT_TEXTURE_RESOURCE(TrumpRedHatSpriteSheet)
INIT_TEXTURE_RESOURCE(TrumpDamageSpriteSheet)
INIT_TEXTURE_RESOURCE(TrumpIntroSpriteSheet)
INIT_TEXTURE_RESOURCE(Mexican1SpriteSheet)
INIT_TEXTURE_RESOURCE(Mexican1PinkSpriteSheet)
INIT_TEXTURE_RESOURCE(WallGoldTexture)
INIT_TEXTURE_RESOURCE(WallTexture)
INIT_TEXTURE_RESOURCE(WallOpaqueTexture)
INIT_TEXTURE_RESOURCE(BrickGoldTexture)
INIT_TEXTURE_RESOURCE(BrickTexture)
INIT_TEXTURE_RESOURCE(HUDTexture)
INIT_TEXTURE_RESOURCE(TrumpFaceTexture)
INIT_TEXTURE_RESOURCE(SingleBrickTexture)
INIT_TEXTURE_RESOURCE(SmallBrickTileTexture)
INIT_TEXTURE_RESOURCE(TrumpDamageHatSpriteSheet)
INIT_TEXTURE_RESOURCE(BombTexture)
INIT_TEXTURE_RESOURCE(RedHatTexture)
INIT_TEXTURE_RESOURCE(SombreroTexture)
INIT_TEXTURE_RESOURCE(TitleScreenTexture)
INIT_TEXTURE_RESOURCE(StarBGTexture)
INIT_TEXTURE_RESOURCE(CloudTexture)
//INIT_TEXTURE_RESOURCE(InfoTexture);
INIT_TEXTURE_RESOURCE(MexicanFaceTexture);
INIT_TEXTURE_RESOURCE(Tiles);
INIT_TEXTURE_RESOURCE(BitmapFont);
INIT_TEXTURE_RESOURCE(RedHatSpriteSheet);
INIT_TEXTURE_RESOURCE(BombSpriteSheet);
INIT_TEXTURE_RESOURCE(SombreroSpriteSheet);
INIT_TEXTURE_RESOURCE(SpeechBubble1);
INIT_TEXTURE_RESOURCE(SpeechBubble2);
INIT_TEXTURE_RESOURCE(CatSpriteSheet);

INIT_ANIMATION_RESOURCE(TrumpAnimation)
INIT_ANIMATION_RESOURCE(TrumpRedHatAnimation)
INIT_ANIMATION_RESOURCE(TrumpIntroAnimation)
INIT_ANIMATION_RESOURCE(Mexican1Animation)
INIT_ANIMATION_RESOURCE(Mexican1PinkAnimation)
INIT_ANIMATION_RESOURCE(TrumpDamageAnimation)
INIT_ANIMATION_RESOURCE(RedHatAnimation)
INIT_ANIMATION_RESOURCE(BombAnimation)
INIT_ANIMATION_RESOURCE(SombreroAnimation)
INIT_ANIMATION_RESOURCE(TrumpDamageHatAnimation)
INIT_ANIMATION_RESOURCE(CatAnimation)

ResourceManager::ResourceManager()
{
	
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < AllResources.size(); i++)
	{
		if (AllResources[i])
		{
			delete AllResources[i];
			AllResources[i] = NULL;
		}
	}
}

/*
ResourceManager::~ResourceManager()
{
	SDL_DestroyTexture(SpaceShipTexture);
}

void ResourceManager::LoadResources(SDL_Renderer *Renderer)
{
	LoadImage("resource/spaceship.bmp", &SpaceShipTexture, Renderer);
}

void ResourceManager::LoadImage(string FileName, SDL_Texture **Texture, SDL_Renderer *Renderer)
{
	SDL_Surface * Image = SDL_LoadBMP(FileName.c_str());
	SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 0xFF, 0, 0xFF));
	*Texture = SDL_CreateTextureFromSurface(Renderer, Image);
	SDL_FreeSurface(Image);
}

ResourceManager *ResourceManager::GetInstance()
{
	if (!Instance)
	{
		Instance = new ResourceManager;
	}

	return Instance;
}
*/