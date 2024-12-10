#include "Path.h"
#include <fstream>
#include <cassert>

#define RESET "\033[0m"

Path::Path(string name, vector<int> start_stats, bool start_advisor, int size)
{
    _name = name;
    _tiles = vector<Tile>(size);
    
    // Main stat is at end of start_stats vector
    _start_main_stat = start_stats[start_stats.size()-1];
    start_stats.pop_back(); // Remove the main stat from start_stats
    _start_stats = start_stats;

    _start_with_advisor = start_advisor;
    
    initializeTiles(size);
    initializeEvents();
    
}

string Path::getName()
{
    return _name;
}
int Path::getSize()
{
    return _tiles.size();
}
bool Path::getStartAdvisor()
{
    return _start_with_advisor;
}
int Path::getStartMainStat()
{
    return _start_main_stat;
}
vector<int> Path::getStartStats()
{
    return _start_stats;
}
vector<Tile> Path::getTiles()
{
    return _tiles;
}
Tile Path::getTileAtPos(int pos)
{
    assert(pos >= 0 && pos < _tiles.size());
    return _tiles[pos];
}
vector<Event> Path::getEvents()
{
    return _events;
}
string Path::getDescriptionDisplay()
{
    // Add name, size and if path starts with advisor to string
    string result = _name + "\n";
    result += "Size: " + to_string(getSize()) + "\n";
    result += "Start with advisor: ";
    if(_start_with_advisor)
    {
        result += "True \n";
    }
    else
    {
        result += "False \n";
    }
    // For each stat determine if stat gets boosted or deducted when player starts there.
    // Then add corresponding stat name, deduction/boost and value of deduction/boost to string
    vector<string> stat_names = getStatNames();
    for(int i = 0; i < _start_stats.size(); i++)
    {
        if(_start_stats[i] < 0)
        {
            result += stat_names[i] + " deduction: " + to_string(-1 * _start_stats[i]) + "\n";
        }
        else
        {
            result += stat_names[i] + " boost: " + to_string(_start_stats[i]) + "\n";
        }
        
    }
    if(_start_main_stat > 0)
    {
        result += getMainStatName() + " boost: " + to_string(_start_main_stat);
    }
    else
    {
        result += getMainStatName() + " deduction: " + to_string(-1 * _start_main_stat);
    }
    
    return result;
}
void Path::setName(string name)
{
    _name = name;
}
void Path::setStartAdvisor(bool start_advisor)
{
    _start_with_advisor = start_advisor;
}
void Path::setStartStats(vector<int> stats)
{
    _start_stats = stats;
}
void Path::setMainStat(int main_stat)
{
    _start_main_stat = main_stat;
}
void Path::setTiles(vector<Tile> tiles)
{
    _tiles = tiles;
}
void Path::setEvents(vector<Event> events)
{
    _events = events;
}

void Path::displayTile(int pos, vector<int> on_tile, int width)
{
    // Template for displaying a tile: <line filler space> <color start> |<player symbol or blank space>| <reset color> <line filler space> <endl>
    // Get color to display
    string color = _tiles[pos].getColor().color_value;

    // If players are on tile then print first player numbers followed by other players with & in between
    if (on_tile.size()>0)
    {
        cout << color << "|";
        string inner = to_string(on_tile[0] + 1);   // on_tile contains indicies of players so add 1 when displaying
        for(int i = 1; i<on_tile.size(); i++)
        {
            inner += "&" + to_string(on_tile[i]+1);
        }
        cout << setw(width) << left << inner;
        cout << "|" << RESET;
    }
    else
    {
        // Set Width so that corresponding tiles on different paths will have same width
        cout << color << "|" << setw(width) << " ";
        cout << "|" << RESET;
    }

}

