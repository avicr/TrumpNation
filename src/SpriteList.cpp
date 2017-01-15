#include "../inc/SpriteList.h"
#include "../inc/Globals.h"

void SpriteList::Tick(double DeltaTime)
{
	for (int i = size() - 1; i >= 0; i--)
	{
		(*this)[i]->Tick(DeltaTime);
		(*this)[i]->CheckCollision(ThePlayer);

		if ((*this)[i]->GetPendingDelete())
		{
			delete ((*this)[i]);
			(*this).erase(begin() + i);
			i--;
		}
	}
}

void SpriteList::Render(SDL_Renderer *Renderer)
{
	for (int i = size() - 1; i >= 0; i--)
	{
		(*this)[i]->Render(Renderer);
		/*SDL_Rect CollisionRect = Mexicans[i]->GetScreenSpaceCollisionRect();
		SDL_SetRenderDrawColor(GRenderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(GRenderer, &CollisionRect);*/
	}
}

void SpriteList::DeleteAll()
{
	if (size() > 0)
	{
		for (int i = size() - 1; i >= 0; i--)
		{
			delete (*this)[i];
			(*this).erase(begin() + i);			
		}
		resize(0);
	}
}