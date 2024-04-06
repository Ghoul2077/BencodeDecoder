#include "encoder.h"

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

void printError(bEncodeReturnTypes& ret) {
    if (ret == B_INTEGER_ERROR) {
        cout << "Integer Error" << endl;
    } else if (ret == B_STRING_SIZE_ERROR) {
        cout << "String size error" << endl;
    } else if (ret == B_STRING_ERROR) {
        cout << "String error" << endl;
    } else if (ret == B_LIST_ERROR) {
        cout << "List error" << endl;
    } else if (ret == B_DICTIONARY_ERROR) {
        cout << "Dictionary error" << endl;
    } else if (ret == B_INPUT_ARGS_ERROR) {
        cout << "Input arguments error" << endl;
    } else if (B_DELIMITERS_NOT_PROPER) {
        cout << "Input string is malformed";
    } else {
        cout << "Unidentified error";
    }
}

void Bdecode::print() {
    DBG_LOG(" : total token count = " << tokenCount);

    cout << endl << "==============================================" << endl;
    for (bigInt i = 1; i <= tokenCount; i++) {
        decodedData[i].print();
        if (i != (tokenCount)) {
            cout << ", ";
        }
    }
    cout << endl
         << "==============================================" << endl
         << endl;
}

/* Take bencoded string as input & parses the first integer from start point
    passed, it prints the parsed value in std output of terminal */
bigInt Bdecode::parseInt(const string& input, const bigInt& start, bigInt& out,
                         bEncodeReturnTypes& ret) {
    bigInt inputSize = input.size();
    bigInt end = inputSize;

    string intValInStr = "";
    bigInt iterator = start;
    do {
        intValInStr += input[iterator++];
    } while ((iterator < inputSize) && (input[iterator] != 'e'));

    bigInt res;
    if (((iterator != inputSize) && (input[iterator] == 'e')) &&
        (stringToInt(intValInStr, res))) {
        out = res;
        ret = B_SUCCESS;
        end = iterator + 1;
    } else {
        ret = B_INTEGER_ERROR;
    }

    return end;
}

/* Take bencoded string as input & parses the first integer from start point
    passed, it prints the parsed value in std output of terminal */
bigInt Bdecode::parseStringSize(const string& input, const bigInt& start,
                                bigInt& size, bEncodeReturnTypes& ret) {
    bigInt inputSize = input.size();
    bigInt end = inputSize;

    if (start > inputSize) {
        ret = B_STRING_SIZE_ERROR;
        return end;
    }

    bigInt iter = start;
    string _stringSize = "";

    while ((iter < inputSize) && (input[iter] != ':')) {
        if (isInteger(input[iter])) {
            _stringSize += input[iter];
            end = min(inputSize, iter + 2);
        } else {
            ret = B_STRING_SIZE_ERROR;
            end = inputSize;
            break;
        }
        iter++;
    }

    if ((iter != inputSize) && (!stringToInt(_stringSize, size))) {
        ret = B_STRING_SIZE_ERROR;
        end = inputSize;
    }

    return end;
}

bigInt Bdecode::parseString(const string& input, const bigInt& start,
                            const bigInt& stringSize, string& out,
                            bEncodeReturnTypes& ret) {
    ret = B_SUCCESS;
    bigInt inputSize = input.size();
    bigInt end = inputSize;

    if ((start + stringSize) > inputSize) {
        ret = B_STRING_ERROR;
        return end;
    }

    string res = "";
    bigInt iter = start;
    while ((iter < (start + stringSize)) && (iter < inputSize)) {
        res += input[iter];
        iter++;
    }

    end = min(iter, inputSize);
    out = res;
    return end;
}

vector<BEncodeToken> Bdecode::getVectorizedFormat() {
    vector<BEncodeToken> res;
    for (auto i : decodedData) {
        res.push_back(i.second);
    }
    return res;
}

unordered_map<string, BEncodeToken> Bdecode::getDictionaryFormat() {
    unordered_map<string, BEncodeToken> res;
    for (bigInt i = 1; i <= tokenCount; i += 2) {
        res.insert({decodedData[i].toString(), decodedData[i + 1]});
    }
    return res;
}

