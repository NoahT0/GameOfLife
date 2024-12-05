#ifndef GAME_H
#define GAME_H

#include "Board.h"

class Game
{
    public:
        Game(string theme);
        int getNumPlayers();
        void takeTurn();
        bool isFinished();
        Player calculateWinner();
        void displayWinner();

    private:
        vector<Player> getPossiblePlayers();
        void printPlayerStats(vector<Player> players);
        int findCharacterByName(vector<Player> players, string name);
        vector<Player> characterSelect();
        vector<Player> pathSelect(vector<Player> players, vector<Path> paths);
        
        void displayMenu();
        void doPlayerAction(int choice);
        void movePlayer();
        Player applyTileEffect(int roll);

        vector<Player> initializePlayersOnPath(vector<Player> players);

        Board _board;
        int _turn;
        bool _extra_turn[1];
};

#endif