void Path::initializeTiles(int size)
{
    vector<Tile> tiles = getAllTiles();
    // Number of rows represent sections. ex: three rows means path will be divided into thirds
    // Columns represent percentage a particular special tile will be picked
    vector<vector<int>> all_percentages = getTilePercentages();
    vector<Tile> special_tiles = getPossibleSpecialTiles(); // Paths won't have all the special tiles so get possible ones from file
    vector<int> percentages = all_percentages[0];   // Start with first section of percentages
    assert(special_tiles.size() == percentages.size()); // There must be same amount of special tiles and special tiles percentages in paths.txt 

    int percentage_index = 1;

    int total_green = getGreenCount();

    Tile temp;
    int green_count = 0;
    int total_tiles = size;

    for (int i = 0; i < total_tiles; i++)
    {
        if (i == total_tiles - 1) {
            // Set the last tile
            _tiles[i] = tiles[1];
        }
        else if (i == 0) {
            // Set the first tile
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
    getline(path_file,line);    // Increment past tile count
    
    getline(path_file,line);
    assert(validateInt(line)); // Make sure there is a number for the path size and green count otherwise there will be an errror
    int green_count = stoi(line);

    path_file.close();

    return green_count;
}
vector<vector<int>> Path::getTilePercentages()
{
    // Number of rows represent sections. ex: three rows means path will be divided into thirds
    // Columns represent percentage a particular special tile will be picked in a particular row

    vector<vector<int>> percentages;

    ifstream path_file = iterateToStringInStream("paths.txt", {_name, "Tiles and Percentages:"});

    string line;
    
    // Increment past tile type description
    getline(path_file,line);

    vector<string> str_perc;
    
    while(getline(path_file,line) && line != "Random Events:")
    {
        // special tile name1|special tile name2|special tile name3|etc.
        str_perc = vectorSplit(line, '|');
        vector<int> int_perc;

        // Convert string vector to int vector
        for(int i = 0; i<str_perc.size(); i++)
        {
            assert(validateInt(str_perc[i]));
            int_perc.push_back(stoi(str_perc[i]));
        }

        // Add row of percentages to 2d vector
        percentages.push_back(int_perc);
    }
    path_file.close();
    
    return percentages;
}
vector<string> Path::getPossibleSpecialTileNames()
{
    vector<string> names;

    ifstream path_file = iterateToStringInStream("paths.txt", {_name, "Tiles and Percentages:"});

    // Names in paths.txt represented by:
    // special tile name1|special tile name2|special tile name3|etc.
    string line;
    getline(path_file,line);

    path_file.close();
    vector<string> tile_names = vectorSplit(line,'|');

    return tile_names;
}
vector<Tile> Path::getPossibleSpecialTiles()
{
    // Get the names of the special tiles and then find corresponding tile with name in all_tiles vector and add it to special_tiles

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
    // Loop until tile with particular name is found
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
    ifstream tile_file = iteratePastDescription("tile_types.txt");

    vector<Color> colors = getPossibleColors();

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
        vector<string> arr = vectorSplit(line,'|');

        Color color = getColorByName(colors, arr[0]);
        assert(toUpperString(color.name) == toUpperString(arr[0])); // Make sure corresponding color is found for color name specified in file
        tile.setColor(color);

        tile.setAdditionalEffect(arr[1]);

        // Some tiles like move back x tiles will have extra data
        // stored that specifies how much x is
        if(arr.size() == 3)
        {
            tile.setExtraData(arr[2]);
        }

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

void Path::initializeEvents()
{
    ifstream path_file = iterateToStringInStream("paths.txt", {_name, "Random Events:"});
    
    // event1|event2|event3|etc.
    string line;
    getline(path_file,line);
    vector<string> event_names =  vectorSplit(line, '|');
    path_file.close();

    // Loop through event names and find corresponding event with particular name and add it to events vector
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

    // Initialize all events
    string line;

    getline(event_file,line);   // Increment past description
    vector<Event> possible_events;
    while(getline(event_file, line))
    {   
        // Name|Description|Advisor|Main stat
        vector<string> arr = vectorSplit(line, '|');
        assert(arr.size() == 4);
        assert(validateInt(arr[3]));
        Event event = {arr[0], arr[1], arr[2], stoi(arr[3])};
        possible_events.push_back(event);
    }
    event_file.close();

    return possible_events;
}
Event Path::getEventByName(string name, vector<Event> events)
{
    // Loop through events until event with particular name is found
    for(int i = 0; i<events.size(); i++)
    {
        if(events[i].name == name)
        {
            return events[i];
        }
    }
    return Event();
}

vector<Color> Path::getPossibleColors()
{
    ifstream color_file("../files/colors.txt");
    if(!color_file.is_open())
    {
        cout << "colors.txt failed to open. Add it in the files folder" << endl;
    }
    assert(color_file.is_open());

    vector<Color> colors;

    // Initialize new color from line of file and add to vector
    string line;
    while(getline(color_file,line))
    {
        // color name|red val|green val|blue val
        string arr[4];
        split(line, '|', arr, 4);

        Color color;
        color.name = arr[0];
        // arr[1] - arr[3] correspond to rgb values
        color.color_value = "\033[48;2;" + arr[1] + ";" + arr[2] + ";" + arr[3] + "m";

        colors.push_back(color);
    }
    color_file.close();

    return colors;
}
Color Path::getColorByName(vector<Color> colors, string name)
{
    // Loop through colors until color with particular name is found
    for(int i = 0; i < colors.size(); i++)
    {
        if(toUpperString(colors[i].name) == toUpperString(name))
        {
            return colors[i];
        }
    }

    return Color();
}