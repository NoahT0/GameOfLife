#ifndef PATH_H
#define PATH_H

#include "Tile.h"

ifstream getInputStream(string file_name);

class Path
{
    public:
        //Path(int index, int size);
        Path(string name, string leadership, bool start_advisor, int size);
        void displayTile(int pos, vector<int> on_tile, int width);
        Tile getTileAtPos(int pos);
        string getName();
        bool getStartAdvisor();
        int getStartStamina();
        int getStartWisdom();
        int getStartStrength();
        int getStartPridePoints();
    
    private:
        static const int _BOARD_SIZE = 52;
        string _name;
        int _size;
        bool _start_with_advisor;
        int _start_stamina;
        int _start_wisdom;
        int _start_strength;
        int _start_pride_points;
        Tile _tiles[_BOARD_SIZE];

        void initializeTiles();
        int getGreenCount();
        vector<int> getTilePercentages(bool is_first_half);
        string colorFromCharacter(char color);
        vector <Tile> getTiles();

};

#endif