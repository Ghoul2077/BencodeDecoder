#include <bits/stdc++.h>

#include "bEncodeToken.h"
#include "utils.h"
using namespace std;

#define B_TOKEN_END 'e'

enum bEncodeReturnTypes {
    B_SUCCESS = 1,
    B_UNIDENTIFIED_ERROR = -1,
    B_INTEGER_ERROR = -2,
    B_STRING_SIZE_ERROR = -3,
    B_STRING_ERROR = -4,
    B_LIST_ERROR = -5,
    B_DICTIONARY_ERROR = -6,
    B_INPUT_ARGS_ERROR = -7,
    B_DELIMITERS_NOT_PROPER = -8
};

class Bdecode {
   private:
    int tokenCount;
    unordered_map<bigInt, BEncodeToken> decodedData;
    unordered_map<bigInt, bigInt> delimiterPairLocation;

    bigInt parseInt(const string& input, const int& start, bigInt& out,
                    bEncodeReturnTypes& ret);
    bigInt parseStringSize(const string& input, const int& start, bigInt& size,
                           bEncodeReturnTypes& ret);
    bigInt parseString(const string& input, const int& start,
                       const bigInt& stringSize, string& out,
                       bEncodeReturnTypes& ret);
    unordered_map<bigInt, bigInt> parseDelimiters(const string& input,
                                                  const bigInt& start,
                                                  const bigInt& end,
                                                  bEncodeReturnTypes& res);
    bigInt decode(const string& input, const int& start, const int& end,
                  bEncodeReturnTypes& res);

   public:
    Bdecode(const string& input) {
        tokenCount = 0;
        bEncodeReturnTypes res = B_SUCCESS;
        bigInt inputSize = input.size();

        delimiterPairLocation = parseDelimiters(input, 0, inputSize, res);
        if (res != B_SUCCESS) {
            throw B_DELIMITERS_NOT_PROPER;
        }
        decode(input, 0, inputSize, res);
        if (res != B_SUCCESS) {
            throw res;
        }
    }

    Bdecode(const string& input, const bigInt& start, const bigInt& stop) {
        tokenCount = 0;
        bEncodeReturnTypes res = B_SUCCESS;

        delimiterPairLocation = parseDelimiters(input, start, stop, res);
        if (res != B_SUCCESS) {
            throw B_DELIMITERS_NOT_PROPER;
        }
        decode(input, start, stop, res);
        if (res != B_SUCCESS) {
            throw res;
        }
    }

    Bdecode(const string& input, const bigInt& start, const bigInt& stop,
            unordered_map<bigInt, bigInt> const& _delimiterLocation) {
        delimiterPairLocation = _delimiterLocation;
        tokenCount = 0;

        bEncodeReturnTypes res = B_SUCCESS;
        decode(input, start, stop, res);
        if (res != B_SUCCESS) {
            throw res;
        }
    }

    void print();
    vector<BEncodeToken> getVectorizedFormat();
    unordered_map<string, BEncodeToken> getDictionaryFormat();
    bool getElementAtIndex(const int& index, void* res,
                           bEncodeDataTypes& returnType);
};
