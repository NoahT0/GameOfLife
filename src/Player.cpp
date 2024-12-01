#include "Player.h"
#include <fstream>
#include <iomanip>

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


Player::Player()
{
    _name = "";
    _strength = 100;
    _stamina = 100;
    _wisdom = 100;
    _age = 1;
    _pride_points = 0;
    _path = 0;
    _position = 0;

}

Player::Player(string name, int strength, int stamina, int wisdom)
{
    _name = name;
    _strength = strength;
    _stamina = stamina;
    _wisdom = wisdom;
    _age = 1;
    _strength = clamp(_strength, 100, 1000);
    _stamina = clamp(_stamina, 100, 1000);
    _wisdom = clamp(_wisdom, 100, 1000);
    _pride_points = 0;
    _path = 0;
    _position = 0;
}
Player::Player(string name, int strength, int stamina, int wisdom, int path)
{
    _name = name;
    _strength = strength;
    _stamina = stamina;
    _wisdom = wisdom;
    _age = 1;
    _strength = clamp(_strength, 100, 1000);
    _stamina = clamp(_stamina, 100, 1000);
    _wisdom = clamp(_wisdom, 100, 1000);
    _pride_points = 0;
    _path = path;
    _position = 0;
}

string Player::getName()
{
    return _name;
}

int Player::getStrength()
{
    return _strength;
}

int Player::getStamina()
{
    return _stamina;
}

int Player::getWisdom()
{
    return _wisdom;
}

int Player::getAge()
{
    return _age;
}

int Player::getPridePoints()
{
    return _pride_points;
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
bool Player::setStrength(int strength)
{
    
    _strength = strength;
    if(_strength <100)
    {
        _strength = 100;
        return false;
    }
    return true;
}
bool Player::setStamina(int stamina)
{
    _stamina = stamina;
    if(_stamina < 100)
    {
        _stamina = 100;
        return false;
    }
    return true;
}
bool Player::setWisdom(int wisdom)
{
    _wisdom = wisdom;
    if(_wisdom < 100)
    {
        _wisdom = 100;
        return false;
    }
    return true;
}
void Player::setPridePoints(int pride_points)
{
    _pride_points = pride_points;
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

bool Player::addStrength(int added_strength)
{
    return setStrength(_strength + added_strength);
}
bool Player::addStamina(int added_stamina)
{
    return setStamina(_stamina + added_stamina);
}
bool Player::addWisdom(int added_wisdom)
{
    return setWisdom(_wisdom + added_wisdom);
}
void Player::addPridePoints(int added_pride_points)
{
    _pride_points += added_pride_points;
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
        "Strength: " + to_string(_strength),
        "Stamina: " + to_string(_stamina),
        "Wisdom: " + to_string(_wisdom),
        "Pride Points: " + to_string(_pride_points)
        
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
    cout << "Strength: " << _strength << endl;
    cout << "Stamina: " << _stamina << endl;
    cout << "Wisdom: " << _wisdom << endl;
    cout << "Pride Points: " << _pride_points << endl;
    
}