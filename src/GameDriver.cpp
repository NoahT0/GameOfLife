#include "Game.h"
#include <fstream>
#include <cassert>

// Get the names of the different themes
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
// Get the folder names for different themes
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

// Print the theme options
void displayOptions(vector<string> options_names)
{
    cout << "Theme options: " << endl;
    for(int i = 0; i<options_names.size(); i++)
    {
        cout << (i+1) << ". " << options_names[i] << " (" << (i+1) << ")" << endl;
    }
}
string theme_select()
{
    vector<string> option_names = getOptionNames();
    vector<string> options = getOptionFolders();

    displayOptions(option_names);
    cout << "Choose an option using the corresponding number: " << endl;

    string choice_str;

    getline(cin, choice_str);

    // Make sure user inputs an integer that is in range of theme choices
    // If not then prompt user for a new number
    while(!validateInt(choice_str) || stoi(choice_str) < 1 || stoi(choice_str) > option_names.size())
    {
        cout << "Invalid choice. Select a number between 1 and " << option_names.size() << endl;
        cout << endl;
        displayOptions(option_names);

        cout << "Choose an option using the corresponding number: " << endl;
        getline(cin, choice_str);
    }
    cout << endl;
    // Print chosen theme name and return the file name
    int choice = stoi(choice_str);
    cout << option_names[choice - 1] << " theme chose!" << endl << endl;

    return options[choice-1];
}
int main()
{
    srand(time(0));
    
    string theme = theme_select();

    Game game(theme);

    while(!game.isFinished())
    {
        game.takeTurn();
    }
    game.displayWinner();

    return 0;
}