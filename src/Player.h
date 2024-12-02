#ifndef PLAYER_H
#define PLAYER_H

#include "Advisor.h"

int clamp(int num, int lower, int upper);

class Player
{
    public:
        Player();
        Player(string name, int strength, int stamina, int wisdom);
        Player(string name, int strength, int stamina, int wisdom, int path);

        string getName();
        int getStrength();
        int getStamina();
        int getWisdom();
        int getPridePoints();
        int getAge();
        int getPath();
        int getPosition();
        Advisor getAdvisor();
        string getPlayerTitle(int index);

        void setName(string name);
        bool setStrength(int strength);
        bool setStamina(int stamina);
        bool setWisdom(int wisdom);
        void setPridePoints(int pride_points);
        bool setAge(int age);
        bool setPath(int path);
        bool setPosition(int position);
        void setAdvisor(Advisor advisor);

        bool addStrength(int added_strength);
        bool addStamina(int added_stamina);
        bool addWisdom(int added_wisdom);
        void addPridePoints(int added_pride_points);
        void move();


        void displayProgress(int index);
        void displayCharacter(int index);
        void displayAdvisor(int index);
        void printStats();
        void printStatsWithArt(int imageNum);

    private:
        string _name;
        int _strength;
        int _stamina;
        int _wisdom;
        int _pride_points;
        int _age;
        Advisor _advisor;
        // Board stuff
        int _path;
        int _position;

        bool iterateToImage(ifstream &input_file, int imageNum);
        

};

#endif