#include "Game.h"

#pragma comment (lib,"SDL2")
#pragma comment (lib,"SDL2main")

int main(int argc, char** argv)
{
    Game game;
    bool success = game.Init();
    if (success)
    {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}
