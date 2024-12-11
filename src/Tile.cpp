#include "Tile.h"
#include <fstream>
#include <cassert>

int findAdvisorByName(vector <Advisor> advisors, string name)
{
    // Loop through advisors unitl advisor with particular name is found
    for(int i = 0; i<advisors.size(); i++)
    {
        if(toUpperString(advisors[i].name) == toUpperString(name))
        {
            return i;
        }
    }
    return -1;
}
vector<Advisor> getAdvisors()
{
    vector<Advisor> advisors;
    
    ifstream advisor_file = getInputStream("advisors.txt");

    string line;
    getline(advisor_file, line);
    while(getline(advisor_file,line))
    {
        // advisor name|advisor ability
        string arr[2];
        split(line, '|',arr,2);
        Advisor advisor = {arr[0], arr[1]};

        advisors.push_back(advisor);
    }
    advisor_file.close();
    return advisors;
}
Player advisorSelect(Player player)
{
    vector<Advisor> advisors = getAdvisors();

    // Loop until valid advisor is selected
    int advisor_index = -1;
    while(advisor_index<0)
    {
        // Display advisor options
        cout << "Advisor choices: " << endl;
        
        for(int i = 0; i<advisors.size(); i++)
        {
            printAdvisorProfile(advisors[i]);
        }
        cout << endl;

        // Prompt and get input for name
        cout << player.getPlayerTitle() << ", enter advisor name:" << endl;
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
            // Set player advisor and display chosen advisor
            player.setAdvisor(advisors[advisor_index]);
            cout << player.getPlayerTitle() << " chose " << advisors[advisor_index].name << "!" << endl;
        }
        cout << endl;
    }
    return player;
}
void printAdvisorProfile(Advisor advisor)
{
    cout << "Name: " << advisor.name << endl;
    cout << "Special ability: " << advisor.special_ability << endl;
}
vector<Player> pathSelect(vector<Player> players, vector<string> path_description, vector<int> sizes)
{
    for(int i = 0; i < players.size(); i++)
    {
        players[i] = playerPathSelect(players[i], path_description, sizes);
    }
    return players;
}
Player playerPathSelect(Player player, vector<string> path_description, vector<int> sizes)
{
    string choice;
    bool is_path_picked = false;

    // Loop until player picks a valid path
    while(!is_path_picked)
    {
        // Display path options
        displayPathOptions(player, path_description);
        
        cin >> choice;
        // First check if user entered an int then make sure it is a valid choice
        if(!validateInt(choice) || stoi(choice) < 1 || stoi(choice) > path_description.size())
        {
            cout << "Invalid path. Enter a number between 1 and " << path_description.size() << endl;
        }
        else
        {
            int choice_index = stoi(choice)-1;
            player.setPath(choice_index);
            
            // For when player switches paths. If player switches to path where they are already past the final index of the new path
            // Then set the position to the final index of the new path
            if(player.getPosition() > sizes[choice_index]-1)
            {
                player.setPosition(sizes[choice_index] - 1);
            }
            
            int new_line_idx = getCharIndex(path_description[choice_index], '\n'); // Extract name from description string
            string name = path_description[choice_index].substr(0,new_line_idx);
            cout << player.getPlayerTitle() << " chose " << name << "!" << endl;
            is_path_picked = true;
            
        }
        cout << endl;
    }
    cin.ignore();
    return player;
}
void displayPathOptions(Player player, vector<string> path_descriptions)
{
    cout << "Path options: " << endl;
    for(int i = 0; i < path_descriptions.size(); i++)
    {
        cout << (i+1) << ". " << path_descriptions[i] << endl;
        cout << "-------------------------" << endl;
    }
    cout << player.getPlayerTitle() << " enter path with corresponding number: " << endl;
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
        // Get random event and print its description
        Event event = getRandomEvent(events);
        cout << event.description << endl;

        if(event.main_stat>0)
        {
            // player gains main_stat from event
            cout << "Gain " << event.main_stat << " " << getMainStatName() << "!" << endl;
            player.addMainStat(event.main_stat);
        }
        else if(player.getAdvisor().name == event.advisor_name)
        {
            // player has advisor that prevents losing main_stat
            cout << event.advisor_name << " protects you!" << endl;
        }
        else
        {
            // player loses main_stat from event
            cout << "You lose " << (event.main_stat*-1) << " " << getMainStatName() << "." << endl;
            player.addMainStat(event.main_stat);
        }   

    }

    return player;
}
Player Tile::moveBack(Player player)
{
    assert(validateInt(_extra_data));   // Make sure extra data is an int

    // Move back "_extra_data" amount
    cout << "Move back " + _extra_data +  " tiles. " + getStatWinsAndLoss() << endl;

    int newPos = player.getPosition()-stoi(_extra_data);
    newPos = clamp(newPos,0,player.getPosition());
    player.setPosition(newPos);

    return player;
}
Player Tile::moveToPrevious(Player player, int roll)
{
    // Move back by the previous roll amount
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
Player Tile::switchAdvisor(Player player)
{
    cout << getStatWinsAndLoss() << endl;
    // If players don't have adivsor they get to choose one
    if(player.getAdvisor().name.length() == 0)
    {
        player = advisorSelect(player);
    }
    else
    {
        // Print current advisor and prompt if they want to switch
        cout << "Your current advisor:" << endl;
        printAdvisorProfile(player.getAdvisor());
        cout << "Would you like to switch advisors? Yes (Y) or No (N)" << endl;

        // If they choose 'Y' they are able to choose new advisor
        char choice;
        cin >> choice;
        if(toupper(choice) == 'Y')
        {
            cin.ignore();
            player = advisorSelect(player);
        }
        
    }

    return player;
}
Player Tile::switchPath(Player player, string path_name, vector<string> path_description, vector<int> sizes)
{
    cout << getStatWinsAndLoss() << endl;

    // Print current path and prompt if player wants to switch
    cout << "Your current path:" << endl;
    cout << path_name << endl;
    cout << "Would you like to switch paths? Yes (Y) or No (N)" << endl;

    // If they choose 'Y' they are able to choose new path
    char choice;
    cin >> choice;
    if(toupper(choice) == 'Y')
    {
        cin.ignore();
        
        player = playerPathSelect(player, path_description, sizes);
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
        player.addStatAtIndex(index, boost);
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

// Returns string saying what particular tile does to stat values
// Ex: If tile has stats: -100|-100|-100, then return string Lose 100 stat1, stat2 and stat3
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
        // If particular stat value is not found in vector and not just 0 then add it to all stats
        // Ex: all_stats = {100,-300} and _stats[i] = 400. then 400 will be added to all_stats -> all_stats = {100,-300,400}
        // Ex: all_stats = {100,-300} and _stats[i] = 100. then it won't be added so stil -> all_stats = {100,-300}
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
    // string in form: Lose 500: stat1, Receive 600: stat2, stat3. etc
    string result;
    for(int i = 0; i < all_names.size(); i++)
    {
        int num = all_stats[i];
        
        // If all_stats is positive than start string with Receive else start with Lose
        // Then add the amount player is losing or adding
        if(num < 0)
        {
            result += "Lose " + to_string(num * -1) + " ";
        }
        else
        {
            result += "Receive " + to_string(num) + " ";
        }

        int size = all_names[i].size();

        // If only one stat is being changed by particular value add that name followed by a period.
        if(size == 1)
        {
            result += all_names[i][0] + ". ";
        }
        else
        {
            // size - 1 so and can be put before last element
            for(int j = 0; j < size-1; j++)
            {
                result += all_names[i][j];

                // No comma before the and
                if(j < size - 2)
                {
                    result += ",";
                }
                result += " ";
                
            }

            // add last element to result with and before
            result += "and " + all_names[i][size-1] + ". ";
        }
        
    }
    return result;

}
