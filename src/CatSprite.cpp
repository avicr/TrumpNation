#include "../inc/CatSprite.h"
#include "../inc/TrumpPlayerSprite.h"
#include "../inc/ScoreSprite.h"
#include "../inc/SpriteList.h"
#include "../inc/Globals.h"

CatSprite::CatSprite()
	:Mexican1Sprite()
{
	PlayAnimation(ResourceManager::CatAnimation);
	SetWidth(32 * GLOBAL_SCALE);
	SetHeight(32 * GLOBAL_SCALE);
	GrowthRate = 1.5;
	MaxVelocity = 460;
	SetAnimationPlayRate(1.25);
}

void CatSprite::Interact(TrumpPlayerSprite *OtherSprite)
{
	int Score = CAT_SCORE;
	Score *= bSwapSprites ? 2 : 1;

	DecoSprites.insert(DecoSprites.begin(), new ScoreSprite(Rect.x, Rect.y - 74, Score));
	OtherSprite->AddToScore(Score);	

	Mix_PlayChannel(CHAN_ITEM_PICKUP, PickUpItemFX, 0);	
	bPendingDelete = true;

	string Phrase = "THEY LET\nYOU DO IT";
	if (rand() % 2)
	{
		Phrase = "YOU CAN DO\nANYTHING";
	}
	ThePlayer->Say(2, Phrase);
}