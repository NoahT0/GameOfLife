#ifndef TILE_H
#define TILE_H

#include "Player.h"
#include "Helper.h"

class Path;
Player advisorSelect(Player player, int player_index);
vector<Advisor> getAdvisors();
int findAdvisorByName(vector <Advisor> advisors, string name);
vector<Player> pathSelect(vector<Player> players, vector<string> path_description);
void displayPathOptions(Player player, int index, vector<string> path_descriptions);

struct Event
{
    string name;
    string description;
    string advisor_name;
    int main_stat;
};

class Tile
{
    public:
        Tile();
        char getColor();
        string getName();
        string getDescription();
        vector<int> getStats();
        string getAdditionalEffect();
        string getExtraData();

        void setColor(char color);
        void setName(string name);
        void setDescription(string description);
        void setStats(vector<int> stats);
        void setAdditionalEffect(string additional_effects);
        void setExtraData(string data);

        //Player applyEffect(Player player, vector<Event> events, int turn, int roll, bool extra_turn[1]);
        Player changePlayerStats(Player player);
        Player doRandom(Player player, vector<Event> events);
        Player moveBack(Player player);
        Player moveToPrevious(Player player, int roll);
        Player extraTurn(Player player);
        Player switchAdvisor(Player player, int turn);
        Player doRiddle(Player player);
        
        Event getRandomEvent(vector<Event> events);
        vector<string> getAllRiddles();
        string getRandomRiddle();
        string getStatWinsAndLoss();

    private:
        char _color;
        string _description;
        string _name;
        vector<int> _stats;
        string _additional_effect;
        string _extra_data;

};

#endif