#include "Game.h"
#include <fstream>
#include <cassert>

Game::Game(string theme)
{
    // Write chosen theme to file (Used for path file paths)
    ofstream theme_choice("../files/theme_choice.txt");
    theme_choice << theme << endl;
    theme_choice.close();

    // Have players select a character
    vector<Player> players = characterSelect();

    // Initialize board and have players select a path
    _board = Board(players);
    vector<string> path_descriptions = _board.getPathDescriptions();
    vector<int> path_sizes = _board.getPathSizes();
    players = pathSelect(players, path_descriptions, path_sizes);
    players = initializePlayersOnPath(players);
    
    _board.setPlayers(players);
    _board.displayBoard();
    _turn = 0;

    _extra_turn = false;
}

Board Game::getBoard()
{
    return _board;
}
int Game::getTurn()
{
    return _turn;
}

void Game::setBoard(Board board)
{
    _board = board;
}
void Game::setTurn(int turn)
{
    if(turn >= 0)
    {
        _turn = turn;
    }
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

        // Get input for player selection
        cout << "Player " << count << " enter character name or 'X' to move on:" << endl;
        getline(cin,name);
        name = toUpperString(name);

        if(name != "X")
        {
            // Search for chosen character. If invalid loop again
            int characterIndex = findCharacterByName(possible_players, name);

            if(characterIndex<0)
            {
                cout << "Invalid character name. Try again." << "\n\n\n";
            }
            else
            {
                possible_players[characterIndex].setNum(count);
                // Add selected character to vector of players and update remaining characters that can be chosen
                players.push_back(possible_players[characterIndex]);
                cout << endl;
                cout << "Player " << count << " selected " << possible_players[characterIndex].getName() << "!\n\n\n";
                count ++;

                // Remove chosen character from vector
                possible_players.erase(possible_players.begin()+characterIndex);
            }
        }
        else if(players.size() == 0)
        {
            // Can't play the game if no players were selected
            cout << "Must have at least one player." << "\n\n\n";
            name = "";
        }
    }
    cout << endl;
    return players;
}

