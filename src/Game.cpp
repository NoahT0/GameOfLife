#include "Game.h"
#include <fstream>

bool validateInt(string input)
{
    int counter = 0;
    if(input.length() == 0)
    {
        return 0;
    }
    if(input[0] == '-')
    {
        if(input[1] < 48 || input[1] > 57)
        {
            return false;
        }
        counter++;
    }
    for(unsigned int i = counter; i<input.length(); i++)
    {
        if(input[i] < 48 || input[i] > 57)
        {
            return false;
        }
    }
    return true;
}

int getNumOfLinesInFile(string file_name)
{
    ifstream input_file = getInputStream(file_name);

    string line;
    int count = 0;
    while(getline(input_file, line))
    {
        if(line.length() > 0)
        {
            count++;
        }  
    }

    input_file.close();

    return count;
}

Game::Game()
{
    vector<Player> players =  characterSelect();
    _board = Board(players);
    
    players = pathSelect(players, _board.getPaths());
    cout << "Player"<< players[0].getPath() << endl;
    players = initializePlayersOnPath(players);
    
    _board.setPlayers(players);
    _board.displayBoard();
    _turn = 0;
    
}
vector<Player> Game::characterSelect()
{
    vector<Player> possible_players = getPossiblePlayers();
    vector<Player> players;
    string name;
    int count = 1;
    while(possible_players.size()>0 && name != "X")
    {
        // Print possible character options
        cout << "Possible characters: " << endl;
        printPlayerStats(possible_players);
        cout << "Player " << count << " enter character name or 'X' to move one:" << endl;
        cin >> name;
        name = toUpperString(name);

        if(name != "X")
        {
            int characterIndex = findCharacterByName(possible_players, name);

            if(characterIndex<0)
            {
                cout << "Invalid character name. Try again." << "\n\n\n";
            }
            else
            {
                // Add selected character to vector of players and update remaining characters that can be chosen
                players.push_back(possible_players[characterIndex]);
                cout << endl;
                cout << "Player " << count << " selected " << possible_players[characterIndex].getName() << "!\n\n\n";
                count ++;

                possible_players.erase(possible_players.begin()+characterIndex);
            }
        }
    }
    cout << endl;
    return players;
}
vector<Player> Game::pathSelect(vector<Player> players, vector<Path> paths)
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
            cout << players[i].getPlayerTitle(i) << " select path: " << endl;
            cout << "1. " << paths[0].getName() << "(1)";
            for(int i = 1; i<paths.size(); i++)
            {
                cout << " or " << (i+1) << ". " << paths[i].getName() << "(" << (i+1) << ")";
            }
            cout << endl;
            
            cin >> choice;
            // First check if user entered an int then make sure it is a valid choice
            if(!validateInt(choice) || stoi(choice) < 1 || stoi(choice) > paths.size())
            {
                cout << "Invalid path. Enter a number between 1 and " << paths.size() << endl;
            }
            else
            {
                players[i].setPath(stoi(choice)-1);
                
                cout << players[i].getPlayerTitle(i) << " chose " << paths[stoi(choice)-1].getName() << "!" << endl;
                is_path_picked = true;
                
            }
            cout << endl;
        }
        
    }
    
    return players;
}
int Game::findCharacterByName(vector <Player> players, string name)
{
    for(int i = 0; i<players.size(); i++)
    {
        if(toUpperString(players[i].getName()) == name)
        {
            return i;
        }
    }
    return -1;
}
void Game::printPlayerStats(vector<Player> players)
{
    for(int i = 0; i<players.size(); i++)
    {
        players[i].printStats();
        cout << endl;
    }
}
vector<Player> Game::getPossiblePlayers()
{
    string line;
    vector<Player> possible_players;

    //ifstream character_input("Files/characters.txt");
    ifstream character_input = getInputStream("characters.txt");

    while(getline(character_input, line))
    {
        // playerName|age|strength|stamina|wisdom|pridePoints
        string arr[6];
        split(line,'|',arr,6);
        Player player(arr[0],stoi(arr[2]), stoi(arr[3]),stoi(arr[4]));
        player.setAge(stoi(arr[1]));
        player.setPridePoints(20000);
        possible_players.push_back(player);

    }
    character_input.close();

    return possible_players;
}
// Add starting leadership and pride points and select advisor if path automatically gives advisor
vector<Player> Game::initializePlayersOnPath(vector<Player> players)
{
    vector <Path> paths = _board.getPaths();
    for(int i = 0; i<players.size(); i++)
    {

        Path path = paths[players[i].getPath()];
        // Add path's starting leadership and pride points 
        players[i].addPridePoints(path.getStartPridePoints());
        players[i].addStamina(path.getStartStamina());
        players[i].addWisdom(path.getStartWisdom());
        players[i].addStrength(path.getStartStrength());

        // If path starts with advisor then select advisor
        if(path.getStartAdvisor())
        {
            players[i] = advisorSelect(players[i], i);
        }
        else
        {
            players[i].setAdvisor(Advisor());
        }
        
    }
    return players;
}
Player Game::advisorSelect(Player player, int player_index)
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
        cin >> name;
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

int Game::findAdvisorByName(vector <Advisor> advisors, string name)
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
vector <Advisor> Game::getAdvisors()
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


