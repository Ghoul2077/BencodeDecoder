#include "encoder.h"

bool getElementAtIndex(
    unordered_map<int, string>& stringOutput,
    unordered_map<int, bigInt>& integerOutput,
    unordered_map<int, vector<string>>& listOutput,
    unordered_map<int, unordered_map<string, string>>& dictionaryOutput,
    const int& index, string& res) {
    if (stringOutput.count(index) > 0) {
        res = stringOutput[index];
        return true;
    } else if (integerOutput.count(index) > 0) {
        if (intToString(integerOutput[index], res)) {
            return true;
        }
    } else if (listOutput.count(index) > 0) {
        if (arrayToString(listOutput[index], res)) {
            return true;
        }
    } else if (dictionaryOutput.count(index) > 0) {
        if (unorderedMapToString(dictionaryOutput[index], res)) {
            return true;
        }
    }

    return false;
}

int getTokenType(const char& startingChar) {
    switch (startingChar) {
        case 'i':
            return B_INTEGER;
            break;
        case 'l':
            return B_LIST;
            break;
        case 'd':
            return B_DICTIONARY;
            break;
    }
    if (isInteger(startingChar)) {
        return B_STRING;
    }
    return B_NONE;
}

pair<bEncodeErrorTypes, int> bDecode(
    string input, int start, int stop, int& totalElementcount,
    unordered_map<int, string>& stringOutput,
    unordered_map<int, bigInt>& integerOutput,
    unordered_map<int, vector<string>>& listOutput,
    unordered_map<int, unordered_map<string, string>>& dictionaryOutput,
    const int& steps) {
    if (input.size() < (stop - start)) {
        return {B_INPUT_ARGS_ERROR, 0};
    }

    totalElementcount = 0;
    stack<char> st;

    for (int i = start; i < stop; i++) {
        int tokenType = getTokenType(input[i]);

        switch (tokenType) {
            case B_INTEGER: {
                string tempInteger = "";
                do {
                    i++;
                    tempInteger += input[i];
                } while ((i < stop) && (input[i + 1] != 'e'));

                bigInt res;
                if (stringToInt(tempInteger, res)) {
                    integerOutput[totalElementcount++] = res;
                    i++;
                } else {
                    return {B_INTEGER_ERROR, i};
                }
                break;
            }
            case B_STRING: {
                string _stringSize = "";
                while ((i < stop) && (input[i] != ':')) {
                    if (isInteger(input[i])) {
                        _stringSize += input[i];
                    } else {
                        return {B_STRING_ERROR, i};
                    }
                    i++;
                }

                bigInt stringSize;
                if (stringToInt(_stringSize, stringSize)) {
                    i++;
                    string res = "";
                    int end = i + stringSize;
                    for (; i < end - 1; i++) {
                        if (i >= stop) {
                            return {B_STRING_ERROR, i};
                        }
                        res += input[i];
                    }
                    if (i < stop) {
                        res += input[i];
                    }
                    stringOutput[totalElementcount++] = res;
                } else {
                    return {B_STRING_ERROR, i};
                }
                break;
            }
            case B_LIST: {
                i++;
                int listElementCount = 0;
                unordered_map<int, string> _listStringOutput;
                unordered_map<int, bigInt> _listIntegerOutput;
                unordered_map<int, vector<string>> _listOutput;
                unordered_map<int, unordered_map<string, string>>
                    _dictionaryOutput;

                pair<bEncodeErrorTypes, int> ret =
                    bDecode(input, i, stop, listElementCount, _listStringOutput,
                            _listIntegerOutput, _listOutput, _dictionaryOutput);

                if ((ret.first == UNIDENTIFIED_ERROR) &&
                    (input[ret.second] == 'e')) {
                    for (int j = 0; j < listElementCount; j++) {
                        if (_listStringOutput.count(j) > 0) {
                            listOutput[totalElementcount].push_back(
                                _listStringOutput[j]);
                        } else if (_listIntegerOutput.count(j) > 0) {
                            string num = "";
                            if (intToString(_listIntegerOutput[j], num)) {
                                listOutput[totalElementcount].push_back(num);
                            }
                        } else if (_listOutput.count(j) > 0) {
                            string lst;
                            if (arrayToString(_listOutput[j], lst)) {
                                listOutput[totalElementcount].push_back(lst);
                            }
                        } else if (_dictionaryOutput.count(j) > 0) {
                            string dict;
                            if (unorderedMapToString(_dictionaryOutput[j],
                                                     dict)) {
                                listOutput[totalElementcount].push_back(dict);
                            }
                        }
                    }

                    i = ret.second;
                    totalElementcount++;
                } else {
                    return {B_LIST_ERROR, ret.second};
                }
                break;
            }
            case B_DICTIONARY: {
                i++;
                int dictionaryElementCount = 0;
                pair<bEncodeErrorTypes, int> ret;
                unordered_map<int, string> _dictionaryStringOutput;
                unordered_map<int, bigInt> _dictionaryIntegerOutput;
                unordered_map<int, vector<string>> _listOutput;
                unordered_map<int, unordered_map<string, string>>
                    _dictionaryOutput;

                do {
                    _dictionaryStringOutput.clear();
                    _dictionaryIntegerOutput.clear();
                    _listOutput.clear();
                    _dictionaryOutput.clear();

                    ret = bDecode(input, i, stop, dictionaryElementCount,
                                  _dictionaryStringOutput,
                                  _dictionaryIntegerOutput, _listOutput,
                                  _dictionaryOutput, 1);

                    if (ret.first != B_SUCCESS) {
                        return {B_DICTIONARY_ERROR, ret.second};
                    }

                    string elem1;
                    if (getElementAtIndex(_dictionaryStringOutput,
                                          _dictionaryIntegerOutput, _listOutput,
                                          _dictionaryOutput, 0, elem1)) {
                    } else {
                        return {B_DICTIONARY_ERROR, ret.second};
                    }

                    i = ret.second + 1;

                    _dictionaryStringOutput.clear();
                    _dictionaryIntegerOutput.clear();
                    _listOutput.clear();
                    _dictionaryOutput.clear();

                    ret = bDecode(input, i, stop, dictionaryElementCount,
                                  _dictionaryStringOutput,
                                  _dictionaryIntegerOutput, _listOutput,
                                  _dictionaryOutput, 1);

                    if (ret.first != B_SUCCESS) {
                        return {B_DICTIONARY_ERROR, ret.second};
                    }

                    string elem2;
                    if (getElementAtIndex(_dictionaryStringOutput,
                                          _dictionaryIntegerOutput, _listOutput,
                                          _dictionaryOutput, 0, elem2)) {
                        dictionaryOutput[totalElementcount][elem1] =
                            (elem1 == "pieces") ? stringToHex(elem2) : elem2;
                    } else {
                        return {B_DICTIONARY_ERROR, ret.second};
                    }

                    i = ret.second + 1;
                } while ((i < stop) && (input[i]) != 'e');

                if (input[i] == 'e') {
                    totalElementcount++;
                } else {
                    return {B_DICTIONARY_ERROR, ret.second};
                }

                break;
            }
            case B_NONE: {
                return {UNIDENTIFIED_ERROR, i};
            }
        }

        if (totalElementcount == steps) {
            return {B_SUCCESS, i};
        }
    }

    return {B_SUCCESS, stop};
}

