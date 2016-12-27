#include "../inc/ResourceManager.h"

SDL_Texture *ResourceManager::SpaceShipTexture;

ResourceManager::~ResourceManager()
{
	SDL_DestroyTexture(SpaceShipTexture);
}

void ResourceManager::LoadResources(SDL_Renderer *Renderer)
{
	LoadImage("resource/spaceship.bmp", SpaceShipTexture, Renderer);
}

void ResourceManager::LoadImage(string FileName, SDL_Texture *Texture, SDL_Renderer *Renderer)
{
	SDL_Surface * Image = SDL_LoadBMP(FileName.c_str());
	SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 0xFF, 0, 0xFF));
	Texture = SDL_CreateTextureFromSurface(Renderer, Image);
	SDL_FreeSurface(Image);
}