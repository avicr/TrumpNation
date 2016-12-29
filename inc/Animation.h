#ifndef ANIMATION_H
#define ANIMATION_H

#ifdef _WIN32
#include <SDL.h>
#else
#include "SDL2/SDL.h"
#endif

#include <vector>
#include "AssetResource.h"

using namespace std;

// Frames are only added during copy constructor!
#define BEGIN_DECLARE_ANIMATION_RESORCE(RESOURCE_NAME) \
class ANIMATION_RESOURCE_##RESOURCE_NAME : public AnimationResource \
{ \
	public: \
	ANIMATION_RESOURCE_##RESOURCE_NAME() {ResourceManager::##RESOURCE_NAME = new ANIMATION_RESOURCE_##RESOURCE_NAME(*this);} \
	ANIMATION_RESOURCE_##RESOURCE_NAME(const ANIMATION_RESOURCE_##RESOURCE_NAME &Other):AnimationResource(Other) { \
	

#define END_DECLARE_ANIMATION_RESOURCE(RESOURCE_NAME) \
} \
}; \
public : \
static ANIMATION_RESOURCE_##RESOURCE_NAME *RESOURCE_NAME; \
private : \
static ANIMATION_RESOURCE_##RESOURCE_NAME _##RESOURCE_NAME; 

#define INIT_ANIMATION_RESOURCE(RESOURCE_NAME) \
ResourceManager::ANIMATION_RESOURCE_##RESOURCE_NAME ResourceManager::_##RESOURCE_NAME; \
ResourceManager::ANIMATION_RESOURCE_##RESOURCE_NAME *ResourceManager::RESOURCE_NAME = NULL; 

class Frame
{
protected:
	SDL_Texture *Texture;
	SDL_Rect SrcRect;
	double FrameTime;	

public:
	Frame();
	Frame(SDL_Texture* InTexture, SDL_Rect InSrcRect, double InFrameTime);

	SDL_Texture *GetTexture();
	SDL_Rect GetSrcRect();
	double GetFrameTime();
	

};

class AnimationResource : public AssetResource
{
friend class ResourceManager;

protected:
	vector <Frame*> Frames;	

public:	
	Frame *GetFrame(int FrameNum);
	int GetFrameCount();
	AnimationResource();
	AnimationResource(const AnimationResource &Other);
	~AnimationResource();
};

struct AnimInfo
{
	AnimationResource *Anim;
	double CountDown;
	double PlayRate;
	int CurrentFrameIndex;	

	AnimInfo() : Anim(NULL), CurrentFrameIndex(0), PlayRate(1) {};

};
#endif