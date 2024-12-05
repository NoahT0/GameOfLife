#ifndef PLAYER_H
#define PLAYER_H

#include "Advisor.h"

class Player
{
    public:
        Player();
        Player(string name, vector<int> stats);

        string getName();
        int getStatAtIndex(int index);
        int getMainStat();
        int getAge();
        int getPath();
        int getPosition();
        Advisor getAdvisor();
        string getPlayerTitle(int index);

        void setName(string name);

        bool setStatAtIndex(int index, int stat);
        void setMainStat(int main_stat);
        bool setAge(int age);
        bool setPath(int path);
        bool setPosition(int position);
        void setAdvisor(Advisor advisor);

        bool addStatAtIndex(int index, int added_stat);
        void addMainStat(int added_main_stat);
        void addStats(vector<int> stats);
        void move();

        void displayProgress(int index);
        void displayCharacter(int index);
        void displayAdvisor(int index);
        void printStats();
        void printStatsWithArt(int imageNum);

    private:
        string _name;
        int _age;
        Advisor _advisor;
        vector<int> _stats;
        int _main_stat;

        // Board stuff
        int _path;
        int _position;

        bool iterateToImage(ifstream &input_file, int imageNum);
        

};

#endif