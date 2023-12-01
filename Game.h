#pragma once
#include "SDL.h"
#include <vector>

struct Vector2
{
	float X;
	float Y;
};

class Game
{
public:
	Game();
	~Game();
	bool Init();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* MyWindow;
	SDL_Renderer* MyRenderer;
	Uint32 MyTicksCount;
	bool IsRunning;

	int MyPaddleDir;
	Vector2 MyPaddlePos;
	Vector2 MyBallPos;
	Vector2 MyBallVel;

};

