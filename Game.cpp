#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :MyWindow(nullptr), MyRenderer(nullptr), MyTicksCount(0), IsRunning(true), MyPaddleDir(0)
{

}

Game::~Game()
{
}

bool Game::Init()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unalble to init SDL:%s", SDL_GetError());
		return false;
	}
	MyWindow = SDL_CreateWindow("PingPong", 100, 100, 1024, 768, 0);
	if (!MyWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	MyRenderer = SDL_CreateRenderer(
		MyWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!MyRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	MyPaddlePos.X = 10.0f;
	MyPaddlePos.Y = 768.0f / 2.0f;
	MyBallPos.X = 1024.0f / 2.0f;
	MyBallPos.Y = 768.0f / 2.0f;
	MyBallVel.X = -200.0f;
	MyBallVel.Y = 235.0f;
	return true;
}

void Game::RunLoop()
{
	while (IsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(MyRenderer);
	SDL_DestroyWindow(MyWindow);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			IsRunning = false;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		IsRunning = false;
	}
	MyPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		MyPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		MyPaddleDir += 1;
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), MyTicksCount + 16));
	float deltaTime = (SDL_GetTicks() - MyTicksCount) / 1000.0f;

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	MyTicksCount = SDL_GetTicks();
	if (MyPaddleDir != 0)
	{
		MyPaddlePos.Y += MyPaddleDir * 300.0f * deltaTime;
		if (MyPaddlePos.Y < (paddleH / 2.0f + thickness))
		{
			MyPaddlePos.Y = paddleH / 2.0f + thickness;
		}
		else if (MyPaddlePos.Y > (768.0f - paddleH / 2.0f - thickness));

	}
	MyBallPos.X += MyBallVel.X * deltaTime;
	MyBallPos.Y += MyBallVel.Y * deltaTime;

	float diff = MyPaddlePos.Y - MyBallPos.Y;
	diff = (diff > 0.0f) ? diff : -diff;
	if (diff <= paddleH / 2.0f && MyBallPos.X <= 25.0f && MyBallPos.X >= 20.0f && MyBallVel.X < 0.0f)
	{
		MyBallVel.X *= -1.0f;
	}
	else if (MyBallPos.X <= 0.0f)
	{
		IsRunning = false;
	}
	else if(MyBallPos.X>=(1024.0f-thickness)&&MyBallVel.X>0.0f)
	{
		MyBallVel.X *= -1.0f;
	}
	if (MyBallPos.Y <= thickness && MyBallVel.Y < 0.0f)
	{
		MyBallVel.Y *= -1;
	}
	else if (MyBallPos.Y >= (768 - thickness) && MyBallVel.Y > 0.0f)
	{
		MyBallVel.Y *= -1;
	}

}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(MyRenderer, 0, 0, 255, 255);
	SDL_RenderClear(MyRenderer);
	SDL_SetRenderDrawColor(MyRenderer, 255, 255, 255, 255);
	SDL_Rect wall{
		0,0,1024,thickness };
	SDL_RenderFillRect(MyRenderer, &wall);
	wall.y = 768 - thickness;
	SDL_RenderFillRect(MyRenderer, &wall);
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness; 
	wall.h = 1024;
	SDL_RenderFillRect(MyRenderer, &wall);
	SDL_Rect paddle{
		static_cast<int>(MyPaddlePos.X),
		static_cast<int>(MyPaddlePos.Y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(MyRenderer, &paddle);
	SDL_Rect ball{
		static_cast<int>(MyBallPos.X - thickness / 2),
		static_cast<int>(MyBallPos.Y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(MyRenderer, &ball);
	SDL_RenderPresent(MyRenderer);

}
