#ifndef TILE_H
#define TILE_H

#include "Player.h"
#include "Helper.h"

Player advisorSelect(Player player, int player_index);
vector<Advisor> getAdvisors();
int findAdvisorByName(vector <Advisor> advisors, string name);

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

        void setColor(char color);
        void setName(string name);
        void setDescription(string description);
        void setStats(vector<int> stats);
        void setAdditionalEffect(string additional_effects);

        Player applyEffect(Player player, vector<Event> events, int turn, int roll, bool extra_turn[1]);
        Player changePlayerStats(Player player);
        Player doRandom(Player player, vector<Event> events);
        Player moveBack(Player player);
        Player moveToPrevious(Player player, int roll);
        Player switchAdvisor(Player player, int turn);
        Player doRiddle(Player player);
        
        Event getRandomEvent(vector<Event> events);
        vector<string> getAllRiddles();
        string getRandomRiddle();

    private:
        char _color;
        string _description;
        string _name;
        vector<int> _stats;
        string _additional_effect;

};

#endif