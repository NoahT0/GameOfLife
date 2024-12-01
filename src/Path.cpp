#include "Path.h"
#include <fstream>
#include <cassert>

#define RED "\033[48;2;230;10;10m"
#define GREEN "\033[48;2;34;139;34m" /* Grassy Green (34,139,34) */
#define BLUE "\033[48;2;10;10;230m"
#define PINK "\033[48;2;255;105;180m"
#define BROWN "\033[48;2;139;69;19m"
#define PURPLE "\033[48;2;128;0;128m"
#define ORANGE "\033[48;2;230;115;0m" /* Orange (230,115,0) */
#define GREY "\033[48;2;128;128;128m" /* Grey (128,128,128) */
#define RESET "\033[0m"

void stringToIntArray(string arr1[], int arr2[], int size)
{
    for(int i = 0; i <size; i++)
    {
        arr2[i] = stoi(arr1[i]);
    }
}
//int split(string input_string, char separator, string arr[], const int ARR_SIZE);
ifstream getInputStream(string file_name)
{
    ifstream input_file("../files/" + file_name);
    
    if(!input_file.is_open())
    {
        cout << file_name << " failed to open." << endl;
    }
    assert(input_file.is_open());

    return input_file;
}
vector<string> vectorSplit(string input_string, char separator)
{
    vector <string> result;

    if(input_string.length() == 0)
    {
        cout << "input string has 0 length" << endl;
        return result;
    }


    int charIdx = getCharIndex(input_string, separator);
    if(charIdx < 0)
    {
        result.push_back(input_string);
        return result;
    }
    while(charIdx >= 0)
    {
        result.push_back(input_string.substr(0,charIdx));
        input_string = input_string.substr(charIdx + 1);
        charIdx = getCharIndex(input_string,separator);

    }
    result.push_back(input_string);
    
    return result;
}


Path::Path(string name, string leadership, bool start_advisor, int size)
{

    _name = name;
    _size = size;

    string arr[4];
    int leadership_arr[4];
    split(leadership,'|',arr,4);
    stringToIntArray(arr, leadership_arr,4);

    _start_stamina = leadership_arr[0];
    _start_strength = leadership_arr[1];
    _start_wisdom = leadership_arr[2];
    _start_pride_points = leadership_arr[3];

    _start_with_advisor = start_advisor;

    initializeTiles();

}

void Path::initializeTiles()
{
    vector<Tile> tiles = getTiles();
    vector<int> percentages = getTilePercentages(true);
    // number of special tiles must be the same in both paths.txt and tile_types.txt. The +4 = 3 basic tiles + one less special tile in paths
    assert(tiles.size() == percentages.size() + 4); 

    int total_green = getGreenCount();

    Tile temp;
    int green_count = 0;
    int total_tiles = _size;

    for (int i = 0; i < total_tiles; i++)
    {
        if (i == total_tiles - 1) {
            // Set the last tile as Orange for "Pride Rock"
            _tiles[i] = tiles[1];
        }
        else if (i == 0) {
            // Set the last tile as Orange for "Pride Rock"
            _tiles[i] = tiles[0];
        }
        else if (green_count < total_green && (rand() % (total_tiles - 1 - i) < total_green - green_count)) {
            _tiles[i] = tiles[2];
            green_count++;
        }
        else
        {
            
            int color_choice = rand() % 100;
            int cur_percentage = percentages[0];

            int count = 0;
            // Loop while color_choice is >= cur_percentage
            // Example: color_choice is 30 so loops first because greater than cur_percentage which is 
            // equal to graveyard percentage (20). Then hyena percentage is added (20) so cur_percentage is 40
            // so doesn't loop again and count is 1 and hyena is the second special(index 1) tile in tile_types.txt
            while(color_choice>=cur_percentage && count < percentages.size()-1)
            {
                count ++;
                cur_percentage += percentages[count];
            }

            // Adds 1 to count since the last special tile is just remaining percentage out of 100 and not included in the vector
            if(color_choice >= cur_percentage)
            {
                count++;
            }
            // count + 3 since special tiles come after start, grassland and end tiles
            _tiles[i] = tiles[count+3];

        }
        
        // Switch to second half percentages
        if(i == total_tiles/2)
        {
            percentages = getTilePercentages(false);

        }
    }
    
}
int Path::getGreenCount()
{
    //ifstream path_file("Files/paths.txt");
    ifstream path_file = getInputStream("paths.txt");
    
    string line;

    // Iterate to correct path
    while(line != _name)
    {
        getline(path_file,line);
    }
    // Iterate to tile initialization section of path data
    while(line != "Tile Initialization:")
    {
        getline(path_file,line);
    }

    getline(path_file,line);
    int green_count = stoi(line);

    path_file.close();
    return green_count;
}
vector<int> Path::getTilePercentages(bool is_first_half)
{

    vector<int> percentages;

    ifstream path_file = getInputStream("paths.txt");

    string line;

    // Iterate to correct path
    while(line != _name)
    {
        getline(path_file,line);
    }
    // Iterate to tile initialization section of path data
    while(line != "Tile Initialization:")
    {
        getline(path_file,line);
    }

    // Move past green count
    getline(path_file,line);

    getline(path_file, line);
    vector<string> str_perc;

    if(!is_first_half)
    {
        getline(path_file,line);
    }
    str_perc = vectorSplit(line, '|');

    path_file.close();

    // Convert string vector to int vector
    for(int i = 0; i<str_perc.size(); i++)
    {
        percentages.push_back(stoi(str_perc[i]));
    }

    return percentages;
}

