#include "game.h"

// Program executes here
int main(int argc, char** argv)
{
    Game game("Boulder Dash", 640, 480);
    game.MainLoop();

    return 0;
}
