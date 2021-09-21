/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <string>
#include "basetypes.h"
#include "conversions.h"

namespace qblocks {

//---------------------------------------------------------------------------------------
string_q nextTokenClear(string_q& line, char delim, bool doClear) {
    string_q ret;
    size_t find = line.find(delim);
    if (find != string::npos) {
        ret = extract(line, 0, find);
        line = extract(line, find + 1);

    } else if (!line.empty()) {
        ret = line;
        if (doClear)
            line = "";
    }
    return ret;
}

//--------------------------------------------------------------------------------
size_t explode(CStringArray& result, const string& input, char needle, bool trim) {
    result.reserve(result.size() + countOf(input, needle) + 1);  // maybe an append

    string_q buffer{""};
    for (auto ch : input) {
        if (ch != needle) {
            buffer += ch;
        } else if (ch == needle) {
            if (trim)
                result.push_back(trimWhitespace(buffer));
            else
                result.push_back(buffer);
            buffer = "";
        }
    }

    if (buffer != "")
        result.push_back(buffer);
    return result.size();
}

//--------------------------------------------------------------------------------
size_t explode(CUintArray& result, const string& input, char needle, bool trim) {
    result.reserve(result.size() + countOf(input, needle) + 1);  // maybe an append

    string_q buffer{""};
    for (auto ch : input) {
        if (ch != needle) {
            buffer += ch;
        } else if (ch == needle) {
            if (trim)
                result.push_back(str_2_Uint(trimWhitespace(buffer)));
            else
                result.push_back(str_2_Uint(buffer));
            buffer = "";
        }
    }

    if (buffer != "")
        result.push_back(str_2_Uint(buffer));
    return result.size();
}

//--------------------------------------------------------------------------------
size_t forEveryExplodedItem(const string& input, char needle, APPLYFUNC func, void* data) {
    if (!func)
        return 0;

    string_q buffer{""};

    for (auto ch : input) {
        if (ch != needle) {
            buffer += ch;
        } else if (ch == needle) {
            func(buffer, data);
            buffer = "";
        }
    }
    return 0;
}

//---------------------------------------------------------------------------------------
bool contains(const string_q& haystack, const string_q& needle) {
    return (haystack.find(needle) != string::npos);
}

//---------------------------------------------------------------------------------------
bool contains(const string_q& haystack, char ch) {
    string_q ss;
    ss = ch;
    return contains(haystack, ss);
}

//---------------------------------------------------------------------------------------
bool containsI(const string_q& haystack, const string_q& needle) {
    string_q hay = toLower(haystack);
    string_q need = toLower(needle);
    return contains(hay, need);
}

//---------------------------------------------------------------------------------------
void replace(string_q& target, const string_q& what, const string_q& with) {
    size_t f = target.find(what);
    if (f != string::npos)
        target = (extract(target, 0, f) + string_q(with.c_str()) + extract(target, f + what.length()));
}

//---------------------------------------------------------------------------------------
void replaceAll(string_q& target, const string_q& what, const string_q& with) {
    if (what.empty())
        return;

    if (with.find(what) != string::npos) {
        // may cause endless recursions so do it in two steps instead
        replaceAll(target, what, "~*~``");
        replaceAll(target, "~*~``", with);
        return;
    }

    size_t f = target.find(what);
    while (f != string::npos) {
        replace(target, what, with);
        f = target.find(what);
    }
}

//---------------------------------------------------------------------------------------
void replaceAny(string_q& target, const string_q& list, const string_q& with) {
    for (size_t i = 0; i < list.length(); i++) {
        string_q ss;
        ss = list[i];
        replaceAll(target, ss, with);
    }
}

//---------------------------------------------------------------------------------------
void replaceReverse(string_q& target, const string_q& what, const string_q& with) {
    string_q w1 = what;
    string_q w2 = with;
    reverse(target);
    reverse(w1);
    reverse(w2);
    replace(target, w1, w2);
    reverse(target);
}

//---------------------------------------------------------------------------------------
void reverse(string_q& target) {
    size_t i, j;
    size_t n = target.length();
    for (i = 0, j = n - 1; i < n / 2; i++, j--) {
        char tmp = target[i];
        target[i] = target[j];
        target[j] = tmp;
    }
}

//----------------------------------------------------------------------------------------
string_q nextTokenClearReverse(string_q& str, char token) {
    reverse(str);
    string_q ret = nextTokenClear(str, token);
    reverse(ret);
    reverse(str);
    return ret;
}

//---------------------------------------------------------------------------------------
string_q snagFieldClear(string_q& in, const string_q& field, const string_q& defVal) {
    string_q f1 = "</" + field + ">";
    string_q ret = extract(in, 0, in.find(f1));

    string_q f2 = "<" + field + ">";
    ret = extract(ret, ret.find(f2) + f2.length());

    replace(in, f2 + ret + f1, "");

    if (ret.empty())
        ret = defVal;

    return ret;
}

//--------------------------------------------------------------------
size_t countOf(const string_q& haystack, char needle) {
    string hay = haystack.c_str();
    return (size_t)count(hay.begin(), hay.end(), needle);
}

//--------------------------------------------------------------------
bool startsWith(const string_q& haystack, const string_q& needle) {
    string_q hay = haystack.c_str();
    if (hay.empty() || needle.empty())
        return false;
    return (extract(hay, 0, needle.length()) == needle);
}

//--------------------------------------------------------------------
bool startsWith(const string_q& haystack, char ch) {
    string_q ss;
    ss = ch;
    return startsWith(haystack, ss);
}

//--------------------------------------------------------------------
bool endsWith(const string_q& haystack, const string_q& needle) {
    string_q hay = haystack.c_str();
    if (hay.empty() || needle.empty())
        return false;
    return (extract(hay, hay.length() - needle.length(), needle.length()) == needle);
}

//--------------------------------------------------------------------
bool endsWith(const string_q& haystack, char ch) {
    string_q ss;
    ss = ch;
    return endsWith(haystack, ss);
}

//---------------------------------------------------------------------------------------
bool startsWithAny(const string_q& haystack, const string_q& needles) {
    string need = needles.c_str();
    for (const auto elem : need)
        if (startsWith(haystack, elem))
            return true;
    return false;
}

//---------------------------------------------------------------------------------------
bool endsWithAny(const string_q& haystack, const string_q& needles) {
    string need = needles.c_str();
    for (const auto elem : need)
        if (endsWith(haystack, elem))
            return true;
    return false;
}

//---------------------------------------------------------------------------------------------------
string_q firstLower(const string_q& in) {
    string_q ret = in;
    if (ret.length())
        ret[0] = (char)tolower(ret[0]);
    return ret;
}

//---------------------------------------------------------------------------------------------------
string_q firstUpper(const string_q& in) {
    string_q ret = in;
    if (ret.length())
        ret[0] = (char)toupper(ret[0]);
    return ret;
}

//--------------------------------------------------------------------
string_q toLower(const string_q& in) {
    string ret;
    string str = in.c_str();
    for (const auto elem : str)
        ret += static_cast<char>(tolower(elem));
    return ret.c_str();
}

//--------------------------------------------------------------------
string_q toUpper(const string_q& in) {
    string ret;
    string str = in.c_str();
    for (const auto elem : str)
        ret += static_cast<char>(toupper(elem));
    return ret.c_str();
}

//--------------------------------------------------------------------
string_q toProper(const string_q& in) {
    string ret;
    string str = in.c_str();
    char prev = ' ';  // not a space
    for (const auto elem : str) {
        if (isspace(prev) || prev == '_')
            ret += static_cast<char>(toupper(elem));
        else
            ret += static_cast<char>(tolower(elem));
        prev = elem;
    }
    return ret.c_str();
}

//--------------------------------------------------------------------
string_q trim(const string_q& str, char c) {
    return trimTrailing(trimLeading(str, c), c);
}

//--------------------------------------------------------------------
string_q trimTrailing(const string_q& str, char c) {
    string_q ret = str;
    while (endsWith(ret, c))
        ret = extract(ret, 0, ret.length() - 1);
    return ret;
}

//--------------------------------------------------------------------
string_q trimLeading(const string_q& str, char c) {
    string_q ret = str.c_str();
    while (startsWith(ret, c))
        ret = extract(ret, 1);
    return ret.c_str();
}

//--------------------------------------------------------------------
string_q trimWhitespace(const string_q& str, const string_q& add) {
    string_q ret = str.c_str();
    string_q any = string_q("\t\r\n ") + add.c_str();
    while (endsWithAny(ret, any) || startsWithAny(ret, any)) {
        for (size_t i = 0; i < any.length(); i++)
            ret = trim(ret, any[i]);
    }
    return ret.c_str();
}

//---------------------------------------------------------------------------
string_q stripWhitespace(const string_q& str) {
    return substitute(substitute(substitute(substitute(str, "\"", "\\\""), "\n", " "), "\r", " "), "\t", " ");
}

//--------------------------------------------------------------------
string_q padRight(const string_q& str, size_t len, char p) {
    if (len > str.length())
        return str + string_q(len - str.length(), p);
    return str;
}

//--------------------------------------------------------------------
string_q padLeft(const string_q& str, size_t len, char p) {
    if (len > str.length())
        return string_q(len - str.length(), p) + str;
    return str;
}

//--------------------------------------------------------------------
string_q padCenter(const string_q& str, size_t len, char p) {
    if (len > str.length()) {
        size_t padding = (len - str.length()) / 2;
        return string_q(padding, p) + str + string_q(padding, p);
    }
    return str;
}

//--------------------------------------------------------------------
string_q extract(const string_q& haystack, size_t pos, size_t len) {
    if (pos >= haystack.length())
        return "";
    return haystack.substr(pos, len);
}

//--------------------------------------------------------------------
string_q escape_string(const string_q& str) {
    string_q res;
    for (auto it = str.begin(); it != str.end(); ++it) {
        // clang-format off
             if (*it == '\b') res += "\\b";  // NOLINT
        else if (*it == '\t') res += "\\t";
        else if (*it == '\n') res += "\\n";
        else if (*it == '\f') res += "\\f";
        else if (*it == '\r') res += "\\r";
        else if (*it == '"') res += "\\\"";
        else if (*it == '\\') res += "\\\\";
        else if (static_cast<uint32_t>(*it) <= UINT32_C(0x001f)) {  // NOLINT
            res += "\\u";
            stringstream ss;
            ss << hex << static_cast<uint32_t>(*it);
            res += ss.str();
        } else {
            res += *it;
        }
        // clang-format on
    }
    return res;
}

//--------------------------------------------------------------------------------
void cleanString(string_q& str, bool isCode) {
    size_t pos = 0;
    typedef enum { OUT, IN_DASH, IN_SPACE, IN_NL, COMM_START, IN_COMM } StateThing;
    string_q comment_end, lastTwo = "~~";
    StateThing state = OUT;
    for (auto ch : str) {
        lastTwo = trim((comment_end == "\n" ? ' ' : lastTwo[lastTwo.length() - 1]) + string(1, ch));
        switch (state) {
            case OUT:
                switch (ch) {
                    case '\t':
                        break;  // skip it
                    case '\r':
                        if (!isCode)
                            str[pos++] = ch;
                        break;
                    case '\n':
                        if (isCode)
                            str[pos++] = ' ';
                        else
                            str[pos++] = ch;
                        break;
                    case ' ':
                        state = IN_SPACE;
                        break;
                    case '-':
                        if (!isCode)
                            state = IN_DASH;
                        else
                            str[pos++] = ch;
                        break;
                    case '\\':
                        state = IN_NL;
                        break;
                    case '/':
                        if (isCode)
                            state = COMM_START;
                        break;
                    default:
                        str[pos++] = ch;
                }
                break;
            case IN_SPACE:
                if (isCode && ch == '\n') {
                    str[pos++] = ' ';
                } else {
                    switch (ch) {
                        case ' ':
                            break;  // skip it
                        default:
                            str[pos++] = ' ';
                            str[pos++] = ch;
                            state = OUT;
                            break;
                    }
                }
                break;
            case IN_DASH:
                switch (ch) {
                    case 'v':
                    case 'h':
                        break;  // skip it
                    default:
                        str[pos++] = '-';
                        str[pos++] = ch;
                        state = OUT;
                        break;
                }
                break;
            case IN_NL:
                switch (ch) {
                    case '\n':
                        break;  // skip it
                    default:
                        str[pos++] = '\\';
                        str[pos++] = ch;
                        state = OUT;
                        break;
                }
                break;
            case COMM_START:
                switch (ch) {
                    case '*':
                        state = IN_COMM;
                        comment_end = "*/";
                        break;
                    case '/':
                        state = IN_COMM;
                        comment_end = "\n";
                        break;
                    default:
                        str[pos++] = '/';
                        str[pos++] = ch;
                        state = OUT;
                        break;
                }
                break;
            case IN_COMM:
                if (lastTwo == comment_end) {
                    state = OUT;
                }
                break;
        }
    }
    str[pos] = '\0';
    str.resize(pos);
}

//--------------------------------------------------------------------------------
string_q removeCharacters_inner(const string_q& str, size_t n, const char* chars) {
    string_q ret;
    for (auto ch : str) {
        bool found = false;
        for (size_t i = 0; i < n && !found; i++) {
            if (chars[i] == ch) {
                found = true;
            }
        }
        if (!found)
            ret += ch;
    }
    return ret;
}

//--------------------------------------------------------------------------------
void removeCharacters(string_q& str, size_t n, const char* chars) {
    str = removeCharacters_inner(str, n, chars);
}

//-----------------------------------------------------------------------
void simplifySolidity(string_q& code) {
    replaceAll(code, "`", "");    // easier
    replaceAll(code, "*/", "`");  // easier
    string_q ret;
    char endChar = '\0';
    typedef enum { OUT, START, STOP, IN } State;
    State state = OUT;
    for (auto ch : code) {
        if (ch == '\r')
            ch = '\n';
        if (ch == '{')
            ch = ';';
        switch (state) {
            case OUT:
                switch (ch) {
                    case '/':
                        state = START;
                        break;
                    default:
                        ret += ch;
                        break;
                }
                break;
            case START:
                switch (ch) {
                    case '*':
                        state = IN;
                        endChar = '`';  // easier
                        break;
                    case '/':
                        state = IN;
                        endChar = '\n';
                        break;
                    default:
                        ret += '/';
                        state = OUT;
                        break;
                }
                break;
            case IN:
                if (ch == endChar) {
                    state = OUT;
                }
                break;
            default:
                ASSERT(0);
                return;
        }
    }
    code = substitute(substitute(ret, "\t", " "), "  ", " ");
}

//---------------------------------------------------------------------------------------
bool containsAny(const string_q& haystack, const string_q& needle) {
    string need = needle.c_str();
    for (const auto elem : need)
        if (contains(haystack, elem))
            return true;
    return false;
}

//---------------------------------------------------------------------------------------
inline size_t find_nth(const string& haystack, size_t pos, const string& needle, size_t nth) {
    size_t found_pos = haystack.find(needle, pos);
    if (0 == nth || string::npos == found_pos)
        return found_pos;
    return find_nth(haystack, found_pos + 1, needle, nth - 1);
}

//---------------------------------------------------------------------------------------
size_t find_nth(const string& haystack, const string& needle, size_t nth) {
    return find_nth(haystack, 0, needle, nth);
}

string_q substituteAny(const string_q& strIn, const string_q& what, const string_q& with) {
    string_q ret = strIn;
    for (auto ch : what)
        ret = substitute(ret, string_q(1, ch), with);
    return ret;
}
}  // namespace qblocks
