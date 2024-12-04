#include "Game.h"
#include <fstream>
#include <cassert>

vector<string> getOptionNames()
{
    ifstream theme_file("../files/theme_choices.txt");
    
    if(!theme_file.is_open())
    {
        cout << "theme_choices.txt" << " failed to open." << endl;
        cout << "add it inside the files folder " << endl;
    }
    assert(theme_file.is_open());

    vector<string> option_names;

    string line;
    while(getline(theme_file,line))
    {
        // Dispaly Name|Folder Name
        string arr[2];
        split(line,'|', arr, 2);
        option_names.push_back(arr[0]);
    }
    theme_file.close();

    return option_names;
}

vector<string> getOptionFolders()
{
    ifstream theme_file("../files/theme_choices.txt");
    
    if(!theme_file.is_open())
    {
        cout << "theme_choices.txt" << " failed to open." << endl;
        cout << "add it inside the files folder " << endl;
    }
    assert(theme_file.is_open());

    vector<string> options;

    string line;
    while(getline(theme_file,line))
    {
        // Dispaly Name|Folder Name
        string arr[2];
        split(line,'|', arr, 2);
        options.push_back(arr[1]);
    }
    theme_file.close();

    return options;
}
void displayOptions(vector<string> options_names)
{
    cout << "Theme options: " << endl;
    for(int i = 0; i<options_names.size(); i++)
    {
        cout << (i+1) << ". " << options_names[i] << " (" << (i+1) << ")" << endl;
    }
}

int main()
{
    srand(time(0));
    cout << " hello" << endl;
    
    vector<string> option_names = getOptionNames();
    vector<string> options = getOptionFolders();

    displayOptions(option_names);
    cout << "Choose an option using the corresponding number: " << endl;
    int choice;
    cin >> choice;
    while(choice < 1 || choice > option_names.size())
    {
        cout << "Invalid choice. Select a number between 1 and " << option_names.size() << endl;
        cout << endl;
        displayOptions(option_names);

        cout << "Choose an option using the corresponding number: " << endl;
        cin >> choice;
    }
    cout << endl;
    cout << option_names[choice - 1] << " theme chose!" << endl << endl;


    Game game(options[choice-1]);

    while(!game.isFinished())
    {
        game.takeTurn();
    }
    game.displayWinner();

    //add colors in file and don't specify change of points in description in tile types. 
    // Also add another element in tile types that is optional. Can be used to specify how far to move back
    return 0;
}