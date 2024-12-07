#include "Game.h"
#include <fstream>
#include <cassert>

Game::Game(string theme)
{
    // Set theme
    ofstream theme_choice("../files/theme_choice.txt");
    theme_choice << theme << endl;
    theme_choice.close();

    vector<Player> players = characterSelect();
    _board = Board(players);
    
    vector<Path> paths = _board.getPaths();
    vector<string> path_names(paths.size());
    for(int i = 0; i < path_names.size(); i++)
    {
        path_names[i] = paths[i].getDescriptionDisplay();
    }
    
    players = pathSelect(players, path_names);
    players = initializePlayersOnPath(players);
    
    _board.setPlayers(players);
    _board.displayBoard();
    _turn = 0;

    _extra_turn[0] = false;
    
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

    // Increment past description
    getline(character_input, line);

    while(getline(character_input, line))
    {
        // playerName|age|strength|stamina|wisdom|pridePoints
        //string arr[6];
       // split(line,'|',arr,6);
        vector<string> traits = vectorSplit(line, '|');

        // Get the stats of character(strength,stamina,wisdom)
        vector<int> stats(traits.size()-3);
        //cout << stats.size() << endl;
        for(int i = 0; i<stats.size(); i++)
        {
            //cout << traits[i+2] << endl;
            int stat = stoi(traits[i + 2]); // +2 since the first element in traits is name and the second is age
            stats[i] = stat;
        }

        Player player(traits[0],stats);
        player.setAge(stoi(traits[1]));
        player.setMainStat(stoi(traits[traits.size()-1]));
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
        players[i].addMainStat(path.getStartMainStat());
        vector<int> stats = path.getStartStats();
        players[i].addStats(stats);

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
        cin.ignore();
        doPlayerAction(stoi(choice));

        if(choice != "5")
        {
            choice = "";
        }
        
    }
    
    updateTurn();
}
void Game::updateTurn()
{
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
            player.displayProgress(_turn);
            break;
        case 2:
            player.displayCharacter(_turn);
            break;
        case 3:
            _board.displayBoard();
            break;
        case 4:
            player.displayAdvisor(_turn);
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
    cout << player.getPlayerTitle(_turn) << " rolled a " << roll << "." << endl;
    cout << endl;

    // Move player and display updated board
    _board.movePlayer(_turn,roll);
    _board.displayBoard();

    // Apply the tile's effect to player
    player = _board.getPlayerAtIndex(_turn);
    player = applyTileEffect(roll);

    _board.setPlayerAtIndex(_turn, player); 

    if(_extra_turn[0])
    {
        _extra_turn[0] = false;
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
// Change player stats based off what tile they landed on and also apply special effect to player
Player Game::applyTileEffect(int roll)
{
    Player player = _board.getPlayerAtIndex(_turn);
    Tile tile = _board.getTileAtPlayer(_turn);
    Path player_path = _board.getPaths()[player.getPath()];

    cout << tile.getDescription() << endl;
    player = tile.changePlayerStats(player);

    string effect = tile.getAdditionalEffect();

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
        _extra_turn[0] = true;
        player = tile.extraTurn(player);
    }
    else if(effect == "advisor")
    {
        player = tile.switchAdvisor(player, _turn);
    }
    else if(effect == "riddle")
    {
        player = tile.doRiddle(player);
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