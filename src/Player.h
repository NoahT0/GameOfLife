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
        vector<int> getStats();
        int getAge();
        int getPath();
        int getPosition();
        Advisor getAdvisor();
        int getNum();
        string getPlayerTitle();
        int getConvertedMainStat(); // all stats get converted to main stat

        void setName(string name);
        bool setStatAtIndex(int index, int stat);
        void setMainStat(int main_stat);
        void setStats(vector<int> stats);
        bool setAge(int age);
        bool setPath(int path);
        bool setPosition(int position);
        void setAdvisor(Advisor advisor);
        bool setNum(int num);

        bool addStatAtIndex(int index, int added_stat);
        void addMainStat(int added_main_stat);
        void addStats(vector<int> stats);
        void move();

        void displayProgress();
        void displayCharacter();
        void displayAdvisor();
        void printStats();
        void printFinalStats();
        void printStatsWithArt(int imageNum);

    private:
        string _name;
        int _age;
        Advisor _advisor;
        vector<int> _stats;
        int _main_stat;
        int _num;

        // Board stuff
        int _path;
        int _position;

        bool iterateToImage(ifstream &input_file, int imageNum);
};

#endif