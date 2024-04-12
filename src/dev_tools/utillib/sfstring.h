#pragma once
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

namespace qblocks {

//--------------------------------------------------------------------
using string_q = std::string;
using CStringArray = vector<string_q>;
using CUintArray = vector<uint64_t>;

//-------------------------------------------------------------------------
typedef bool (*APPLYFUNC)(string_q& line, void* data);
typedef bool (*CONSTAPPLYFUNC)(const string_q& path, void* data);
typedef bool (*CHARPTRFUNC)(const char* str, void* data);
typedef int (*SEARCHFUNC)(const void* ob1, const void* ob2);
typedef int (*SORTFUNC)(const void* ob1, const void* ob2);

//---------------------------------------------------------------------------------------
// We use 'extract' instead of the string class's substr to protect against throws
extern string_q extract(const string_q& haystack, size_t pos, size_t len = string::npos);

//---------------------------------------------------------------------------------------
extern void replace(string_q& target, const string_q& what, const string_q& with);
extern void replaceAll(string_q& target, const string_q& what, const string_q& with);
extern void replaceAny(string_q& target, const string_q& list, const string_q& with);
extern void replaceReverse(string_q& target, const string_q& what, const string_q& with);
extern void reverse(string_q& target);

//---------------------------------------------------------------------------------------
extern string_q substituteAny(const string_q& strIn, const string_q& what, const string_q& with);
inline string_q substitute(const string_q& strIn, const string_q& what, const string_q& with) {
    string_q ret;
    ret = strIn;
    replaceAll(ret, what, with);
    return ret;
}

//--------------------------------------------------------------------
inline bool operator%(const string_q& str1, const string_q& str2) {
    return !strcasecmp(str1.c_str(), str2.c_str());
}

//--------------------------------------------------------------------
inline bool operator%(const string_q& str1, const char* str2) {
    return !strcasecmp(str1.c_str(), str2);
}

//--------------------------------------------------------------------
inline bool operator%(const char* str1, const string_q& str2) {
    return !strcasecmp(str1, str2.c_str());
}

//--------------------------------------------------------------------
inline bool operator%(const string_q& str1, char ch) {
    string_q str2;
    str2 = ch;
    return !strcasecmp(str1.c_str(), str2.c_str());
}

//--------------------------------------------------------------------
inline bool operator%(char ch, const string_q& str2) {
    string_q str1;
    str1 = ch;
    return !strcasecmp(str1.c_str(), str2.c_str());
}

//--------------------------------------------------------------------
extern size_t countOf(const string_q& haystack, char needle);
extern bool startsWith(const string_q& haystack, const string_q& needle);
extern bool startsWith(const string_q& haystack, char ch);
extern bool endsWith(const string_q& haystack, const string_q& needle);
extern bool endsWith(const string_q& haystack, char ch);
extern bool endsWithAny(const string_q& haystack, const string_q& needle);
extern bool startsWithAny(const string_q& haystack, const string_q& needle);
extern bool contains(const string_q& haystack, const string_q& needle);
extern bool contains(const string_q& haystack, char ch);
extern bool containsI(const string_q& haystack, const string_q& needle);
extern bool containsAny(const string_q& haystack, const string_q& needle);

//--------------------------------------------------------------------
extern string_q toLower(const string_q& in);
extern string_q toUpper(const string_q& in);
extern string_q toProper(const string_q& in);
extern string_q toSingular(const string_q& in);
extern string_q toCamelCase(const string_q& in);
extern string_q firstLower(const string_q& in);
extern string_q firstUpper(const string_q& in);

//--------------------------------------------------------------------
extern string_q padRight(const string_q& str, size_t len, char p = ' ');
extern string_q padLeft(const string_q& str, size_t len, char p = ' ');
extern string_q padCenter(const string_q& str, size_t len, char p = ' ');

//--------------------------------------------------------------------
extern string_q trim(const string_q& str, char c = ' ');
extern string_q trimTrailing(const string_q& str, char c = ' ');
extern string_q trimLeading(const string_q& str, char c = ' ');
extern string_q trimWhitespace(const string_q& str, const string_q& add = "");
extern string_q stripWhitespace(const string_q& str);

//--------------------------------------------------------------------
extern string_q nextTokenClearReverse(string_q& str, char token);
extern string_q nextTokenClear(string_q& str, char delim, bool doClear = true);
extern string_q snagFieldClear(string_q& str, const string_q& tagName, const string_q& defVal = "");
extern size_t explode(CStringArray& result, const string& input, char ch, bool trim = true);
extern size_t explode(CUintArray& result, const string& input, char ch, bool trim = true);

//---------------------------------------------------------------------------
template <typename T>
string_q join(const T begin, const T end, const string_q& separator, const string_q& concluder) {
    stringstream ss;
    for (T iter = begin; iter != end; iter++) {
        if (!ss.str().empty())
            ss << separator;
        ss << *iter;
    }
    ss << concluder;
    return ss.str();
}

//---------------------------------------------------------------------------
template <typename T>
string_q join(T begin, T end, string_q separator) {
    return join(begin, end, separator, "");
}

//---------------------------------------------------------------------------
template <typename T>
string_q join(T begin, T end) {
    return join(begin, end, ", ");
}

}  // namespace qblocks
