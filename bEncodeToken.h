#include <bits/stdc++.h>

#include "utils.h"
using namespace std;

#define B_TOKEN_END 'e'

enum bEncodeDataTypes { B_INTEGER, B_STRING, B_LIST, B_DICTIONARY, B_NONE };

struct BEncodeToken {
    bEncodeDataTypes type;

    // TODO: check if vairant data type can be implmented here
    bigInt num;
    string str;
    vector<BEncodeToken> list;
    // TODO: Add supporting map data structure for lexicographic ordering
    unordered_map<string, BEncodeToken> dict;

    BEncodeToken() { type = B_NONE; }
    BEncodeToken(const BEncodeToken& _);
    BEncodeToken& operator=(BEncodeToken const& _data);
    bool operator==(BEncodeToken const& _data) const;
    string toString() const;
    void print() const;
};
