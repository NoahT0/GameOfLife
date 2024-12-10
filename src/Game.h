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
        void displayWinner();

    private:
        // Finalize player initialization after they select their path
        vector<Player> initializePlayersOnPath(vector<Player> players);

        // Character select functions
        vector<Player> getPossiblePlayers();
        void printPlayerStats(vector<Player> players);
        int findCharacterByName(vector<Player> players, string name);
        vector<Player> characterSelect();
        
        // Functions for a player's turn
        void displayMenu();
        void doPlayerAction(int choice);
        void movePlayer();
        Player applyTileEffect(int roll);
        void updateTurn();

        // Sort players by who has the most main_stat
        vector<Player> bubbleSortPlayers();

        Board _board;
        int _turn;
        bool _extra_turn;
};

#endif
