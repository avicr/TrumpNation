#include "../inc/ScoreSprite.h"

ScoreSprite::ScoreSprite(int X, int Y, int InScore)
{
	SetPosition(X, Y);
	Score = InScore;
	CountDown = SCORE_LIFE_TIME;
}

void ScoreSprite::Render(SDL_Renderer *Renderer)
{
	DrawText(std::to_string(Score), PosX, PosY, 0, 0, Renderer, FontSeg20White);
}

void ScoreSprite::Tick(double DeltaTime)
{
	CountDown -= DeltaTime;

	if (CountDown <= 0)
	{
		bPendingDelete = true;
	}

	if (CountDown >= SCORE_LIFE_TIME / 2.0)
	{
		PosY += -35 * DeltaTime;
	}
}