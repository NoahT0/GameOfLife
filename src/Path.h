#ifndef PATH_H
#define PATH_H

#include "Tile.h"

class Path
{
    public:
        Path(string name, vector<int> start_stats, bool start_advisor, int size);
        
        string getName();
        int getSize();
        bool getStartAdvisor();
        vector<int> getStartStats();
        int getStartMainStat();
        vector<Tile> getTiles();
        Tile getTileAtPos(int pos);
        vector<Event> getEvents();
        string getDescriptionDisplay();

        void setName(string name);
        void setStartAdvisor(bool start_advisor);
        void setStartStats(vector<int> stats);
        void setMainStat(int main_stat);
        void setTiles(vector<Tile> tiles);
        void setEvents(vector<Event> events);

        void displayTile(int pos, vector<int> on_tile, int width);
    
    private:
        string _name;
        bool _start_with_advisor;

        vector<int> _start_stats;
        int _start_main_stat;

        vector<Tile> _tiles;
        vector <Event> _events;
        
        // Functions for initializing tiles
        void initializeTiles(int size);
        int getGreenCount();
        vector<vector<int>> getTilePercentages();
        vector<Tile> getAllTiles();
        vector<string> getPossibleSpecialTileNames();
        vector<Tile> getPossibleSpecialTiles();
        Tile getTileByName(string name, vector<Tile>);

        // Functions for initializing events
        void initializeEvents();
        vector<Event> getPossibleEvents();
        Event getEventByName(string name, vector<Event>);

        // Functions for initializing colors
        vector<Color> getPossibleColors();
        Color getColorByName(vector<Color> colors, string name);

};

#endif