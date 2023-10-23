#include "utils.h"

bool isInteger(char ch) {
    int asciiVal = (int)ch;
    if ((asciiVal >= ZERO_ASCII_VAL) && (asciiVal <= NINE_ASCII_VAL)) {
        return true;
    }
    return false;
}

bool stringToInt(string num, int& out) {
    if (num.size() == 0) {
        return false;
    }
    int len = num.size();
    for (int i = 0; i < len; i++) {
        if (!isInteger(num[i])) {
            return false;
        }
    }
    out = 0;
    for (int i = 0; i < len; i++) {
        int asciiVal = (int)num[i];
        int res = asciiVal - ZERO_ASCII_VAL;
        out *= 10;
        out += res;
    }
    return true;
}

bool intToString(int num, string& out) {
    while (num > 0) {
        int digit = num % 10;
        out = (char)(digit + ZERO_ASCII_VAL) + out;
        num /= 10;
    }
    return true;
}

bool arrayToString(const vector<string> input, string& out) {
    int size = input.size();
    out = "[ ";
    for (int i = 0; i < size; i++) {
        out += (input[i] + ((i != (size - 1)) ? ", " : ""));
    }
    out += " ]";
    return true;
}

bool unorderedMapToString(const unordered_map<string, string> input,
                          string& out) {
    int size = input.size();
    int counter = 0;

    out = "{ ";
    for (auto i : input) {
        counter++;
        out += (i.first + " : " + i.second + ((counter != (size)) ? ", " : ""));
    }
    out += " }";
    return true;
}
