#include "bEncodeToken.h"

BEncodeToken::BEncodeToken() : val() {}

BEncodeToken::BEncodeToken(const bigInt _) : val(_) {}

BEncodeToken::BEncodeToken(const string _) : val(_) {}

BEncodeToken::BEncodeToken(const vector<BEncodeToken> _) : val(_) {}

BEncodeToken::BEncodeToken(const unordered_map<string, BEncodeToken> _) {
    val = _;
}

BEncodeToken::BEncodeToken(const BEncodeToken& _) { val = _.val; }

BEncodeToken& BEncodeToken::operator=(BEncodeToken const& _) {
    val = _.val;
    return *this;
}

bool BEncodeToken::operator==(const BEncodeToken& _) const {
    return (val == _.val);
}

void BEncodeToken::print() const {
    if (holds_alternative<bigInt>(val)) {
        cout << get<bigInt>(val);
    } else if (holds_alternative<string>(val)) {
        string const* str = &get<string>(val);
        cout << *str;
    } else if (holds_alternative<vector<BEncodeToken>>(val)) {
        int instances = 0;
        cout << "[";
        vector<BEncodeToken> const* list = &get<vector<BEncodeToken>>(val);
        for (auto i : *list) {
            i.print();
            instances++;
            if (instances != list->size()) {
                cout << ", ";
            }
        }
        cout << "]";
    } else if (holds_alternative<unordered_map<string, BEncodeToken>>(val)) {
        int instances = 0;
        cout << "{";
        unordered_map<string, BEncodeToken> const* dict =
            &get<unordered_map<string, BEncodeToken>>(val);
        for (auto i : *dict) {
            cout << i.first;
            cout << " : ";
            i.second.print();
            instances++;
            if (instances != dict->size()) {
                cout << ", ";
            }
        }
        cout << "}";
    }
}

string BEncodeToken::toString() const {
    string res = "";

    if (holds_alternative<bigInt>(val)) {
        bigInt const* num = &get<bigInt>(val);
        if (!intToString(*num, res)) {
            cout << __FUNCTION__ << "() : " << __LINE__ << " : "
                 << "Cannot convert the integer token to string" << endl;
            res = "";
        }
    } else if (holds_alternative<string>(val)) {
        string const* str = &get<string>(val);
        res = "\"" + (*str) + "\"";
    } else if (holds_alternative<vector<BEncodeToken>>(val)) {
        vector<BEncodeToken> const* list = &get<vector<BEncodeToken>>(val);
        vector<string> out;
        for (auto i : *list) {
            out.push_back(i.toString());
        }
        if (!arrayToString(out, res)) {
            cout << __FUNCTION__ << "() : " << __LINE__ << " : "
                 << "Cannot convert the list to string" << endl;
            res = "";
        }
    } else if (holds_alternative<unordered_map<string, BEncodeToken>>(val)) {
        unordered_map<string, BEncodeToken> const* dict =
            &get<unordered_map<string, BEncodeToken>>(val);
        unordered_map<string, string> out;
        for (auto i : *dict) {
            if (out.count(i.first) == 0) {
                out[i.first] = i.second.toString();
            }
        }
        if (!unorderedMapToString(out, res)) {
            cout << __FUNCTION__ << "() : " << __LINE__ << " : "
                 << "Cannot convert dictionary to string" << endl;
        }
    }

    return res;
}

bEncodeDataTypes BEncodeToken::getDatatype() const {
    if (holds_alternative<bigInt>(val)) {
        return B_INTEGER;
    } else if (holds_alternative<string>(val)) {
        return B_STRING;
    } else if (holds_alternative<vector<BEncodeToken>>(val)) {
        return B_LIST;
    } else if (holds_alternative<unordered_map<string, BEncodeToken>>(val)) {
        return B_DICTIONARY;
    } else {
        return B_NONE;
    }
}
