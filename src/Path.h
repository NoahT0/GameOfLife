#ifndef PATH_H
#define PATH_H

#include "Tile.h"

ifstream getInputStream(string file_name);
ifstream iterateToStringInStream(string file_name, string strings[], int size);
ifstream iteratePastDescription(string file_name);
string toUpperString(string str);

struct Event
{
    string name;
    string description;
    string advisor_name;
    int pride_points;
};

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
        vector<Event> getEvents();
    
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