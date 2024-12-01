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
        int getStaminaChange();
        int getStrengthChange();
        int getWisdomChange();
        string getAdditionalEffect();

        void setColor(char color);
        void setName(string name);
        void setDescription(string description);
        void setStaminaChange(int stamina);
        void setStrengthChange(int strength);
        void setWisdomChange(int wisdom);
        void setAdditionalEffect(string additional_effects);

        //void setAllByName(string name);
        Player changePlayerStats(Player player);
    private:
        char _color;
        string _description;
        string _name;
        int _stamina_change;
        int _strength_change;
        int _wisdom_change;
        string _additional_effect;

};

#endif