unordered_map<bigInt, bigInt> Bdecode::parseDelimiters(
    const string& input, const bigInt& start, const bigInt& end,
    bEncodeReturnTypes& res) {
    unordered_map<bigInt, bigInt> delimiters;
    res = B_SUCCESS;

    stack<bigInt> st;
    bigInt i = 0;
    while (i < end) {
        if (input[i] == B_TOKEN_END) {
            if (st.size()) {
                bigInt delimiterStart = st.top();
                st.pop();
                delimiters.insert({delimiterStart, i});
            } else {
                DBG_LOG("Delimiter not proper, index " << i);
                res = B_DELIMITERS_NOT_PROPER;
                break;
            }
            i++;
            continue;
        }

        switch (getTokenType(input[i])) {
            case B_LIST:
            case B_DICTIONARY: {
                st.push(i);
                break;
            }
            case B_INTEGER: {
                bigInt tokenStart = i;
                while ((i < end) && (input[i] != B_TOKEN_END)) {
                    i++;
                }
                if ((i < end) && (input[i] == B_TOKEN_END)) {
                    delimiters.insert({tokenStart, i});
                } else {
                    DBG_LOG("Delimiter not proper, index " << i);
                    res = B_DELIMITERS_NOT_PROPER;
                }
                break;
            }
            case B_STRING: {
                bigInt stringSize = 0;
                bigInt stringStart = parseStringSize(input, i, stringSize, res);
                if (res == B_SUCCESS) {
                    bigInt tokenStart = i;
                    i = min(stringStart + stringSize - 1, end);
                    if (i == end) {
                        res = B_DELIMITERS_NOT_PROPER;
                        break;
                    }
                    delimiters.insert({tokenStart, i});
                } else {
                    DBG_LOG(
                        "string parsing error while parsing delimiters, index "
                        << i);
                    res = B_DELIMITERS_NOT_PROPER;
                }
                break;
            }
            default:
                res = B_DELIMITERS_NOT_PROPER;
        }
        if (res == B_DELIMITERS_NOT_PROPER) {
            break;
        }
        i++;
    }

    if (st.size() > 0) {
        DBG_LOG("Pair not present for all starting delimiters, count = "
                << st.size());
        cout << "Indices with mismatch : ";
        while (st.size()) {
            cout << st.top() << " ";
            st.pop();
        }
        cout << endl;
        res = B_DELIMITERS_NOT_PROPER;
    }

#ifdef DEBUG
    DBG_LOG("Delimiter array : ");
    if (delimiters.size() > 0) {
        cout << "[";
        bigInt instance = 0;
        for (auto i : delimiters) {
            cout << "[" << i.first << ", " << i.second << "]";
            instance++;
            if (instance != delimiters.size()) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }
#endif

    return delimiters;
}

bigInt Bdecode::decode(const string& input, const bigInt& start,
                       const bigInt& end, bEncodeReturnTypes& res) {
    bigInt inputSize = end - start;

    if (inputSize == 0) {
        res = B_SUCCESS;
        return 0;
    }

    bigInt i = start;

    while (i < end) {
        switch (getTokenType(input[i])) {
            case B_INTEGER: {
                bigInt out;
                i = parseInt(input, i + 1, out, res);
                if (res == B_SUCCESS) {
                    DBG_LOG("Integer parsing success, " << out);
                    BEncodeToken token(out);
                    decodedData[++tokenCount] = token;
                } else {
                    DBG_LOG("Integer parsing failure, res : " << res);
                }
                break;
            }
            case B_STRING: {
                bigInt stringSize = 0;
                bigInt stringStart = parseStringSize(input, i, stringSize, res);
                if (res == B_SUCCESS) {
                    string out;
                    i = parseString(input, stringStart, stringSize, out, res);
                    if (res == B_SUCCESS) {
                        DBG_LOG("String parsing success, " << out);
                        BEncodeToken token(out);
                        decodedData[++tokenCount] = token;
                    }
                }
                break;
            }
            case B_LIST: {
                if (delimiterPairLocation.count(i) == 0) {
                    DBG_LOG("no ending delimiter found for list, index : "
                            << i + 1);
                    return B_LIST_ERROR;
                }
#ifdef DEBUG

                DBG_LOG("Decoding list from, index : "
                        << i << " to " << delimiterPairLocation[i]);
#endif

                try {
                    Bdecode subListDecoded(input, i + 1,
                                           delimiterPairLocation[i],
                                           delimiterPairLocation);
                    BEncodeToken token(subListDecoded.getVectorizedFormat());
                    decodedData[++tokenCount] = token;
#ifdef DEBUG
                    DBG_LOG("List parsing success, index : "
                            << i << " to " << delimiterPairLocation[i]);
                    subListDecoded.print();
#endif
                    i = delimiterPairLocation[i] + 1;
                } catch (bEncodeReturnTypes& err) {
                    DBG_LOG("Error while parsing list, index : " << i);
                    res = err;
                }
                break;
            }
            case B_DICTIONARY: {
                if (delimiterPairLocation.count(i) == 0) {
                    DBG_LOG("no ending delimiter found for dictionary, index : "
                            << i);
                    return B_DICTIONARY_ERROR;
                }
#ifdef DEBUG
                DBG_LOG("Decoding dictionary from, index : "
                        << i << " to " << delimiterPairLocation[i]);
#endif

                try {
                    Bdecode subDictDecoded(input, i + 1,
                                           delimiterPairLocation[i],
                                           delimiterPairLocation);
                    BEncodeToken token(subDictDecoded.getDictionaryFormat());
                    decodedData[++tokenCount] = token;
#ifdef DEBUG
                    DBG_LOG("Dictionary parsing success, index : "
                            << i << " to " << delimiterPairLocation[i]);
                    DBG_LOG(endl);
                    subDictDecoded.print();
#endif
                    i = delimiterPairLocation[i] + 1;
                } catch (bEncodeReturnTypes& err) {
                    DBG_LOG("Error while parsing dictionary, index : " << i);
                    res = err;
                }
                break;
            }
            default: {
#ifdef DEBUG
                DBG_LOG("Returning unidentified error, index : "
                        << i << ", value : " << input[i]);
                DBG_LOG("");
                print();
#endif
                res = B_UNIDENTIFIED_ERROR;
                return i;
            }
        }

        if (res != B_SUCCESS) {
            cout << __FUNCTION__ << "() : " << __LINE__ << " : "
                 << "Parsing error, index : " << i << endl;
            break;
        }
    }

    return i;
}

int main(int argc, char const* argv[]) {
    string input = "";
    ifstream inputFile("input4.torrent", ifstream::in | ifstream::binary);
    vector<unsigned char> buffer(istreambuf_iterator<char>(inputFile), {});
    for (auto i : buffer) {
        input += i;
    }
    inputFile.close();

    try {
        Bdecode decodedInstance(input);
        DBG_LOG("");
        decodedInstance.print();
    } catch (bEncodeReturnTypes err) {
        DBG_LOG("");
        printError(err);
    }

    return 0;
}
