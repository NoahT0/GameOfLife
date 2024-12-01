#include "Tile.h"

int getCharIndex(string input_string, char separator)
{
    for(int i = 0; i < (int)input_string.length(); i++)
    {
        if(input_string[i] == separator)
        {
            return i;
        }
    }
    return -1;   
}

int split(string input_string, char separator, string arr[], const int ARR_SIZE)
{
    if(input_string.length() == 0)
    {
        
        return 0;
    }
    int charIdx = getCharIndex(input_string, separator);
    if(charIdx < 0)
    {
        arr[0] = input_string;
        return 1;
    }
    int arrIndex = 0;
    while(charIdx >= 0 && arrIndex < ARR_SIZE)
    {
        arr[arrIndex] = input_string.substr(0,charIdx);
        input_string = input_string.substr(charIdx + 1);
        arrIndex ++;
        charIdx = getCharIndex(input_string,separator);

        
    }
    if(arrIndex < ARR_SIZE)
    {
        arr[arrIndex] = input_string;
        arrIndex ++;
        return arrIndex;
    }
    
    return -1;
    
}

Tile::Tile()
{
    _color = 'G';
}

char Tile::getColor()
{
    return _color;
}

void Tile::setColor(char color)
{
    _color = color;
}

string Tile::getName()
{
    return _name;
}
string Tile::getDescription()
{
    return _description;
}

int Tile::getStaminaChange()
{
    return _stamina_change;
}
int Tile::getStrengthChange()
{
    return _strength_change;
}
int Tile::getWisdomChange()
{
    return _wisdom_change;
}

string Tile::getAdditionalEffect()
{
    return _additional_effect;
}

void Tile::setName(string name)
{
    _name = name;
}
void Tile::setDescription(string description)
{
    _description = description;
}

void Tile::setStaminaChange(int stamina)
{
    _stamina_change = stamina;
}
void Tile::setStrengthChange(int strength)
{
    _strength_change = strength;
}
void Tile::setWisdomChange(int wisdom)
{
    _wisdom_change = wisdom;
}

void Tile::setAdditionalEffect(string additional_effect)
{
    _additional_effect = additional_effect;
}
/*
void Tile::setAllByName(string name)
{
    ifstream special_file("Files/tile_types.txt");

    if(!special_file.is_open())
    {
        cout << "Failed to open tile_types.txt." << endl;
        return;
    }

    string line;
    while(getline(special_file,line) && line != name)
    {
        // Iterate until get to tile name
    }

    if(line != name)
    {
        cout << "Name not found." << endl;
        return;
    }
    _name = name;
    getline(special_file,line);
    _description = line;

    getline(special_file,line);
    string arr[5];
    split(line,'|',arr,5);

    special_file.close();

    _color = arr[0][0];
    _stamina_change = stoi(arr[1]);
    _strength_change = stoi(arr[2]);
    _wisdom_change = stoi(arr[3]);
    _additional_effect = arr[4];

}
*/


Player Tile::changePlayerStats(Player player)
{
    player.addStamina(_stamina_change);
    player.addStrength(_strength_change);
    player.addWisdom(_wisdom_change);

    return player;
}   