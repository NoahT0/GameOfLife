#include "Board.h"
#include <iomanip>
#include <fstream>

#define RED "\033[48;2;230;10;10m"
#define GREEN "\033[48;2;34;139;34m" /* Grassy Green (34,139,34) */
#define BLUE "\033[48;2;10;10;230m"
#define PINK "\033[48;2;255;105;180m"
#define BROWN "\033[48;2;139;69;19m"
#define PURPLE "\033[48;2;128;0;128m"
#define ORANGE "\033[48;2;230;115;0m" /* Orange (230,115,0) */
#define GREY "\033[48;2;128;128;128m" /* Grey (128,128,128) */
#define RESET "\033[0m"

string toUpperString(string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        str[i] = toupper(str[i]);
    }
    return str;
}

void Board::initializeBoard()
{
    // Seed random number generator in your main function once

    //ifstream path_file("Files/paths.txt");
    ifstream path_file = getInputStream("paths.txt");

    string line = " ";

    // Iterate past description
    while(line.length() > 0)
    {
        getline(path_file,line);
    }

    
    string name;
    string leadership;
    bool start_advisor;

    bool set_new_path = true;
    // Initialize new path then iterate until next path (when line is blank)
    while(getline(path_file,line))
    {
        
        if(set_new_path)
        {
            name = line;
            getline(path_file,line);
            leadership = line;
            getline(path_file,line);
            start_advisor = stoi(line);

            _paths.push_back(Path(name, leadership, start_advisor, _BOARD_SIZE));
            set_new_path = false;
        }
        
        if(line.length() == 0)
        {
            set_new_path = true;
        }
    }
    path_file.close();
}

vector<Path> Board::getPaths()
{
    return _paths;
}

#include <cstdlib> // For rand() and srand()
#include <ctime> // For time()

Board::Board()
{
    /*
    _players.push_back(Player());
    // Initialize tiles
    initializeBoard();
    */
}
Board::Board(int player_count)
{
    if (player_count > _MAX_PLAYERS)
    {
        player_count = _MAX_PLAYERS;
    }
    
    // Initialize player position
    for (int i = 0; i < player_count; i++)
    {
        _players.push_back(Player());
    }
    // Initialize tiles
    initializeBoard();
}
Board::Board(vector <Player> players)
{
    for(int i = 0; i<_MAX_PLAYERS && i<players.size(); i++)
    {
        _players.push_back(players[i]);
    }
    //initializePlayersOnPath();
    // for(int i = 0; i<_players.size(); i++)
    // {
    //     cout << _players[i].getAdvisor().getName() << endl;
    // }

    initializeBoard();
    
}

bool Board::isPlayerOnTile(int player_index, int path, int pos)
{
    if (_players[player_index].getPath() == path)
    {
        if(_players[player_index].getPosition() == pos)
        {
            return true;
        }
    }
    return false;
}
vector<int> Board::playersOnTile(int path, int pos)
{
    vector<int> players;
    for(int i = 0; i<_players.size(); i++)
    {
        if(isPlayerOnTile(i,path,pos))
        {
            players.push_back(i);
            //cout << i << endl;
        }
    }
    return players;
}

int Board::getWidthOfTile(int path, int pos)
{
    vector<int> onTile = playersOnTile(path, pos);
    if(onTile.size()<2)
    {
        return 1;
    }
    return 1 + 2*(onTile.size()-1); // Takes up two more spaces for each additonal player on tile
}

// Gets the necessary width of a tile since sometimes a tile may have to be wider on one path since it is wider on another
int Board::getMaxWidthOfTile(int pos)
{
    int first = getWidthOfTile(0,pos);
    int second = getWidthOfTile(1,pos);

    if(first>second)
    {
        return first;
    }

    return second;
}

void Board::displayTrack(int path)
{
    for (int i = 0; i < _BOARD_SIZE; i++)
    {
        vector<int> on_tile = playersOnTile(path, i);
        int width = getMaxWidthOfTile(i);
        _paths[path].displayTile(i,on_tile, width);
    }
    cout << endl;
}
void Board::displayBoard()
{
    
    for (int i = 0; i < _paths.size(); i++)
    {
        displayTrack(i);
        if (i == 0) {
            cout << endl; // Add an extra line between the two lanes
        }
    }
}
void Board::setPlayerPosition(int player_index, int pos)
{
    pos = clamp(pos, 0, _BOARD_SIZE-1);
    _players[player_index].setPosition(pos);
}
bool Board::movePlayer(int player_index)
{
    // Increment player position
    if(_players[player_index].getPosition() < _BOARD_SIZE - 1)
    {
        _players[player_index].move();
    }
    if (_players[player_index].getPosition() == _BOARD_SIZE - 1)
    {
        // Player reached last tile
        return true;
    }
    return false;
}
bool Board::movePlayer(int player_index, int amount)
{
    if(amount < 0)
    {
        amount = 0;
    }
    // Move certain forward amount of times or until player reaches the end
    for(int i = 0; i<amount && !movePlayer(player_index); i++);

    return (_players[player_index].getPosition() == _BOARD_SIZE - 1);
}
int Board::getPlayerPosition(int player_index)
{
    if (player_index >= 0 && player_index < _players.size())
    {
        return _players[player_index].getPosition();
    }
    return -1;
}

vector<Player> Board::getPlayers()
{
    return _players;
}
Player Board::getPlayerAtIndex(int index)
{
    return _players[index];
}

void Board::setPlayers(vector<Player> players)
{
    _players = players;
}
void Board::setPlayerAtIndex(int index, Player player)
{
    _players[index] = player;
}
Tile Board::getTileAtPlayer(int index)
{
    Player player = _players[index];
    return _paths[player.getPath()].getTileAtPos(player.getPosition());
}

bool Board::isPlayerFinshed(int index)
{
    return (_players[index].getPosition() == _BOARD_SIZE-1);
}

bool Board::areAllPlayersFinished()
{
    for(int i = 0; i<_players.size(); i++)
    {
        if(!isPlayerFinshed(i))
        {
            return false;
        }
    }

    return true;
}

int Board::getBoardSize()
{
    return _BOARD_SIZE;
}