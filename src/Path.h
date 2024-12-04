#ifndef PATH_H
#define PATH_H

#include "Tile.h"

ifstream getInputStream(string file_name);
ifstream iterateToStringInStream(string file_name, vector<string> strings);
ifstream iteratePastDescription(string file_name);
string toUpperString(string str);
vector<int> vectorStringToInt(vector<string> vec);
vector<string> vectorSplit(string input_string, char separator);
vector<string> getStatNames();
string getMainStatName();
int getIndexOfStatByName(string name);

struct Event
{
    string name;
    string description;
    string advisor_name;
    int main_stat;   // CHANGE
};

class Path
{
    public:
        //Path(int index, int size);
        Path(string name, vector<int> start_stats, bool start_advisor, int size);
        void displayTile(int pos, vector<int> on_tile, int width);
        Tile getTileAtPos(int pos);
        string getName();
        bool getStartAdvisor();

        int getStartMainStat();
        vector<int> getStartStats();
        
        vector<Event> getEvents();
    
    private:
        static const int _BOARD_SIZE = 52;
        string _name;
        int _size;
        bool _start_with_advisor;

        vector<int> _start_stats;
        int _start_main_stat;

        Tile _tiles[_BOARD_SIZE];
        vector <Event> _events;

        void initializeTiles();
        int getGreenCount();
        vector<vector<int>> getTilePercentages();
        string colorFromCharacter(char color);
        vector<Tile> getAllTiles();
        vector<string> getPossibleSpecialTileNames();
        vector<Tile> getPossibleSpecialTiles();
        Tile getTileByName(string name, vector<Tile>);

        void initializeEvents();
        vector<Event> getPossibleEvents();
        Event getEventByName(string name, vector<Event>);

};

#endif