int main(int argc, char const* argv[]) {
    string input = "";
    ifstream inputFile("input.torrent", ifstream::in | ifstream::binary);
    for (string line; getline(inputFile, line);) {
        input += line;
    }
    inputFile.close();

    int totalElements = 0;
    unordered_map<int, string> stringOutput;
    unordered_map<int, bigInt> integerOutput;
    unordered_map<int, vector<string>> listOutput;
    unordered_map<int, unordered_map<string, string>> dictionaryOutput;

    pair<bEncodeErrorTypes, int> ret =
        bDecode(input, 0, input.size(), totalElements, stringOutput,
                integerOutput, listOutput, dictionaryOutput);

    cout << "Count of total elements : " << totalElements << endl;
    for (int i = 0; i < totalElements; i++) {
        if (stringOutput.count(i) > 0) {
            cout << stringOutput[i];
        } else if (integerOutput.count(i) > 0) {
            cout << integerOutput[i];
        } else if (listOutput.count(i) > 0) {
            string lst;
            if (arrayToString(listOutput[i], lst)) {
                cout << lst;
            }
        } else if (dictionaryOutput.count(i) > 0) {
            string dict;
            if (unorderedMapToString(dictionaryOutput[i], dict)) {
                cout << dict;
            }
        }
        if (i != (totalElements - 1)) {
            cout << ", ";
        }
    }

    if (ret.first != B_SUCCESS) {
        cout << ret.first << endl;
        cout << "Error occured at index : " << ret.second << endl;
    }

    return 0;
}
