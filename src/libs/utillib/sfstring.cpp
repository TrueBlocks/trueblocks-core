/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "basetypes.h"
#include "conversions.h"

namespace qblocks {

    //---------------------------------------------------------------------------------------
    string_q nextTokenClear(string_q& line, char delim, bool doClear) {
        string_q ret;
        size_t find = line.find(delim);
        if (find != string::npos) {
            ret  = line.substr(0,find);
            line = line.substr(find+1);

        } else if (!line.empty()) {
            ret  = line;
            if (doClear)
                line = "";
        }
        return ret;
    }

    //---------------------------------------------------------------------------------------
    char nullStr[2];

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
            target = (target.substr(0, f) + string_q(with.c_str()) + target.substr(f + what.length()));
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
        for (size_t i = 0 ; i < list.length() ; i++) {
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
        size_t i,j;
        size_t n = target.length();
        for ( i = 0, j = n-1 ; i < n/2; i++, j-- ) {
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
        string_q ret = in.substr(0,in.find(f1));

        string_q f2 = "<" + field + ">";
        ret = ret.substr(ret.find(f2)+f2.length());

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
        return (hay.substr(0, needle.length()) == needle);
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
        return (hay.substr(hay.length() - needle.length(), needle.length()) == needle);
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
        for (auto elem : need)
            if (startsWith(haystack, elem))
                return true;
        return false;
    }

    //---------------------------------------------------------------------------------------
    bool endsWithAny(const string_q& haystack, const string_q& needles) {
        string need = needles.c_str();
        for (auto elem : need)
            if (endsWith(haystack, elem))
                return true;
        return false;
    }

    //--------------------------------------------------------------------
    string_q toLower(const string_q& in) {
        string ret;
        string str = in.c_str();
        for (auto elem : str)
            ret += (char)tolower(elem);
        return ret.c_str();
    }

    //--------------------------------------------------------------------
    string_q toUpper(const string_q& in) {
        string ret;
        string str = in.c_str();
        for (auto elem : str)
            ret += (char)toupper(elem);
        return ret.c_str();
    }

    //--------------------------------------------------------------------
    string_q toProper(const string_q& in) {
        string ret;
        string str = in.c_str();
        char prev = ' '; // not a space
        for (auto elem : str) {
            if (isspace(prev) || prev == '_')
                ret += (char)toupper(elem);
            else
                ret += (char)tolower(elem);
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
            ret = ret.substr(0,ret.length()-1);
        return ret;
    }

    //--------------------------------------------------------------------
    string_q trimLeading(const string_q& str, char c) {
        string_q ret = str.c_str();
        while (startsWith(ret, c))
            ret = ret.substr(1);
        return ret.c_str();
    }

    //--------------------------------------------------------------------
    string_q trimWhitespace(const string_q& str, const string_q& add) {
        string_q ret = str.c_str();
        string_q any = string_q("\t\r\n ") + add.c_str();
        while (endsWithAny(ret, any) || startsWithAny(ret, any)) {
            for (size_t i = 0 ; i < any.length() ; i++)
                ret = trim(ret, any[i]);
        }
        return ret.c_str();
    }

    //--------------------------------------------------------------------
    string_q padRight(const string_q& str, size_t len, char p) {
        if (len > str.length())
            return str + string_q(len-str.length(), p);
        return str;
    }

    //--------------------------------------------------------------------
    string_q padLeft(const string_q& str, size_t len, char p) {
        if (len > str.length())
            return string_q(len-str.length(), p) + str;
        return str;
    }

    //--------------------------------------------------------------------
    string_q padCenter(const string_q& str, size_t len, char p) {
        if (len > str.length()) {
            size_t padding = (len-str.length()) / 2;
            return string_q(padding, p) + str + string_q(padding, p);
        }
        return str;
    }
}  // namespace qblocks
