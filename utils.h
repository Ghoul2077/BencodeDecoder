#include <bits/stdc++.h>
using namespace std;

#define THIS_BLOCK __FUNCTION__
#define ZERO_ASCII_VAL 48
#define NINE_ASCII_VAL 57

bool isInteger(const char ch);
bool stringToInt(const string num, int& out);
bool intToString(const int num, string& out);
bool arrayToString(const vector<string> input, string& out);
bool unorderedMapToString(const unordered_map<string, string> input,
                          string& out);
