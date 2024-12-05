#include "Helper.h"
#include <fstream>
#include <cassert>

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
int clamp(int num, int lower, int upper)
{
    if(lower > upper)
    {
        int temp = lower;
        lower = upper;
        upper = temp;
    }
    if(num<lower)
    {
        num = lower;
    }

    if(num>upper)
    {
        num = upper;
    }

    return num;
}

int getCharIndex(string input_string, char separator)
{
    for(int i = 0; i < (int)input_string.length(); i++)
    {
        if(input_string[i] == separator)
        {
            return i;
        }
    }
    return -1;   
}

int split(string input_string, char separator, string arr[], const int ARR_SIZE)
{
    if(input_string.length() == 0)
    {
        
        return 0;
    }
    int charIdx = getCharIndex(input_string, separator);
    if(charIdx < 0)
    {
        arr[0] = input_string;
        return 1;
    }
    int arrIndex = 0;
    while(charIdx >= 0 && arrIndex < ARR_SIZE)
    {
        arr[arrIndex] = input_string.substr(0,charIdx);
        input_string = input_string.substr(charIdx + 1);
        arrIndex ++;
        charIdx = getCharIndex(input_string,separator);

        
    }
    if(arrIndex < ARR_SIZE)
    {
        arr[arrIndex] = input_string;
        arrIndex ++;
        return arrIndex;
    }
    
    return -1;
    
}

string toUpperString(string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        str[i] = toupper(str[i]);
    }
    return str;
}

vector<string> getStatNames()
{
    ifstream stat_file = getInputStream("stat_settings.txt");
    
    string line;
    getline(stat_file, line);
    stat_file.close();

    vector<string> stat_names = vectorSplit(line, '|');
    stat_names.pop_back();  // Get rid of main stat

    return stat_names;
}

vector<string> vectorSplit(string input_string, char separator)
{
    vector <string> result;

    if(input_string.length() == 0)
    {
        cout << "input string has 0 length" << endl;
        return result;
    }


    int charIdx = getCharIndex(input_string, separator);
    if(charIdx < 0)
    {
        result.push_back(input_string);
        return result;
    }
    while(charIdx >= 0)
    {
        result.push_back(input_string.substr(0,charIdx));
        input_string = input_string.substr(charIdx + 1);
        charIdx = getCharIndex(input_string,separator);

    }
    result.push_back(input_string);
    
    return result;
}

string getMainStatName()
{
    ifstream stat_file = getInputStream("stat_settings.txt");
    
    string line;
    getline(stat_file, line);
    stat_file.close();

    vector<string> stats = vectorSplit(line, '|');

    return stats[stats.size()-1];
}

int getIndexOfStatByName(string name)
{
    vector<string> stat_names = getStatNames();

    for(int i = 0; i<stat_names.size(); i++)
    {
        if(toUpperString(stat_names[i]) == toUpperString(name))
        {
            return i;
        }
    }

    return -1;
}


ifstream getInputStream(string file_name)
{
    ifstream theme_file("../files/theme_choice.txt");
    if(!theme_file.is_open())
    {
        cout << "theme_choice.txt" << " failed to open." << endl;
        cout << "add it inside the files folder " << endl;
    }
    assert(theme_file.is_open());
    string theme;
    getline(theme_file,theme);
    theme_file.close();


    ifstream input_file("../files/" + theme + "/" + file_name);
    
    if(!input_file.is_open())
    {
        cout << file_name << " failed to open." << endl;
    }
    assert(input_file.is_open());

    return input_file;
}

void stringToIntArray(string arr1[], int arr2[], int size)
{
    for(int i = 0; i <size; i++)
    {
        arr2[i] = stoi(arr1[i]);
    }
}
//int split(string input_string, char separator, string arr[], const int ARR_SIZE);

ifstream iterateToStringInStream(string file_name, vector<string> strings)
{
    int size = strings.size();
    ifstream input_file = getInputStream(file_name);

    if(size == 0)
    {
        return input_file;
    }

    string line;
    while(getline(input_file, line) && line != strings[0])
    {
        // Iterate until first string is found
    }

    if(line != strings[0])
    {
        cout << strings[0] << " not found in file " << file_name << endl;
    }
    assert(line == strings[0]);

    int index = 1;
    bool is_found = true;
    while(index < size && is_found)
    {
        // Iterate to next string in strings vector
        is_found = false;
        while(!is_found && getline(input_file, line) && line.length() > 0)
        {
            if(line == strings[index])
            {
                is_found = true;
                index ++;
            }
        }

    }

    if(!is_found)
    {
        cout << strings[index] << " not found after " << strings[index-1] << " and before a blank line in file " << file_name << endl;
    }
    assert(is_found);
    return input_file;
}
// Assumes blank line between description and other lines
ifstream iteratePastDescription(string file_name)
{
    return iterateToStringInStream(file_name, {""});
}

vector<int> vectorStringToInt(vector<string> vec)
{
    vector<int> result(vec.size());
    for(int i = 0; i<result.size(); i++)
    {
        result[i] = stoi(vec[i]);
    }
    
    return result;
}