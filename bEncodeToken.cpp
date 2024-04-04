#include "bEncodeToken.h"

BEncodeToken::BEncodeToken(const BEncodeToken& _) : type(_.type) {
    switch (_.type) {
        case B_INTEGER:
            num = _.num;
            break;
        case B_STRING:
            str = _.str;
            break;
        case B_LIST:
            list = _.list;
            break;
        case B_DICTIONARY:
            dict = _.dict;
            break;
        default:
            break;
    }
}

BEncodeToken& BEncodeToken::operator=(BEncodeToken const& _data) {
    type = _data.type;
    switch (_data.type) {
        case B_INTEGER:
            num = _data.num;
            break;
        case B_STRING:
            str = _data.str;
            break;
        case B_LIST: {
            list = _data.list;
            break;
        }
        case B_DICTIONARY: {
            dict = _data.dict;
            break;
        }
        default:
            break;
    }
    return *this;
}

bool BEncodeToken::operator==(const BEncodeToken& _data) const {
    if (type == _data.type) {
        if (type == B_NONE) {
            return true;
        }
        switch (type) {
            case B_STRING:
                return (str == _data.str);
                break;
            case B_INTEGER:
                return (num == _data.num);
                break;
            case B_LIST:
                return (list == _data.list);
                break;
            case B_DICTIONARY:
                return (dict == _data.dict);
                break;
            default:
                break;
        }
    }
    return false;
}

void BEncodeToken::print() const {
    switch (type) {
        case B_INTEGER:
            cout << num;
            break;
        case B_STRING:
            cout << str;
            break;
        case B_LIST: {
            int instances = 0;
            cout << "[";
            for (auto i : list) {
                i.print();
                instances++;
                if (instances != list.size()) {
                    cout << ", ";
                }
            }
            cout << "]";
            break;
        }
        case B_DICTIONARY: {
            int instances = 0;
            cout << "{";
            for (auto i : dict) {
                cout << i.first;
                cout << " : ";
                i.second.print();
                instances++;
                if (instances != dict.size()) {
                    cout << ", ";
                }
            }
            cout << "}";
            break;
        }
        default:
            break;
    }
}

string BEncodeToken::toString() const {
    string res = "";

    switch (type) {
        case B_INTEGER: {
            if (!intToString(num, res)) {
                cout << __FUNCTION__ << "() : " << __LINE__ << " : "
                     << "Cannot convert the integer token to string" << endl;
                res = "";
            }
            break;
        }
        case B_STRING: {
            res = "\"" + str + "\"";
            break;
        }
        case B_LIST: {
            vector<string> out;
            for (auto i : list) {
                out.push_back(i.toString());
            }
            if (!arrayToString(out, res)) {
                cout << __FUNCTION__ << "() : " << __LINE__ << " : "
                     << "Cannot convert the list to string" << endl;
                res = "";
            }
            break;
        }
        case B_DICTIONARY: {
            unordered_map<string, string> out;
            for (auto i : dict) {
                if (out.count(i.first) == 0) {
                    out[i.first] = i.second.toString();
                }
            }
            if (!unorderedMapToString(out, res)) {
                cout << __FUNCTION__ << "() : " << __LINE__ << " : "
                     << "Cannot convert dictionary to string" << endl;
            }
            break;
        }
        default:
            break;
    }

    return res;
}
