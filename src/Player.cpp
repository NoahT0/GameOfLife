#include "Player.h"
#include <fstream>
#include <iomanip>
#include <cassert>


int clamp(int num, int lower, int upper)
{
    if(lower > upper)
    {
        int temp = lower;
        lower = upper;
        upper = temp;
    }
    if(num<lower)
    {
        num = lower;
    }

    if(num>upper)
    {
        num = upper;
    }

    return num;
}
vector<string> getStatNames();
string getMainStatName();

Player::Player()
{
    _name = "";
    _age = 1;
    _path = 0;
    _position = 0;

}

Player::Player(string name, vector<int> stats)
{
    _name = name;
    _age = 1;
    _path = 0;
    _position = 0;

    _stats = stats;
    _main_stat = 0;

}

string Player::getName()
{
    return _name;
}

// int Player::getStrength()
// {
//     return _strength;
// }

// int Player::getStamina()
// {
//     return _stamina;
// }

// int Player::getWisdom()
// {
//     return _wisdom;
// }

int Player::getStatAtIndex(int index)
{
    return _stats[index];
}

int Player::getAge()
{
    return _age;
}

int Player::getMainStat()
{
    return _main_stat;
}

int Player::getPath()
{
    return _path;
}
int Player::getPosition()
{
    return _position;
}
Advisor Player::getAdvisor()
{
    return _advisor;
}
string Player::getPlayerTitle(int index)
{
    return "Player " + to_string(index+1) + ", " + _name;
}
void Player::setName(string name)
{
    _name = name;
}
// bool Player::setStrength(int strength)
// {
    
//     _strength = strength;
//     if(_strength <100)
//     {
//         _strength = 100;
//         return false;
//     }
//     return true;
// }
// bool Player::setStamina(int stamina)
// {
//     _stamina = stamina;
//     if(_stamina < 100)
//     {
//         _stamina = 100;
//         return false;
//     }
//     return true;
// }
// bool Player::setWisdom(int wisdom)
// {
//     _wisdom = wisdom;
//     if(_wisdom < 100)
//     {
//         _wisdom = 100;
//         return false;
//     }
//     return true;
// }
bool Player::setStatAtIndex(int index, int stat)
{
    _stats[index] = stat;
    if(_stats[index] < 100)
    {
        _stats[index] = 100;
        return false;
    }

    return true;
}
void Player::setMainStat(int main_stat)
{
    _main_stat = main_stat;
}
bool Player::setAge(int age)
{
    _age = age;
    if(_age < 1)
    {
        _age = 1;
        return false;
    }
    return true;
}

bool Player::setPath(int path)
{
    if(_path >= 0)
    {
        _path = path;
        return true;
    }
    return false;
    
}
bool Player::setPosition(int position)
{
    _position = position;
    return true;
}
void Player::setAdvisor(Advisor advisor)
{
    _advisor = advisor;
}

bool Player::addStatAtIndex(int index, int added_stat)
{
    return setStatAtIndex(index,_stats[index] + added_stat);
}
void Player::addMainStat(int added_main_stat)
{
    _main_stat += added_main_stat;
}
void Player::addStats(vector<int> stats)
{
    assert(stats.size() == _stats.size());
    for(int i = 0; i < stats.size(); i++)
    {
        addStatAtIndex(i, stats[i]);
    }
}

void Player::move()
{
    _position++;
}


void Player::printStatsWithArt(int imageNum)
{
    string stats_lines[5] = 
    {
        _name + ", age " + to_string(_age),
        //"Strength: " + to_string(_strength),
        //"Stamina: " + to_string(_stamina),
        //"Wisdom: " + to_string(_wisdom),
        "Pride Points: " + to_string(_main_stat)
        
    };

    
    ifstream input_file("Files/ascii_art.txt");
    if(!input_file.is_open())
    {
        return;
    }
    
    // Find start
    if(!iterateToImage(input_file, imageNum))
    {
        cout << "no image" << endl;
        return;
    }
    string line;

    // Print out stats alongside art
    int count = 0;
    while(getline(input_file, line) && line.length()>0)
    {
        if(count < 5)
        {
            cout << setw(20) << left << line << stats_lines[count] << endl;
            count ++;
        }
        else
        {
            cout << line << endl;
        }
        
    }

    // Print out stats that go past art
    while(count<5)
    {
        cout << setw(20) << "" << stats_lines[count] << endl;
        count++;
    }

    input_file.close();
}
bool Player::iterateToImage(ifstream &input_file, int imageNum)
{
    string line;
    int curImage = 0;
    while(curImage != imageNum && getline(input_file, line))
    {
        if(line.length() == 0)
        {
            curImage ++;
        }
    }
    if(curImage == imageNum)
    {
        return true;
    }
    return false;
}
void Player::printStats()
{   
    cout << _name << ", age " << _age << endl;
    vector<string> stat_names = getStatNames();
    assert(stat_names.size() == _stats.size());

    for(int i = 0; i < stat_names.size(); i++)
    {
        cout << stat_names[i] << ": " << _stats[i] << endl;
    }
    // cout << "Strength: " << _strength << endl;
    // cout << "Stamina: " << _stamina << endl;
    // cout << "Wisdom: " << _wisdom << endl;
    cout << getMainStatName() << ": " << _main_stat << endl;
    
}

void Player::displayProgress(int index)
{
    cout << getPlayerTitle(index) << " has " << _main_stat << " " << getMainStatName() << endl;

    vector<string> stat_names = getStatNames();
    assert(stat_names.size() == _stats.size());

    for(int i = 0; i < stat_names.size()-1; i++)
    {
        cout << _stats[i] << " " << stat_names[i] << "," << endl;
    }

    cout << "and " << _stats[_stats.size()-1] << " " << stat_names[stat_names.size()-1] << "." << endl;
    
    // cout << _stamina << " stamina," << endl;
    // cout << _strength << " strength," << endl;
    // cout << "and " << _wisdom << " wisdom." << endl;
}
void Player::displayCharacter(int index)
{
    cout << getPlayerTitle(index) << " is " << _age << endl;
}
void Player::displayAdvisor(int index)
{
    cout << getPlayerTitle(index) << " advisor's is " << _advisor.getName() << endl;
}