string Game::getRandomRiddle()
{
    int num_riddles = getNumOfLinesInFile("riddles.txt");
    int riddle_num = rand() % num_riddles;

    ifstream riddle_file = getInputStream("riddles.txt");

    string line;
    int count = 0;

    while(getline(riddle_file, line) && count != riddle_num)
    {
        if(line.length() > 0)
        {
            count++;
        }  
    }

    riddle_file.close();

    return line;
}

int Game::getNumOfEvents(int path)
{
   return 0;
}
string Game::getRandomEvent()
{
    int num_events = getNumOfLinesInFile("random_events.txt");
    int event_number = rand() % num_events;

    ifstream event_file = getInputStream("random_events.txt");

    string line;
    int count = 0;

    while(getline(event_file, line) && count != event_number)
    {
        if(line.length() > 0)
        {
            count++;
        }  
    }

    event_file.close();

    return line;
}

void Game::takeTurn()
{
    Player player = _board.getPlayerAtIndex(_turn);
    
    string choice;
    
    // Loop until player actually moves
    while(choice != "5")
    {
        displayMenu();
        cin >> choice;
        
        while(!validateInt(choice) || stoi(choice) > 5 || stoi(choice) < 1)
        {
            cout << "Invalid choice try again." << endl;
            cout << endl;
            displayMenu();
            cin >> choice;
        }

        doPlayerAction(stoi(choice));

        if(choice != "5")
        {
            choice = "";
        }
        
    }
    
    _turn ++;
    _turn %= getNumPlayers();
}
void Game::doPlayerAction(int choice)
{
    Player player = _board.getPlayerAtIndex(_turn);
    
    switch(choice)
    {
        case 1:
            cout << player.getPlayerTitle(_turn) << " has " << player.getPridePoints() << " pride points," << endl;
            cout << player.getStamina() << " stamina," << endl;
            cout << player.getStrength() << " strength," << endl;
            cout <<"and " << player.getWisdom() << " wisdom." << endl;
            break;
        case 2:
            cout << player.getPlayerTitle(_turn) << " is " << player.getAge() << endl;
            break;
        case 3:
            _board.displayBoard();
            break;
        case 4:
            cout << player.getPlayerTitle(_turn) << " advisor's is " << player.getAdvisor().getName() << endl;
            break;
        case 5:
            movePlayer();
            break;

    }
    cout << endl;
}
void Game::movePlayer()
{
    Player player = _board.getPlayerAtIndex(_turn);

    cout << "Spinning..." << endl;

    int roll = rand() % 6 + 1;
    cout << player.getPlayerTitle(_turn) << " rolled a " << roll << "." << endl;

    _board.movePlayer(_turn,roll);
    player = _board.getPlayerAtIndex(_turn);
    _board.displayBoard();
    player = applyTileEffect(roll);

    _board.setPlayerAtIndex(_turn, player); 

    if(_extra_turn)
    {
        _extra_turn = false;
        _turn--;
    }
}
void Game::displayMenu()
{
    Player player = _board.getPlayerAtIndex(_turn);
    cout << "Main Menu: Select an option to coninue" << endl;
    cout << "1. Check Player Progress (1)" << endl;
    cout << "2. Review Character (2)" << endl;
    cout << "3. Check Position (3)" << endl;
    cout << "4. Review your Advisor (4)" << endl;
    cout << "5. Move Forward (5)" << endl;
    cout << endl;
    cout << player.getPlayerTitle(_turn) << " please choose an option using the corresponding number:" << endl;
}
int Game::getNumPlayers()
{
    return _board.getPlayers().size();
}

Player Game::applyTileEffect(int roll)
{
    Player player = _board.getPlayerAtIndex(_turn);
    Tile tile = _board.getTileAtPlayer(_turn);
    cout << tile.getDescription() << endl;
    player = tile.changePlayerStats(player);
    
    string effect = tile.getAdditionalEffect();
    if(effect == "random")
    {

        int do_random = rand() % 2;
        
        if(do_random == 0)
        {
            string event = getRandomEvent();
            string arr[4];
            split(event, '|', arr,4);
        }
    }
    else if(effect == "back")
    {
        int newPos = player.getPosition()-10;
        newPos = clamp(newPos,0,_board.getBoardSize());
        player.setPosition(newPos);
    }
    else if(effect == "previous")
    {
        int newPos = player.getPosition()-roll;
        player.setPosition(newPos);
        cout << player.getPosition() << endl;
    }
    else if(effect == "extra")
    {
       _extra_turn = true;
    }
    else if(effect == "advisor")
    {
        if(player.getAdvisor().getName() == "none")
        {
            player = advisorSelect(player,_turn);
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
                player = advisorSelect(player,_turn);
            }
        }
    }
    else if(effect == "riddle")
    {
        string riddle = getRandomRiddle();
        string riddle_arr[2];
        split(riddle, '|',riddle_arr, 2);

        cout << riddle_arr[0] << endl;
        string answer;
        cin >> answer;
        if(answer == riddle_arr[1])
        {
            cout << "You answered correct!" << endl;
            player.addWisdom(500);
        }
        else
        {
            cout << "Wrong lmao." << endl;
        }
    }
    return player;
}

bool Game::isFinished()
{
    return _board.areAllPlayersFinished();
}

/*
Loops through the player vector and converts all leadership points to pride points and finds the player with most 
pride points. returns the player with the most.

*/

Player Game::calculateWinner()
{
    return Player();
}
/*
Gets winner from calculateWinner method and displays the winner's stats followed by the other player's stats
*/
void Game::displayWinner()
{
    Player winner = calculateWinner();
}