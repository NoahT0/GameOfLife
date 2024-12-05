#ifndef BOARD_H
#define BOARD_H

#include "Player.h"
#include "Path.h"


class Board
{
    private:
        static const int _BOARD_SIZE = 52;
        vector <Path> _paths;
        
        static const int _MAX_PLAYERS = 5;
        vector <Player> _players;
        
        bool isPlayerOnTile(int player_index, int path, int pos);
        vector <int> playersOnTile(int path, int pos);

        int getWidthOfTile(int path, int pos);
        int getMaxWidthOfTile(int pos);
        
        
    public:
        Board();
        Board(int player_count);
        Board(vector <Player> players);
        void displayTrack(int player_index);
        void initializeBoard();
        void displayBoard();
        void setPlayerPosition(int player_index, int pos);
        bool movePlayer(int player_index);
        bool movePlayer(int player_index, int amount);
        int getPlayerPosition(int player_index);
        vector<Player> getPlayers();
        Player getPlayerAtIndex(int index);
        void setPlayers(vector<Player> players);
        void setPlayerAtIndex(int index, Player player);
        Tile getTileAtPlayer(int index);
        int getBoardSize();
        vector <Path> getPaths();

        bool isPlayerFinshed(int index);
        bool areAllPlayersFinished();
};
#endif
