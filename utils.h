#include <bits/stdc++.h>
using namespace std;

#define deb(x) cout << #x << " = " << x << endl;
#define deb2(x, y) cout << #x << " = " << x << ", " << #y << " = " << y << endl;

#define THIS_BLOCK __FUNCTION__
#define ZERO_ASCII_VAL 48
#define NINE_ASCII_VAL 57
#define bigInt long long int
#define bigUInt unsigned long long int

bool isInteger(const char ch);
bool stringToInt(const string num, bigInt& out);
bool intToString(const bigInt num, string& out);
bool arrayToString(const vector<string> input, string& out);
bool unorderedMapToString(const unordered_map<string, string> input,
                          string& out);
string stringToHex(const string& input);
