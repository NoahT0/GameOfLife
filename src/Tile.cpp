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

vector<Player> pathSelect(vector<Player> players, vector<string> path_description)
{
    //cout << "Size: " << paths.size() << endl;
    for(int i = 0; i<players.size(); i++)
    {
        string choice;
        bool is_path_picked = false;

        // Loop until player picks a valid path
        while(!is_path_picked)
        {
            
            // Display path options
            displayPathOptions(players[i], i, path_description);
            
            cin >> choice;
            // First check if user entered an int then make sure it is a valid choice
            if(!validateInt(choice) || stoi(choice) < 1 || stoi(choice) > path_description.size())
            {
                cout << "Invalid path. Enter a number between 1 and " << path_description.size() << endl;
            }
            else
            {
                players[i].setPath(stoi(choice)-1);
                
                int new_line_idx = getCharIndex(path_description[stoi(choice)-1], '\n');
                string name = path_description[stoi(choice)-1].substr(0,new_line_idx);
                cout << players[i].getPlayerTitle(i) << " chose " << name << "!" << endl;
                is_path_picked = true;
                
            }
            cout << endl;
        }
        
    }
    cin.ignore();
    return players;
}

void displayPathOptions(Player player, int index, vector<string> path_descriptions)
{
    cout << "Path options: " << endl;
    for(int i = 0; i < path_descriptions.size(); i++)
    {
        cout << (i+1) << ". " << path_descriptions[i] << endl;
        cout << endl;
    }
    cout << player.getPlayerTitle(index) << " enter path with corresponding number: " << endl;
}

Tile::Tile()
{
    _color = Color();
}

Color Tile::getColor()
{
    return _color;
}

void Tile::setColor(Color color)
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
string Tile::getExtraData()
{
    return _extra_data;
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
void Tile::setExtraData(string data)
{
    _extra_data = data;
}
Player Tile::changePlayerStats(Player player)
{
    player.addStats(_stats);

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
    assert(validateInt(_extra_data));   // Make sure extra data is an int

    // Move back 10
    cout << "Move back " + _extra_data +  " tiles. " + getStatWinsAndLoss() << endl;

    int newPos = player.getPosition()-stoi(_extra_data);
    newPos = clamp(newPos,0,player.getPosition());
    player.setPosition(newPos);

    return player;
}
Player Tile::moveToPrevious(Player player, int roll)
{
    cout << "Return to previous position. " + getStatWinsAndLoss() << endl;
    int newPos = player.getPosition()-roll;
    player.setPosition(newPos);

    return player;
}
Player Tile::extraTurn(Player player)
{
    cout << "Gain an extra turn. " << getStatWinsAndLoss() << endl;
    
    return player;
}
Player Tile::switchAdvisor(Player player, int turn)
{
    cout << getStatWinsAndLoss() << endl;
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
    vector<string> riddle_arr = vectorSplit(riddle, '|');
    assert(riddle_arr.size() == 4); // Must contain 4 elements: Stat Name|Question|Answer|Points rewarded

    string stat_boost_name = riddle_arr[0];
    string question = riddle_arr[1];
    string correct_answer = riddle_arr[2];

    assert(validateInt(riddle_arr[3])); // Must contain an int specifying how many points the player gets
    int boost = stoi(riddle_arr[3]);

    // Display possible reward and ask question
    cout << "Answer correctly, and you'll earn a boost of " << boost << " points to your " << stat_boost_name << " Trait—your cleverness pays off!" << endl;
    cout << question << endl;
    
    string answer;
    getline(cin,answer);

    int index = getIndexOfStatByName(stat_boost_name);
    assert(index>=0);   // Make sure the stat is actually found

    if(toUpperString(answer) == toUpperString(correct_answer))
    {
        cout << "You answered correct!" << endl;
        player.addStatAtIndex(index, boost);  // CHANGE LATER
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
    getline(riddle_file,line);  // Increment past description line

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

string Tile::getStatWinsAndLoss()
{
    // Get vector containing all the different possible stat values
    vector<int> all_stats;

    for(int i = 0; i < _stats.size(); i++)
    {
        bool found = false;
        for(int j = 0; j < all_stats.size(); j++)
        {
            if(_stats[i] == all_stats[j])
            {
                found = true;
            }
        }
        if(!found && _stats[i] != 0)
        {
            all_stats.push_back(_stats[i]);
        }
    }

    vector<string> stat_names = getStatNames();

    // Fill 2d vector that has rows of names. Index of a row corresponds to index of all_stats[index] value
    vector<vector<string>> all_names;
    for(int i = 0; i < all_stats.size(); i++)
    {
        vector<string> names;
        for(int j = 0; j < _stats.size(); j++)
        {
            if(all_stats[i] == _stats[j])
            {
                names.push_back(stat_names[j]);
            }
            
        }
        all_names.push_back(names);
    }

    // Create string based off the stat names that are changing.
    string result;
    for(int i = 0; i < all_names.size(); i++)
    {
        int num = all_stats[i];

        if(num < 0)
        {
            result += "Lose " + to_string(num * -1) + " ";
        }
        else
        {
            result += "Receive " + to_string(num) + " ";
        }

        int size = all_names[i].size();
        if(size == 1)
        {
            result += all_names[i][0] + ". ";
        }
        else
        {
            for(int j = 0; j < size-1; j++)
            {
                result += all_names[i][j];
                if(j < size - 2)
                {
                    result += ",";
                }
                result += " ";
                
            }

            result += "and " + all_names[i][size-1] + ". ";
        }
        
    }
    return result;

}
