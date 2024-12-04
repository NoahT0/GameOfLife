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
    ifstream theme_file("../files/theme_choice.txt");
    if(!theme_file.is_open())
    {
        cout << "theme_choice.txt" << " failed to open." << endl;
        cout << "add it inside the files folder " << endl;
    }
    assert(theme_file.is_open());
    string theme;
    getline(theme_file,theme);
    theme_file.close();


    ifstream input_file("../files/" + theme + "/" + file_name);
    
    if(!input_file.is_open())
    {
        cout << file_name << " failed to open." << endl;
    }
    assert(input_file.is_open());

    return input_file;
}

ifstream iterateToStringInStream(string file_name, vector<string> strings)
{
    int size = strings.size();
    ifstream input_file = getInputStream(file_name);

    if(size == 0)
    {
        return input_file;
    }

    string line;
    while(getline(input_file, line) && line != strings[0])
    {
        // Iterate until first string is found
    }

    if(line != strings[0])
    {
        cout << strings[0] << " not found in file " << file_name << endl;
    }
    assert(line == strings[0]);

    int index = 1;
    bool is_found = true;
    while(index < size && is_found)
    {
        // Iterate to next string in strings vector
        is_found = false;
        while(!is_found && getline(input_file, line) && line.length() > 0)
        {
            if(line == strings[index])
            {
                is_found = true;
                index ++;
            }
        }

    }

    if(!is_found)
    {
        cout << strings[index] << " not found after " << strings[index-1] << " and before a blank line in file " << file_name << endl;
    }
    assert(is_found);
    return input_file;
}
// Assumes blank line between description and other lines
ifstream iteratePastDescription(string file_name)
{
    return iterateToStringInStream(file_name, {""});
}

vector<string> getStatNames()
{
    ifstream stat_file = getInputStream("stat_settings.txt");
    
    string line;
    getline(stat_file, line);
    stat_file.close();

    vector<string> stat_names = vectorSplit(line, '|');
    stat_names.pop_back();  // Get rid of main stat

    return stat_names;
}

string getMainStatName()
{
    ifstream stat_file = getInputStream("stat_settings.txt");
    
    string line;
    getline(stat_file, line);
    stat_file.close();

    vector<string> stats = vectorSplit(line, '|');

    return stats[stats.size()-1];
}

int getIndexOfStatByName(string name)
{
    vector<string> stat_names = getStatNames();

    for(int i = 0; i<stat_names.size(); i++)
    {
        if(toUpperString(stat_names[i]) == toUpperString(name))
        {
            return i;
        }
    }

    return -1;
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
vector<int> vectorStringToInt(vector<string> vec)
{
    vector<int> result(vec.size());
    for(int i = 0; i<result.size(); i++)
    {
        result[i] = stoi(vec[i]);
    }
    
    return result;
}

string toUpperString(string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        str[i] = toupper(str[i]);
    }
    return str;
}

Path::Path(string name, vector<int> start_stats, bool start_advisor, int size)
{

    _name = name;
    _size = size;

    // string arr[4];
    // int leadership_arr[4];
    // split(leadership,'|',arr,4);
    // stringToIntArray(arr, leadership_arr,4);

    
    _start_main_stat = start_stats[start_stats.size()-1];
    start_stats.pop_back(); // Remove the main stat from start_stats
    _start_stats = start_stats;

    _start_with_advisor = start_advisor;
    
    initializeTiles();
    initializeEvents();
    
}
void Path::initializeEvents()
{
    ifstream path_file = iterateToStringInStream("paths.txt", {_name, "Random Events:"});
    
    string line;
    getline(path_file,line);
    vector<string> event_names =  vectorSplit(line, '|');

    path_file.close();
    vector<Event> events = getPossibleEvents();
    for(int i = 0; i < event_names.size(); i++)
    {
        Event event = getEventByName(event_names[i], events);
        assert(event.name == event_names[i]);
        _events.push_back(event);
    }

}
vector<Event> Path::getPossibleEvents()
{
    ifstream event_file = getInputStream("random_events.txt");
    string line;
    vector<Event> possible_events;
    while(getline(event_file, line))
    {   
        // Name|Description|Advisor|Main stat
        vector<string> arr = vectorSplit(line, '|');
        assert(arr.size() == 4);
        Event event = {arr[0], arr[1], arr[2], stoi(arr[3])};
        possible_events.push_back(event);
    }

    return possible_events;
}
Event Path::getEventByName(string name, vector<Event> events)
{
    for(int i = 0; i<events.size(); i++)
    {
        if(events[i].name == name)
        {
            return events[i];
        }
    }
    return Event();
}

