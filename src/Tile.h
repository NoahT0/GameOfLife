#ifndef TILE_H
#define TILE_H

#include "Player.h"
#include "Helper.h"

class Path;
Player advisorSelect(Player player);
vector<Advisor> getAdvisors();
int findAdvisorByName(vector <Advisor> advisors, string name);
void printAdvisorProfile(Advisor advisor);
vector<Player> pathSelect(vector<Player> players, vector<string> path_description, vector<int> sizes);
Player playerPathSelect(Player player, vector<string> path_description, vector<int> sizes);
void displayPathOptions(Player player, vector<string> path_descriptions);

struct Event
{
    string name;
    string description;
    string advisor_name;
    int main_stat;
};

struct Color
{
    string name;
    string color_value;
};

class Tile
{
    public:
        Tile();
        Color getColor();
        string getName();
        string getDescription();
        vector<int> getStats();
        string getAdditionalEffect();
        string getExtraData();

        void setColor(Color color);
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
        Player switchAdvisor(Player player);
        Player doRiddle(Player player);
        Player switchPath(Player player, string path_name, vector<string> path_description, vector<int> sizes);
        
        Event getRandomEvent(vector<Event> events);
        vector<string> getAllRiddles();
        string getRandomRiddle();
        string getStatWinsAndLoss();

    private:
        Color _color;
        string _description;
        string _name;
        vector<int> _stats;
        string _additional_effect;
        string _extra_data;

};

#endif