#ifndef ADVISOR_H
#define ADVISOR_H

#include <iostream>

using namespace std;

class Advisor
{
    public:
        Advisor();
        Advisor(string name, string special_ability, int num);

        string getName();
        string getSpecialAbility();
        int getNum();

        void setName(string name);
        void setSpecialAbility(string special_ability);
        void setNum(int num);

        void printProfile();
    private:
        string _name;
        string _special_ability;
        int _num;
    
};

#endif