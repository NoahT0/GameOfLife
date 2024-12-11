#ifndef HELPER_H
#define HELPER_H

#include <iostream>

using namespace std;
bool validateInt(string input);
int clamp(int num, int lower, int upper);
int getCharIndex(string input_string, char separator);
int split(string input_string, char separator, string arr[], const int ARR_SIZE);
string toUpperString(string str);
ifstream getInputStream(string file_name);
ifstream iterateToStringInStream(string file_name, vector<string> strings);
ifstream iteratePastDescription(string file_name);
vector<int> vectorStringToInt(vector<string> vec);
vector<string> vectorSplit(string input_string, char separator);
vector<string> getStatNames();
string getMainStatName();
int getIndexOfStatByName(string name);

#endif