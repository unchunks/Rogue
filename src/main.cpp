#include "Game.h"

int main(int argc, char **argv)
{
    Game game;
    bool success = game.Init();
    if(success)
    {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}