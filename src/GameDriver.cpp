#include "Game.h"
#include <fstream>

int main()
{
    srand(time(0));
    cout << " hello" << endl;
    Game game;

    while(!game.isFinished())
    {
        game.takeTurn();
    }
    game.displayWinner();


    return 0;
}