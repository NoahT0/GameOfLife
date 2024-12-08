#include "Board.h"
#include <iomanip>
#include <fstream>
#include <cassert>

#define RED "\033[48;2;230;10;10m"
#define GREEN "\033[48;2;34;139;34m" /* Grassy Green (34,139,34) */
#define BLUE "\033[48;2;10;10;230m"
#define PINK "\033[48;2;255;105;180m"
#define BROWN "\033[48;2;139;69;19m"
#define PURPLE "\033[48;2;128;0;128m"
#define ORANGE "\033[48;2;230;115;0m" /* Orange (230,115,0) */
#define GREY "\033[48;2;128;128;128m" /* Grey (128,128,128) */
#define RESET "\033[0m"


void Board::initializeBoard()
{

    ifstream path_file = iteratePastDescription("paths.txt");

    string line = " ";
    
    string name;
    vector<string> start_stats;
    bool start_advisor;

    bool set_new_path = true;
    int num_of_stats = getStatNames().size();
    // Initialize new path then iterate until next path (when line is blank)
    while(getline(path_file,line))
    {
        
        if(set_new_path)
        {
            // Get and set name
            name = line;
            getline(path_file,line);

            start_stats = vectorSplit(line, '|');
            assert(start_stats.size() == num_of_stats+1);   // +1 for basic stats and additional main stat
            
            // Get and set advisor
            getline(path_file,line);
            assert(validateInt(line));
            start_advisor = stoi(line);

            // Get path size
            getline(path_file,line);
            getline(path_file,line);
            assert(validateInt(line));
            int path_size = stoi(line);

            _paths.push_back(Path(name, vectorStringToInt(start_stats), start_advisor, path_size));
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
    int max_width = 0;

    for(int i = 0; i< _paths.size(); i++)
    {
        int width = getWidthOfTile(i,pos);
        if(width>max_width)
        {
            max_width = width;
        }
    }

    return max_width;
}

void Board::displayTrack(int path)
{
    Path path_obj = _paths[path];
    for (int i = 0; i < path_obj.getSize(); i++)
    {
        vector<int> on_tile = playersOnTile(path, i);
        int width = getMaxWidthOfTile(i);
        _paths[path].displayTile(i,on_tile, width);

        // Manually break lines otherwise weird console output color
        if((i+1) % _MAX_TILES_PER_LINE == 0)
        {
            cout << endl;
        }
    }
    cout << endl;
}
void Board::displayBoard()
{
    
    for (int i = 0; i < _paths.size(); i++)
    {
        cout << _paths[i].getName() << ":" << endl;
        displayTrack(i);
        cout << endl; // Add line between lanes
    }
}
void Board::setPlayerPosition(int player_index, int pos)
{
    int path_size = getPathAtPlayer(player_index).getSize();
    pos = clamp(pos, 0, path_size-1);
    _players[player_index].setPosition(pos);
}
bool Board::movePlayer(int player_index)
{
    int path_size = getPathAtPlayer(player_index).getSize();
    // Increment player position
    if(_players[player_index].getPosition() < path_size - 1)
    {
        _players[player_index].move();
    }
    if (_players[player_index].getPosition() == path_size - 1)
    {
        // Player reached last tile
        return true;
    }
    return false;
}
bool Board::movePlayer(int player_index, int amount)
{
    int path_size = getPathAtPlayer(player_index).getSize();
    if(amount < 0)
    {
        amount = 0;
    }
    // Move certain forward amount of times or until player reaches the end
    for(int i = 0; i<amount && !movePlayer(player_index); i++);

    return (_players[player_index].getPosition() == path_size - 1);
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
    Path path = getPathAtPlayer(index);
    return path.getTileAtPos(player.getPosition());
}

bool Board::isPlayerFinshed(int index)
{
    int path_size = getPathAtPlayer(index).getSize();
    return (_players[index].getPosition() == path_size-1);
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

Path Board::getPathAtPlayer(int player_index)
{
    bool inRange = (player_index >= 0 && player_index < _players.size());

    assert(inRange);    // Make sure valid index is passed in
    Player player = _players[player_index];
    return _paths[player.getPath()];
}

vector<string> Board::getPathDescriptions()
{
    vector<string> path_names(_paths.size());
    for(int i = 0; i < path_names.size(); i++)
    {
        path_names[i] = _paths[i].getDescriptionDisplay();
    }

    return path_names;
}

vector<int> Board::getPathSizes()
{
    vector<int> path_sizes;
    for(int i = 0; i < _paths.size(); i++)
    {
        path_sizes.push_back(_paths[i].getSize());
    }

    return path_sizes;
}