void Path::initializeTiles()
{
    vector<Tile> tiles = getAllTiles();
    vector<vector<int>> all_percentages = getTilePercentages();
    vector<Tile> special_tiles = getPossibleSpecialTiles();
    vector<int> percentages = all_percentages[0];
    assert(special_tiles.size() == percentages.size()); // There must be same amount of special tiles and special tiles percentages in paths.txt 

    int percentage_index = 1;

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
            // Total percent doesn't have to add to 100. Percent values are more like weights
            int total_percent = 0;
            for(int i = 0; i<percentages.size(); i++)
            {
                total_percent += percentages[i];
            }
            int color_choice = rand() % total_percent;
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

            _tiles[i] = special_tiles[count];

        }
        
        // Switch to next section percentages
        /*
        Example: all_percentages has size 3 so percentages = all_percentages[0] from i = 0 to 17 (1 * 52 / 3)
        then percentages = all_percentages[1] from i = 18 to 34 (2 * 52 / 3) 
        and finally percentages = all_percentages[2] from i = 35 to the end
        */
        if(i == percentage_index * total_tiles/all_percentages.size())
        {
            percentages = all_percentages[percentage_index];
            assert(special_tiles.size() == percentages.size()); // There must be same amount of special tiles and special tiles percentages in paths.txt
            percentage_index ++;
        }
    }
    
}
int Path::getGreenCount()
{
    ifstream path_file = iterateToStringInStream("paths.txt", {_name, "Path Initialization:"});

    string line;
    getline(path_file,line);
    int green_count = stoi(line);

    path_file.close();

    return green_count;
}
vector<vector<int>> Path::getTilePercentages()
{

    vector<vector<int>> percentages;

    ifstream path_file = iterateToStringInStream("paths.txt", {_name, "Tiles and Percentages:"});

    string line;
    
    // Increment past tile type description
    getline(path_file,line);

    vector<string> str_perc;
    
    while(getline(path_file,line) && line != "Random Events:")
    {
        str_perc = vectorSplit(line, '|');
        vector<int> int_perc;

        // Convert string vector to int vector
        for(int i = 0; i<str_perc.size(); i++)
        {
            int_perc.push_back(stoi(str_perc[i]));
            //percentages.push_back(stoi(str_perc[i]));
        }
        percentages.push_back(int_perc);
    }
    path_file.close();
    
    return percentages;
}
vector<string> Path::getPossibleSpecialTileNames()
{
    vector<string> names;

    ifstream path_file = iterateToStringInStream("paths.txt", {_name, "Tiles and Percentages:"});

    string line;
    getline(path_file,line);

    path_file.close();
    vector<string> tile_names = vectorSplit(line,'|');

    return tile_names;
}
vector<Tile> Path::getPossibleSpecialTiles()
{
    vector<Tile> all_tiles = getAllTiles();
    vector<string> special_names = getPossibleSpecialTileNames();
    
    vector<Tile> special_tiles;
    for(int i = 0; i < special_names.size(); i++)
    {
        Tile tile = getTileByName(special_names[i], all_tiles);
        assert(toUpperString(tile.getName()) == toUpperString(special_names[i])); // Tile must be successfully found by name

        special_tiles.push_back(tile);
    }

    return special_tiles;
}
Tile Path::getTileByName(string name, vector<Tile> tiles)
{
    name = toUpperString(name);
    for(int i = 0; i < tiles.size(); i++)
    {
        if(name == toUpperString(tiles[i].getName()))
        {
            return tiles[i];
        }
    }

    return Tile();
}
vector<Tile> Path::getAllTiles()
{
    vector<Tile> tiles;

    // Iterate past description
    string arr[1] = {""};
    ifstream tile_file = iteratePastDescription("tile_types.txt");

    int num_of_stats = getStatNames().size();
    string line;
    while(getline(tile_file,line))
    {
        Tile tile;

        // Set name and description of tile
        tile.setName(line);
        getline(tile_file, line);
        tile.setDescription(line);
        
        // Get/Set color and additional effects
        getline(tile_file,line);
        string arr[2];
        split(line,'|',arr,2);

        tile.setColor(arr[0][0]);
        tile.setAdditionalEffect(arr[1]);

        // tile.setStaminaChange(stoi(arr[1]));
        // tile.setStrengthChange(stoi(arr[2]));
        // tile.setWisdomChange(stoi(arr[3]));
        // tile.setAdditionalEffect(arr[4]);

        //Get/set Stats
        getline(tile_file,line);
        vector<string> stats = vectorSplit(line, '|');
        assert(stats.size() == num_of_stats);   // Make sure correct # of stats is defined in tile_types for each tile
        tile.setStats(vectorStringToInt(stats));


        // Get blank line
        getline(tile_file,line);

        tiles.push_back(tile);
    }

    tile_file.close();

    return tiles;
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
// int Path::getStartStamina()
// {
//     return _start_stamina;
// }
// int Path::getStartWisdom()
// {
//     return _start_wisdom;
// }
// int Path::getStartStrength()
// {
//     return _start_strength;
// }
// int Path::getStartPridePoints()
// {
//     return _start_pride_points;
// }

int Path::getStartMainStat()
{
    return _start_main_stat;
}
vector<int> Path::getStartStats()
{
    return _start_stats;
}
vector<Event> Path::getEvents()
{
    return _events;
}