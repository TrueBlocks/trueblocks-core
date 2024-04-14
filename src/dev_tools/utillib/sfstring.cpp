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
#include "sfstring.h"

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

uint64_t str_2_Uint(const string_q& str) {
    return (uint64_t)strtoul(str.c_str(), NULL, 10);
}

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

void replaceReverse(string_q& target, const string_q& what, const string_q& with) {
    string_q w1 = what;
    string_q w2 = with;
    reverse(target);
    reverse(w1);
    reverse(w2);
    replace(target, w1, w2);
    reverse(target);
}

void reverse(string_q& target) {
    size_t i, j;
    size_t n = target.length();
    for (i = 0, j = n - 1; i < n / 2; i++, j--) {
        char tmp = target[i];
        target[i] = target[j];
        target[j] = tmp;
    }
}

size_t countOf(const string_q& haystack, char needle) {
    string hay = haystack.c_str();
    return (size_t)count(hay.begin(), hay.end(), needle);
}

bool startsWith(const string_q& haystack, const string_q& needle) {
    string_q hay = haystack.c_str();
    if (hay.empty() || needle.empty())
        return false;
    return (extract(hay, 0, needle.length()) == needle);
}

bool startsWith(const string_q& haystack, char ch) {
    string_q ss;
    ss = ch;
    return startsWith(haystack, ss);
}

bool endsWith(const string_q& haystack, const string_q& needle) {
    string_q hay = haystack.c_str();
    if (hay.empty() || needle.empty())
        return false;
    return (extract(hay, hay.length() - needle.length(), needle.length()) == needle);
}

bool endsWith(const string_q& haystack, char ch) {
    string_q ss;
    ss = ch;
    return endsWith(haystack, ss);
}

bool startsWithAny(const string_q& haystack, const string_q& needles) {
    string need = needles.c_str();
    for (const auto elem : need)
        if (startsWith(haystack, elem))
            return true;
    return false;
}

bool endsWithAny(const string_q& haystack, const string_q& needles) {
    string need = needles.c_str();
    for (const auto elem : need)
        if (endsWith(haystack, elem))
            return true;
    return false;
}

string_q toLower(const string_q& in) {
    string ret;
    string str = in.c_str();
    for (const auto elem : str)
        ret += static_cast<char>(tolower(elem));
    return ret.c_str();
}

string_q toUpper(const string_q& in) {
    string ret;
    string str = in.c_str();
    for (const auto elem : str)
        ret += static_cast<char>(toupper(elem));
    return ret.c_str();
}

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

string_q toCamelCase(const string_q& in) {
    string_q ret = substitute(toProper(in), "_", "");
    ret[0] = toLower(ret)[0];
    return ret;
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

string_q trimWhitespace(const string_q& str, const string_q& add) {
    string_q ret = str.c_str();
    string_q any = string_q("\t\r\n ") + add.c_str();
    while (endsWithAny(ret, any) || startsWithAny(ret, any)) {
        for (size_t i = 0; i < any.length(); i++)
            ret = trim(ret, any[i]);
    }
    return ret.c_str();
}

string_q extract(const string_q& haystack, size_t pos, size_t len) {
    if (pos >= haystack.length())
        return "";
    return haystack.substr(pos, len);
}

bool containsAny(const string_q& haystack, const string_q& needle) {
    string need = needle.c_str();
    for (const auto elem : need)
        if (contains(haystack, elem))
            return true;
    return false;
}

}  // namespace qblocks
