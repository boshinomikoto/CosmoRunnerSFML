#include <cstdlib>
#include <ctime>
#include "Game.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    srand(time(NULL));
    Game game;
    game.run();
    return 0;
}