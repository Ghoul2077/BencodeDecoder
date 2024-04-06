#include <bits/stdc++.h>
using namespace std;

// #define DEBUG
#undef DEBUG

#define MULTILINE_STRING(x) #x
#define deb(x) cout << #x << " = " << x << endl;
#define deb2(x, y) cout << #x << " = " << x << ", " << #y << " = " << y << endl;
#define THIS_BLOCK __FUNCTION__
#define DBG_LOG(x) \
    cout << __FUNCTION__ << "() : " << __LINE__ << " : " << x << endl;

#define ASCII_START 0
#define ASCII_END 127
#define ZERO_ASCII_VAL 48
#define NINE_ASCII_VAL 57
#define bigInt long long int
#define bigUInt unsigned long long int

bool isInteger(const char& ch);
bool isAsciiEncoded(const string& str);
bool stringToInt(const string& num, bigInt& out);
bool intToString(bigInt num, string& out);
bool arrayToString(const vector<string>& input, string& out);
bool unorderedMapToString(const unordered_map<string, string>& input,
                          string& out);
string stringToHex(const string& input);
void printStringAsHex(const string& str, const string& delimiter = " ");
