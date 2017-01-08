#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();

	void OnMexicanEscaped();
	int GetLevelNumber();
	void SetLevel(int Level);
	bool LevelComplete();
	int GetNumMexicansEscaped();

	int WallArray[16];

protected:
	int NumMexicansEscaped;
	int CurrentLevel;
};

#endif