#include "../inc/CloudSprite.h"
#include <random>
#include "../inc/Globals.h"

CloudSprite::CloudSprite()
{	
	SetTexture(ResourceManager::CloudTexture->Texture);
	PosX = (rand() % (1024 + 1 - -64)) + -64;
	PosY = (rand() % (CLOUD_Y_MAX + 1 - CLOUD_Y_MIN)) + CLOUD_Y_MIN;
	Scale = (rand() % (int)(CLOUD_SCALE_MAX * 10000 - CLOUD_SCALE_MIN * 10000)) / 10000.0;
	//Speed = (rand() % (CLOUD_SPEED_MAX + 1 - CLOUD_SPEED_MIN)) + CLOUD_SPEED_MIN;	
	Speed = 3 - (CLOUD_Y_MAX - PosY) / 10;
}

void CloudSprite::Tick(double DeltaTime)
{	
	PosX += DeltaTime * Speed;

	if (PosX > 1024)
	{
		bPendingDelete = true;
	}

	Rect.x = round(PosX);
	Rect.y = round(PosY);
}