// Returns index of player that has particular name
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
        cout << "------------------------" << endl;   // Extra line for spacing
    }
}
vector<Player> Game::getPossiblePlayers()
{
    string line;
    vector<Player> possible_players;

    ifstream character_input = getInputStream("characters.txt");

    // Increment past description
    getline(character_input, line);

    while(getline(character_input, line))
    {
        // playerName|age|stat1|stat2|stat3|...|main stat
        vector<string> traits = vectorSplit(line, '|');

        // Get the stats of character ignoring name, age and the main stat
        vector<int> stats(traits.size()-3);

        for(int i = 0; i<stats.size(); i++)
        {
            int stat = stoi(traits[i + 2]); // +2 since the first element in traits is name and the second is age
            stats[i] = stat;
        }

        // Initialize player with name and stats. Then set the age and main stat
        Player player(traits[0],stats);

        assert(validateInt(traits[1])); // Ensure age is an int
        assert(validateInt(traits[traits.size()-1]));   // Ensure main stat is an int

        player.setAge(stoi(traits[1]));
        player.setMainStat(stoi(traits[traits.size()-1]));

        // Add new player to a vector
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
        // Add path's starting stats and main stats 
        players[i].addMainStat(path.getStartMainStat());
        vector<int> stats = path.getStartStats();
        players[i].addStats(stats);

        // If path starts with advisor then select advisor
        if(path.getStartAdvisor())
        {
            players[i] = advisorSelect(players[i]);
        }
        else
        {
            players[i].setAdvisor(Advisor());
        }
        
    }
    return players;
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
        
        // Make sure player actually inputs an int and it is in range of the menu choices
        while(!validateInt(choice) || stoi(choice) > 5 || stoi(choice) < 1)
        {
            cout << "Invalid choice try again." << endl;
            cout << endl;
            displayMenu();
            cin >> choice;
        }
        cin.ignore();
        doPlayerAction(stoi(choice));

        // If player hasn't actually moved reset the menu
        if(choice != "5")
        {
            choice = "";
        }
        
    }
    
    updateTurn();
}
void Game::updateTurn()
{
    // If all players are finished then don't update turn
    if(_board.areAllPlayersFinished())
    {
        return;
    }

    _turn ++;
    _turn %= getNumPlayers();

    // Iterate until player that isn't finish is found
    while(_board.isPlayerFinshed(_turn))
    {
        _turn ++;
        _turn %= getNumPlayers();
    }
    
}
void Game::doPlayerAction(int choice)
{
    Player player = _board.getPlayerAtIndex(_turn);
    
    switch(choice)
    {
        case 1:
            player.displayProgress();
            break;
        case 2:
            player.displayCharacter();
            break;
        case 3:
            _board.displayBoard();
            break;
        case 4:
            player.displayAdvisor();
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

    // Generate random numer 1-6 for roll
    cout << "Spinning..." << endl;
    int roll = rand() % 6 + 1;
    cout << player.getPlayerTitle() << " rolled a " << roll << "." << endl;
    cout << endl;

    // Move player and display updated board
    _board.movePlayer(_turn,roll);
    _board.displayBoard();

    // Apply the tile's effect to player
    player = _board.getPlayerAtIndex(_turn);
    player = applyTileEffect(roll);

    _board.setPlayerAtIndex(_turn, player); 

    // If player gets extra turn from tile effect subtract 1 from turn to offset turn being incremented by 1
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
    cout << player.getPlayerTitle() << " please choose an option using the corresponding number:" << endl;
}
int Game::getNumPlayers()
{
    return _board.getPlayers().size();
}
// Change player stats based off what tile they landed on and also apply special effect to player
Player Game::applyTileEffect(int roll)
{
    // Get current player and their path and tile they are on
    Player player = _board.getPlayerAtIndex(_turn);
    Tile tile = _board.getTileAtPlayer(_turn);
    Path player_path = _board.getPaths()[player.getPath()];

    // Print out description of tile
    cout << tile.getDescription() << endl;

    // Update player stats based off tile
    player = tile.changePlayerStats(player);

    string effect = tile.getAdditionalEffect();

    // Call corresponding function based off effect
    if(effect == "random")
    {
        player = tile.doRandom(player, player_path.getEvents());
    }
    else if(effect == "back")
    {
        player = tile.moveBack(player);
    }
    else if(effect == "previous")
    {
        player = tile.moveToPrevious(player, roll);
    }
    else if(effect == "extra")
    {
        _extra_turn = true;
        player = tile.extraTurn(player);
    }
    else if(effect == "advisor")
    {
        player = tile.switchAdvisor(player);
    }
    else if(effect == "switch")
    {
        string path_name = _board.getPathAtPlayer(player.getNum()-1).getName();
        vector<string> board_descriptions = _board.getPathDescriptions();
        vector<int> path_sizes = _board.getPathSizes();

        player = tile.switchPath(player, path_name, board_descriptions, path_sizes);


        // Check if player is now at an end tile for when player switches from longer path to shorter
        player_path = _board.getPaths()[player.getPath()];
        tile = player_path.getTiles()[player.getPosition()];
        effect = tile.getAdditionalEffect();
        
        if(effect == "end")
        {
            cout << player.getPlayerTitle() << " finished!" << endl;
            cout << "Final Stats: " << endl;
            player.printFinalStats();
            cout << endl;
        }

    }
    else if(effect == "riddle")
    {
        player = tile.doRiddle(player);
    }
    else if(effect == "end")
    {
        cout << player.getPlayerTitle() << " finished!" << endl;
        cout << "Final Stats: " << endl;
        player.printFinalStats();
        cout << endl;
    }
    else
    {
        cout << tile.getStatWinsAndLoss() << endl;
    }

    return player;
}

bool Game::isFinished()
{
    return _board.areAllPlayersFinished();
}
// Ex:
// {6,2,9,8,7} {6,9,2,8,7} {6,9,8,2,7} {6,9,8,7,2}
// {6,9,8,7,2} {9,6,8,7,2} {9,8,6,7,2} {9,8,7,6,2} 
vector<Player> Game::bubbleSortPlayers()
{
    vector<Player> players = _board.getPlayers();
    
    int size = players.size();
    bool isSwapped = false;

    for(int i = 0; i < size - 1; i++)
    {
        isSwapped = false;

        for(int j = 0; j < size - i - 1; j++)
        {
            int current = players[j].getConvertedMainStat();
            int next = players[j + 1].getConvertedMainStat();

            // If next is greater than current swap so greatest is at beginning
            if(current < next)
            {
                swap(players[j], players[j+1]);
                isSwapped = true;
            }
        }

        if(!isSwapped)
        {
            break;
        }
    }
    return players;
}

// Gets the sorted players and then displays them in order
void Game::displayWinner()
{
    vector<Player> players = bubbleSortPlayers();
    cout << "Winner: " << players[0].getName() << endl;
    for(int i = 0; i < players.size(); i++)
    {
        cout << "Place: " << (i+1) << endl;
        players[i].printFinalStats();
        cout << "-----------------------" << endl;
    }
}