#include <bits/stdc++.h>

#include "utils.h"
using namespace std;

enum bEncodeErrorTypes {
    B_SUCCESS = 1,
    UNIDENTIFIED_ERROR = -1,
    B_INTEGER_ERROR = -2,
    B_STRING_ERROR = -3,
    B_LIST_ERROR = -4,
    B_DICTIONARY_ERROR = -5,
    B_INPUT_ARGS_ERROR = -6
};

enum bEncodeDataTypes { B_INTEGER, B_STRING, B_LIST, B_DICTIONARY, B_NONE };

pair<bEncodeErrorTypes, int> bDecode(
    string input, int start, int stop, int& totalElementcount,
    unordered_map<int, string>& stringOutput,
    unordered_map<int, bigInt>& integerOutput,
    unordered_map<int, vector<string>>& listOutput,
    unordered_map<int, unordered_map<string, string>>& dictionaryOutput,
    const int& steps = -1);

bool getElementAtIndex(
    unordered_map<int, string>& stringOutput,
    unordered_map<int, bigInt>& integerOutput,
    unordered_map<int, vector<string>>& listOutput,
    unordered_map<int, unordered_map<string, string>>& dictionaryOutput,
    const int& index, string& res);
