#include "Tile.h"
#include <fstream>
#include <cassert>

int findAdvisorByName(vector <Advisor> advisors, string name)
{
    for(int i = 0; i<advisors.size(); i++)
    {
        if(toUpperString(advisors[i].getName()) == toUpperString(name))
        {
            return i;
        }
    }
    return -1;
}
vector<Advisor> getAdvisors()
{
    vector<Advisor> advisors;
    
    //ifstream advisor_file("Files/advisors.txt");
    ifstream advisor_file = getInputStream("advisors.txt");

    string line;
    int count = 1;
    while(getline(advisor_file,line))
    {
        string arr[2];
        split(line, '|',arr,2);
        Advisor advisor(arr[0], arr[1], count);

        advisors.push_back(advisor);
        count++;
    }
    advisor_file.close();
    return advisors;
}
Player advisorSelect(Player player, int player_index)
{
    cin.ignore();
    int advisor_index = -1;
    while(advisor_index<0)
    {
        cout << "Advisor choices: " << endl;
        vector<Advisor> advisors = getAdvisors();
        for(int i = 0; i<advisors.size(); i++)
        {
            advisors[i].printProfile();
        }
        cout << endl;
        cout << player.getPlayerTitle(player_index) << ", enter advisor name:" << endl;
        string name;
        getline(cin, name);
        advisor_index = findAdvisorByName(advisors,name);

        cout << endl;
        if(advisor_index<0)
        {
            cout << "Invalid advisor name. Try again." << endl;
        }
        else
        {
            player.setAdvisor(advisors[advisor_index]);
            cout << player.getPlayerTitle(player_index) << " chose " << advisors[advisor_index].getName() << "!" << endl;
        }
        cout << endl;
    }

    return player;
}

Tile::Tile()
{
    _color = 'G';
}

char Tile::getColor()
{
    return _color;
}

void Tile::setColor(char color)
{
    _color = color;
}

string Tile::getName()
{
    return _name;
}
string Tile::getDescription()
{
    return _description;
}

vector<int> Tile::getStats()
{
    return _stats;
}
string Tile::getAdditionalEffect()
{
    return _additional_effect;
}

void Tile::setName(string name)
{
    _name = name;
}
void Tile::setDescription(string description)
{
    _description = description;
}

void Tile::setStats(vector<int> stats)
{
    _stats = stats;
}

void Tile::setAdditionalEffect(string additional_effect)
{
    _additional_effect = additional_effect;
}

Player Tile::changePlayerStats(Player player)
{
    player.addStats(_stats);

    return player;
} 

Player Tile::applyEffect(Player player, vector<Event> events, int turn, int roll, bool extra_turn[1])
{

    cout << _description << endl;
    player = changePlayerStats(player);
    
    if(_additional_effect == "random")
    {
        player = doRandom(player, events);
    }
    else if(_additional_effect == "back")
    {
        player = moveBack(player);
    }
    else if(_additional_effect == "previous")
    {
        player = moveToPrevious(player, roll);
    }
    else if(_additional_effect == "extra")
    {
        extra_turn[0] = true;
    }
    else if(_additional_effect == "advisor")
    {
        player = switchAdvisor(player, turn);
    }
    else if(_additional_effect == "riddle")
    {
        player = doRiddle(player);
    }
    return player;
}

Player Tile::doRandom(Player player, vector<Event> events)
{
    // 50% chance to do random event
    int do_random = rand() % 2;
    
    if(do_random == 0)
    {
        Event event = getRandomEvent(events);

        cout << event.description << endl;
        if(event.main_stat>0)
        {
            cout << "Gain " << event.main_stat << " " << getMainStatName() << "!" << endl;
            player.addMainStat(event.main_stat);
        }
        else if(player.getAdvisor().getName() == event.advisor_name)
        {
            cout << event.advisor_name << " protects you!" << endl;
        }
        else
        {
            cout << "You lose " << (event.main_stat*-1) << " " << getMainStatName() << "." << endl;
            player.addMainStat(event.main_stat);
        }   

    }

    return player;
}
Player Tile::moveBack(Player player)
{
    // Move back 10
    int newPos = player.getPosition()-10;
    newPos = clamp(newPos,0,player.getPosition());
    player.setPosition(newPos);

    return player;
}
Player Tile::moveToPrevious(Player player, int roll)
{
    int newPos = player.getPosition()-roll;
    player.setPosition(newPos);

    return player;
}
Player Tile::switchAdvisor(Player player, int turn)
{
    if(player.getAdvisor().getName() == "none")
    {
        player = advisorSelect(player,turn);
    }
    else
    {
        cout << "Your current advisor:" << endl;
        player.getAdvisor().printProfile();
        cout << "Would you like to switch advisors? Yes (Y) or No (N)" << endl;
        char choice;
        cin >> choice;
        if(choice == 'Y')
        {
            player = advisorSelect(player,turn);
        }
    }

    return player;
}
Player Tile::doRiddle(Player player)
{
    // Test players with random riddle and reward with wisdom if they get it correct
    string riddle = getRandomRiddle();
    string riddle_arr[3];
    split(riddle, '|',riddle_arr, 3);
    cout << "Answer correctly, and you'll earn a boost of 500 Points to your " << riddle_arr[0] << " Trait—your cleverness pays off!" << endl;
    cout << riddle_arr[1] << endl;
    string answer;

    cin.ignore();
    getline(cin,answer);

    int index = getIndexOfStatByName(riddle_arr[0]);
    assert(index>=0);   // Make sure the stat is actually found

    if(toUpperString(answer) == toUpperString(riddle_arr[2]))
    {
        cout << "You answered correct!" << endl;
        player.addStatAtIndex(index, 500);  // CHANGE LATER
    }
    else
    {
        cout << "Wrong lmao." << endl;
    }

    return player;
}

Event Tile::getRandomEvent(vector<Event> events)
{
    int choice = rand() % events.size();
    return events[choice];
}

vector<string> Tile::getAllRiddles()
{

    ifstream riddle_file = getInputStream("riddles.txt");

    vector<string> riddles;

    string line;
    while(getline(riddle_file, line))
    {
        riddles.push_back(line);
    }

    riddle_file.close();

    return riddles;
}
string Tile::getRandomRiddle()
{
    vector<string> riddles = getAllRiddles();
    int riddle_num = rand() % riddles.size();

    return riddles[riddle_num];
}
