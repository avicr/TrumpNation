#ifdef _WIN32
	#include <SDL.h>
#else
	#include "SDL2/SDL.h"
#endif

#include "../inc/ResourceManager.h"
#include "../inc/Sprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/Mexican1Sprite.h"
#include "../inc/ItemSprite.h"

SDL_Window *GWindow;
SDL_Renderer *GRenderer;
ResourceManager *GResourceManager;
bool bSDLInitialized = false;
TrumpPlayerSprite *TestSprite;
Uint64 TickFreq;
vector <Mexican1Sprite*> Mexicans;
vector <ItemSprite *> Items;
bool WallArray[16];

void GameLoop();
void Tick(double DeltaTime);
void Render();
void InitSDL();

int main(int argc, char ** argv)
{
	// variables		

	// init SDL	
	InitSDL();

	SDL_SetRenderDrawColor(GRenderer, 255, 255, 255, 255);
	
	GResourceManager = new ResourceManager;

	GameLoop();
	
	delete GResourceManager;
	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(GWindow);
	SDL_Quit();

	return 0;
}

void GameLoop()
{
	bool bDone = false;
	SDL_Event TheEvent;
	TickFreq = SDL_GetPerformanceFrequency();

	TestSprite = new TrumpPlayerSprite();
	TestSprite->PlayAnimation(ResourceManager::TrumpAnimation);

	Items.push_back(new BrickItem(470, 570));
	SDL_SetTextureAlphaMod(ResourceManager::ShadowTexture->Texture, 128);

	memset(WallArray, 0, sizeof(WallArray));
	WallArray[0] = false;
	WallArray[1] = false;
	WallArray[2] = false;
	WallArray[3] = false;
	WallArray[4] = false;
	WallArray[5] = false;
	WallArray[6] = false;
	WallArray[7] = false;
	WallArray[8] = false;
	WallArray[9] = false;
	WallArray[10] = false;
	WallArray[11] = false;
	WallArray[12] = false;
	WallArray[13] = false;
	WallArray[14] = false;
	WallArray[15] = false;
	
	Uint64 StartTime = SDL_GetPerformanceCounter();
	Uint64 CurrentTime = SDL_GetPerformanceCounter();
	double DeltaTime;
	while (!bDone)
	{
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
		{
			bDone = true;
		}
		StartTime = CurrentTime;
		CurrentTime = SDL_GetPerformanceCounter();
		DeltaTime = (double)((CurrentTime - StartTime) * 1000 / (double)SDL_GetPerformanceFrequency());

		
		Tick(DeltaTime * (double)0.001);
		Render();	
		//Handle events on queue
		while (SDL_PollEvent(&TheEvent) != 0)
		{
			//User requests quit
			if (TheEvent.type == SDL_QUIT)
			{
				bDone = true;
			}
		}
	}
}

void Tick(double DeltaTime)
{	
	static double SpawnCountdown = 0.15;
	SpawnCountdown -= DeltaTime;

	if (SpawnCountdown <= 0)
	{
		Mexicans.push_back(new Mexican1Sprite());
		SpawnCountdown = 0.15;
	}

	TestSprite->Tick(DeltaTime);
	
	for (int i = Mexicans.size() - 1; i >= 0; i--)
	{
		Mexicans[i]->Tick(DeltaTime);

		if (Mexicans[i]->GetPendingDelete())
		{
			delete Mexicans[i];
			Mexicans.erase(Mexicans.begin() + i);
			i--;
		}
	}

	for (int i = Items.size() - 1; i >= 0; i--)
	{
		Items[i]->Tick(DeltaTime);
		Items[i]->CheckCollision(TestSprite);

		if (Items[i]->GetPendingDelete())
		{
			delete Items[i];
			Items.erase(Items.begin() + i);
			i--;
		}
	}
}

void Render()
{
	// Clear to ground color
	SDL_SetRenderDrawColor(GRenderer, 217, 201, 124, 255);
	SDL_RenderClear(GRenderer);

	//SDL_RenderCopy(GRenderer, ResourceManager::BackgroundTexture->Texture, &Rect, &Rect);

	// Render sky
	SDL_Rect Rect = { 0, 0, 1024, HORIZON };
	SDL_SetRenderDrawColor(GRenderer, 0, 162, 232, 255);
	SDL_RenderFillRect(GRenderer, &Rect);

	for (int WallIndex = 0; WallIndex < 16; WallIndex++)
	{
		Rect.x = WallIndex * 64;
		Rect.y = WALL_TOP;
		Rect.w = 64;
		Rect.h = 160;

		if (WallArray[WallIndex])
		{
			SDL_RenderCopy(GRenderer, ResourceManager::WallTexture->Texture, NULL, &Rect);
		}
	}

	for (int i = Mexicans.size() - 1; i >= 0; i--)
	{
		Mexicans[i]->Render(GRenderer);
	}

	for (int i = Items.size() - 1; i >= 0; i--)
	{
		Items[i]->Render(GRenderer);
	}

	TestSprite->Render(GRenderer);

	SDL_RenderPresent(GRenderer);

}

void InitSDL()
{
	if (!bSDLInitialized)
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
		GWindow = SDL_CreateWindow("Trump Nation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, SDL_WINDOW_OPENGL);
		GRenderer = SDL_CreateRenderer(GWindow, -1, 0);
		
		bSDLInitialized = true;
	}
}

SDL_Renderer *GetRenderer()
{
	if (GRenderer == NULL)
	{
		InitSDL();
	}

	return GRenderer;
}
