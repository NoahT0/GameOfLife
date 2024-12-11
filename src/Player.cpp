#include "Player.h"
#include <fstream>
#include <iomanip>
#include <cassert>

vector<string> getStatNames();
string getMainStatName();
void printAdvisorProfile(Advisor advisor);
string toUpperString(string str);

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
    _num = 1;
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
vector<int> Player::getStats()
{
    return _stats;
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
int Player::getNum()
{
    return _num;
}
string Player::getPlayerTitle()
{
    return "Player " + to_string(_num) + ", " + _name;
}
int Player::getConvertedMainStat()
{   
    int total_main = _main_stat;
    for(int i = 0; i < _stats.size(); i++)
    {
        total_main += 10 * _stats[i];   // 100 normal stats equal 1000 main stats
    }

    return total_main;
}

void Player::setName(string name)
{
    _name = name;
}
bool Player::setStatAtIndex(int index, int stat)
{
    _stats[index] = stat;

    // Make sure stat is at least 100
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
void Player::setStats(vector<int> stats)
{
    _stats = stats;
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
    if(position < 0)
    {
        return false;
    }
    _position = position;
    return true;
}
void Player::setAdvisor(Advisor advisor)
{
    _advisor = advisor;
}
bool Player::setNum(int num)
{
    if(num <= 0)
    {
        return false;
    }

    _num = num;
    return true;
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

void Player::printStats()
{   
    // Print out name and age
    cout << _name << ", age " << _age << endl;

    // Print out stats
    vector<string> stat_names = getStatNames();
    assert(stat_names.size() == _stats.size());

    for(int i = 0; i < stat_names.size(); i++)
    {
        cout << stat_names[i] << ": " << _stats[i] << endl;
    }

    // Print main stat
    cout << getMainStatName() << ": " << _main_stat << endl;
    
}
void Player::printFinalStats()
{
    cout << "Player " << _num << " ";
    printStats();
    cout << "Converted " << getMainStatName() << ": " << getConvertedMainStat() << endl;
}
void Player::displayProgress()
{
    // Print main stat
    cout << getPlayerTitle() << " has " << _main_stat << " " << getMainStatName() << endl;

    // Print basic stats
    vector<string> stat_names = getStatNames();
    assert(stat_names.size() == _stats.size());

    for(int i = 0; i < stat_names.size()-1; i++)
    {
        cout << _stats[i] << " " << stat_names[i] << "," << endl;
    }
    
    cout << "and " << _stats[_stats.size()-1] << " " << stat_names[stat_names.size()-1] << "." << endl;

    // Display converted main stat
    cout << "Do you want to see how many converted " << getMainStatName() << "'s you have? Yes(Y) or No(N)." << endl;
    string choice;
    cin >> choice;
    if(toUpperString(choice) == "Y")
    {
        cout << getPlayerTitle() << " has " << getConvertedMainStat() << " converted " << getMainStatName() << "." << endl;
    }
    cin.ignore();
    
}
void Player::displayCharacter()
{
    cout << getPlayerTitle() << " is " << _age << endl;
}

// Print current advisor
void Player::displayAdvisor()
{
    if(_advisor.name.length() == 0)
    {
        cout << "No current advisor." << endl;
    }
    else
    {
        cout << getPlayerTitle() << "'s current advisor: " << endl;
        printAdvisorProfile(_advisor);
    }
    
    cout << "Do you want to keep using your current advisor? Yes(Y) or No(N)." << endl;
    
    string choice;
    cin >> choice;
    if(toUpperString(choice) == "N")
    {
        cout << "Too bad!" << endl;
    }
    else
    {
        cout << "Cool" << endl;
    }
    cin.ignore();
}