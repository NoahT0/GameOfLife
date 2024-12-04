#ifndef TILE_H
#define TILE_H

#include "Player.h"

int getCharIndex(string input_string, char separator);
int split(string input_string, char separator, string arr[], const int ARR_SIZE);

class Tile
{
    public:
        Tile();
        char getColor();
        string getName();
        string getDescription();
        vector<int> getStats();
        string getAdditionalEffect();

        void setColor(char color);
        void setName(string name);
        void setDescription(string description);
        void setStats(vector<int> stats);
        void setAdditionalEffect(string additional_effects);

        Player changePlayerStats(Player player);
    private:
        char _color;
        string _description;
        string _name;
        vector<int> _stats;
        string _additional_effect;

};

#endif