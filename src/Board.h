#ifndef BOARD_H
#define BOARD_H

#include "Player.h"
#include "Path.h"


class Board
{
    public:
        Board();
        Board(vector <Player> players);

        vector<Player> getPlayers();
        Player getPlayerAtIndex(int index);
        vector<Path> getPaths();
        Path getPathAtPlayer(int player_index);
        vector<string> getPathDescriptions();
        vector<int> getPathSizes();
        Tile getTileAtPlayer(int player_index);

        void setPlayers(vector<Player> players);
        void setPlayerAtIndex(int player_index, Player player);
        void setPlayerPosition(int player_index, int pos);
        void setPaths(vector<Path> paths);
        
        void initializeBoard();

        void displayTrack(int player_index);
        void displayBoard();

        bool movePlayer(int player_index);
        bool movePlayer(int player_index, int amount);

        bool isPlayerFinshed(int player_index);
        bool areAllPlayersFinished();

    private:
        // Make smaller/bigger depending on how wide console is
        static const int _MAX_TILES_PER_LINE = 53; // Number of tiles before the path goes to next line. If path naturally goes to next line it looks funny
        
        vector <Path> _paths;
        vector <Player> _players;
        
        bool isPlayerOnTile(int player_index, int path, int pos);
        vector <int> playersOnTile(int path, int pos);

        int getWidthOfTile(int path, int pos);
        int getMaxWidthOfTile(int pos);
        
        
};
#endif
