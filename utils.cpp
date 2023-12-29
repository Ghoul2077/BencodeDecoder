#include "utils.h"

bool isInteger(char ch) {
    int asciiVal = (int)ch;
    if ((asciiVal >= ZERO_ASCII_VAL) && (asciiVal <= NINE_ASCII_VAL)) {
        return true;
    }
    return false;
}

bool stringToInt(string num, bigInt& out) {
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

bool intToString(bigInt num, string& out) {
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
    out += "\n";
    for (auto i : input) {
        counter++;
        out += "\t";
        i.second = regex_replace(i.second, regex("\n"), "\n\t");
        out += ('"' + i.first + '"' + " : " + '"' + i.second + '"' +
                ((counter != (size)) ? ", " : ""));
        out += "\n";
    }
    out += "}";
    return true;
}

string stringToHex(const string& input) {
    string res = "";
    int n = input.size();
    for (int i = 0; i < n; i++) {
        ostringstream tmp;
        tmp << hex << setfill('0') << setw(2)
            << static_cast<unsigned int>(static_cast<unsigned char>(input[i]));
        res += tmp.str();
        if (i != (n - 1)) {
            res += " ";
        }
    }
    return res;
}
