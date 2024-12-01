#include "Advisor.h"

Advisor::Advisor()
{
    _name = "none";
    _special_ability = "n/a";
    _num = 0;
}

Advisor::Advisor(string name, string special_ability, int num)
{
    _name = name;
    _special_ability = special_ability;
    _num = num;
}

string Advisor::getName()
{
    return _name;
}
string Advisor::getSpecialAbility()
{
    return _special_ability;
}
int Advisor::getNum()
{
    return _num;
}

void Advisor::setName(string name)
{
    _name = name;
}
void Advisor::setSpecialAbility(string special_ability)
{
    _special_ability = special_ability;
}
void Advisor::setNum(int num)
{
    _num = num;
}

void Advisor::printProfile()
{
    cout << "Name: " << _name << endl;
    cout << "Special ability: " << _special_ability << endl;
}