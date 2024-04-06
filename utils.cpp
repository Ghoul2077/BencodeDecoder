#include "utils.h"

bool isInteger(const char& ch) {
    int asciiVal = (int)ch;
    if ((asciiVal >= ZERO_ASCII_VAL) && (asciiVal <= NINE_ASCII_VAL)) {
        return true;
    }
    return false;
}

bool stringToInt(const string& num, bigInt& out) {
    bigInt len = num.size();
    if (len == 0) {
        return false;
    }

    bool isNegative = false;
    if (num[0] == '-') {
        isNegative = true;
    }

    for (int i = (isNegative ? 1 : 0); i < len; i++) {
        if (!isInteger(num[i])) {
            return false;
        }
    }
    out = 0;
    for (int i = (isNegative ? 1 : 0); i < len; i++) {
        int asciiVal = (int)num[i];
        int res = asciiVal - ZERO_ASCII_VAL;
        // to handle integer overflow
        if (out * 10 < out) {
            return false;
        }
        out *= 10;
        // to handle integer overflow
        if (out + res < out) {
            return false;
        }
        out += res;
    }

    if (isNegative) {
        out *= -1;
    }

    return true;
}

bool intToString(bigInt num, string& out) {
    bool isNegative = false;
    if (num < 0) {
        isNegative = true;
        num *= -1;
    }
    while (num > 0) {
        int digit = num % 10;
        out = (char)(digit + ZERO_ASCII_VAL) + out;
        num /= 10;
    }
    if (isNegative) {
        out += "-";
    }
    return true;
}

bool arrayToString(const vector<string>& input, string& out) {
    int size = input.size();
    out = "[ ";
    for (int i = 0; i < size; i++) {
        out += (input[i] + ((i != (size - 1)) ? ", " : ""));
    }
    out += " ]";
    return true;
}

bool unorderedMapToString(const unordered_map<string, string>& input,
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

bool isAsciiEncoded(const string& str) {
    for (int i = 0; i < str.size(); i++) {
        int asciiVal = (unsigned char)str[i];
        if (asciiVal < ASCII_START || asciiVal > ASCII_END) {
            return false;
        }
    }
    return true;
}

void printStringAsHex(const string& str, const string& delimiter) {
    cout.setf(std::ios_base::hex, std::ios::basefield);
    for (int i = 0; i < str.size(); i++) {
        unsigned char ch = str[i];
        cout << int(ch) << delimiter;
    }
    cout.unsetf(std::ios_base::hex | std::ios::basefield);
}
