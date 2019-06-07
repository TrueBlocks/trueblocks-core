#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
    using string_q     = std::string;
    using CStringArray = vector<string_q>;
    using CUintArray   = vector<uint64_t>;
    typedef bool (*APPLYFUNC)(string_q& line, void *data);

    //---------------------------------------------------------------------------------------
    // We use 'extract' instead of the string class's substr to protect against throws
    extern string_q extract(const string_q& haystack, size_t pos, size_t len = string::npos);

    //---------------------------------------------------------------------------------------
    extern void replace    (string_q& target, const string_q& what, const string_q& with);
    extern void replaceAll (string_q& target, const string_q& what, const string_q& with);
    extern void replaceAny (string_q& target, const string_q& list, const string_q& with);
    extern void replaceReverse(string_q& target, const string_q& what, const string_q& with);
    extern void reverse    (string_q& target);

    //---------------------------------------------------------------------------------------
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
    inline bool operator%(const string_q& str1, const char *str2) {
        return !strcasecmp(str1.c_str(), str2);
    }

    //--------------------------------------------------------------------
    inline bool operator%(const char *str1, const string_q& str2) {
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
    extern size_t  countOf        (const string_q& haystack, char needle);
    extern bool    startsWith     (const string_q& haystack, const string_q& needle);
    extern bool    startsWith     (const string_q& haystack, char ch);
    extern bool    endsWith       (const string_q& haystack, const string_q& needle);
    extern bool    endsWith       (const string_q& haystack, char ch);
    extern bool    endsWithAny    (const string_q& haystack, const string_q& needle);
    extern bool    startsWithAny  (const string_q& haystack, const string_q& needle);
    extern bool    contains       (const string_q& haystack, const string_q& needle);
    extern bool    contains       (const string_q& haystack, char ch);
    extern bool    containsI      (const string_q& haystack, const string_q& needle);
    extern bool    containsAny    (const string_q& haystack, const string_q& needle);

    //--------------------------------------------------------------------
    extern string_q toLower       (const string_q& in);
    extern string_q toUpper       (const string_q& in);
    extern string_q toProper      (const string_q& in);

    //--------------------------------------------------------------------
    extern string_q padRight      (const string_q& str, size_t len, char p = ' ');
    extern string_q padLeft       (const string_q& str, size_t len, char p = ' ');
    extern string_q padCenter     (const string_q& str, size_t len, char p = ' ');

    //--------------------------------------------------------------------
    extern string_q trim          (const string_q& str, char c = ' ');
    extern string_q trimTrailing  (const string_q& str, char c = ' ');
    extern string_q trimLeading   (const string_q& str, char c = ' ');
    extern string_q trimWhitespace(const string_q& str, const string_q& add = "");

    //--------------------------------------------------------------------
    extern string_q nextTokenClearReverse(string_q& str, char token);
    extern string_q nextTokenClear       (string_q& str, char delim, bool doClear = true);
    extern string_q snagFieldClear       (string_q& str, const string_q& tagName, const string_q& defVal = "");
    extern size_t   explode              (CStringArray& result, const string& input, char ch);
    extern size_t   explode              (CUintArray& result, const string& input, char ch);
    extern size_t   forEveryExplodedItem (const string& input, char ch, APPLYFUNC func, void *data);

    //---------------------------------------------------------------------------
    template<typename T>
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
    template<typename T>
    string_q join(T begin, T end, string_q separator) {
        return join(begin, end, separator, "");
    }

    //---------------------------------------------------------------------------
    template<typename T>
    string_q join(T begin, T end) {
        return join(begin, end, ", ");
    }

    //---------------------------------------------------------------------------------------
    inline string_q getEnvStr(const char* name) {
        char *sss = getenv(name);
        return ( sss ? string_q(sss) : string_q(""));
    }
    inline string_q getEnvStr(const string_q& name) { return getEnvStr(name.c_str()); }

    // formatting only
    extern string_q uint_2_Str(uint64_t i);
    extern string_q int_2_Str (int64_t  i);
    //--------------------------------------------------------------------
    inline string_q padNum2(uint64_t n) { return padLeft(uint_2_Str((n)), 2, '0'); }
    inline string_q padNum3(uint64_t n) { return padLeft(uint_2_Str((n)), 3, '0'); }
    inline string_q padNum4(uint64_t n) { return padLeft(uint_2_Str((n)), 4, '0'); }
    inline string_q padNum5(uint64_t n) { return padLeft(uint_2_Str((n)), 5, '0'); }
    inline string_q padNum6(uint64_t n) { return padLeft(uint_2_Str((n)), 6, '0'); }
    inline string_q padNum7(uint64_t n) { return padLeft(uint_2_Str((n)), 7, '0'); }
    inline string_q padNum8(uint64_t n) { return padLeft(uint_2_Str((n)), 8, '0'); }
    inline string_q padNum9(uint64_t n) { return padLeft(uint_2_Str((n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline string_q padNum2(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 2, '0'); }
    inline string_q padNum3(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 3, '0'); }
    inline string_q padNum4(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 4, '0'); }
    inline string_q padNum5(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 5, '0'); }
    inline string_q padNum6(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 6, '0'); }
    inline string_q padNum7(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 7, '0'); }
    inline string_q padNum8(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 8, '0'); }
    inline string_q padNum9(uint32_t n) { return padLeft(uint_2_Str(uint64_t(n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline string_q padNum2(int64_t n) { return padLeft(int_2_Str((n)), 2, '0'); }
    inline string_q padNum3(int64_t n) { return padLeft(int_2_Str((n)), 3, '0'); }
    inline string_q padNum4(int64_t n) { return padLeft(int_2_Str((n)), 4, '0'); }
    inline string_q padNum5(int64_t n) { return padLeft(int_2_Str((n)), 5, '0'); }
    inline string_q padNum6(int64_t n) { return padLeft(int_2_Str((n)), 6, '0'); }
    inline string_q padNum7(int64_t n) { return padLeft(int_2_Str((n)), 7, '0'); }
    inline string_q padNum8(int64_t n) { return padLeft(int_2_Str((n)), 8, '0'); }
    inline string_q padNum9(int64_t n) { return padLeft(int_2_Str((n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline string_q padNum2(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 2, '0'); }
    inline string_q padNum3(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 3, '0'); }
    inline string_q padNum4(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 4, '0'); }
    inline string_q padNum5(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 5, '0'); }
    inline string_q padNum6(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 6, '0'); }
    inline string_q padNum7(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 7, '0'); }
    inline string_q padNum8(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 8, '0'); }
    inline string_q padNum9(int32_t n) { return padLeft(int_2_Str(int64_t(n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline string_q padNum3T(uint64_t n) { return padLeft(uint_2_Str((n)), 3); }
    inline string_q padNum4T(uint64_t n) { return padLeft(uint_2_Str((n)), 4); }
    inline string_q padNum5T(uint64_t n) { return padLeft(uint_2_Str((n)), 5); }
    inline string_q padNum6T(uint64_t n) { return padLeft(uint_2_Str((n)), 6); }
    inline string_q padNum7T(uint64_t n) { return padLeft(uint_2_Str((n)), 7); }
    inline string_q padNum8T(uint64_t n) { return padLeft(uint_2_Str((n)), 8); }
    inline string_q padNum9T(uint64_t n) { return padLeft(uint_2_Str((n)), 9); }

    //--------------------------------------------------------------------
    inline string_q padNum3T(int64_t n) { return padLeft(int_2_Str((n)), 3); }
    inline string_q padNum4T(int64_t n) { return padLeft(int_2_Str((n)), 4); }
    inline string_q padNum5T(int64_t n) { return padLeft(int_2_Str((n)), 5); }
    inline string_q padNum6T(int64_t n) { return padLeft(int_2_Str((n)), 6); }
    inline string_q padNum7T(int64_t n) { return padLeft(int_2_Str((n)), 7); }
    inline string_q padNum8T(int64_t n) { return padLeft(int_2_Str((n)), 8); }
    inline string_q padNum9T(int64_t n) { return padLeft(int_2_Str((n)), 9); }

    //--------------------------------------------------------------------
    extern string_q escape_string(const string_q& str);
    extern void cleanString(string_q& str, bool isCode);

}  // namespace qblocks