string Path::colorFromCharacter(char color)
{
    switch(color)
    {
        case 'R':
            return RED;
        case 'G':
            return GREEN;
        case 'B':
            return BLUE;
        case 'U':
            return PURPLE;
        case 'N':
            return BROWN;
        case 'P':
            return PINK;
        case 'O':
            return ORANGE;
        case 'Y':
            return GREY;
    }
    return GREEN;
}

void Path::displayTile(int pos, vector<int> on_tile, int width)
{
    // Template for displaying a tile: <line filler space> <color start> |<player symbol or blank space>| <reset color> <line filler space> <endl>
    // Determine color to display
    string color = colorFromCharacter(_tiles[pos].getColor());

    if (on_tile.size()>0)
    {
        cout << color << "|";
        string inner = to_string(on_tile[0] + 1);
        for(int i = 1; i<on_tile.size(); i++)
        {
            //cout << ", " << (onTile[i]+1);
            inner += "&" + to_string(on_tile[i]+1);
        }
        cout << setw(width) << left << inner;
        cout << "|" << RESET;
    }
    else
    {
        cout << color << "|" << setw(width) << " ";
        cout << "|" << RESET;
    }

}

vector<Tile> Path::getTiles()
{
    vector<Tile> tiles;

    //ifstream tile_file("Files/tile_types.txt");
    ifstream tile_file = getInputStream("tile_types.txt");  

    string line = " ";

    // Iterate past description
    while(line.length() > 0)
    {
        getline(tile_file,line);
    }

    while(getline(tile_file,line))
    {
        Tile tile;
        tile.setName(line);
        getline(tile_file, line);
        tile.setDescription(line);

        getline(tile_file,line);
        string arr[5];
        split(line,'|',arr,5);


        tile.setColor(arr[0][0]);
        tile.setStaminaChange(stoi(arr[1]));
        tile.setStrengthChange(stoi(arr[2]));
        tile.setWisdomChange(stoi(arr[3]));
        tile.setAdditionalEffect(arr[4]);

        // Get blank line
        getline(tile_file,line);

        tiles.push_back(tile);
    }

    tile_file.close();

    return tiles;
}

Tile Path::getTileAtPos(int pos)
{
    return _tiles[pos];
}

string Path::getName()
{
    return _name;
}

bool Path::getStartAdvisor()
{
    return _start_with_advisor;
}
int Path::getStartStamina()
{
    return _start_stamina;
}
int Path::getStartWisdom()
{
    return _start_wisdom;
}
int Path::getStartStrength()
{
    return _start_strength;
}
int Path::getStartPridePoints()
{
    return _start_pride_points;
}