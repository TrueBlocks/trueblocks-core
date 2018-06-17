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

#ifdef NATIVE
    //---------------------------------------------------------------------------------------
    string_q::string_q() : string() { }
    string_q::string_q(const string_q& str) : string(str) { }
    string_q::string_q(const char *str, size_t start, size_t len) : string(str, start, len) { }
    string_q::string_q(size_t len, char ch) : string(len, ch) { }
    string_q::string_q(char ch) : string(1, ch) { }
    string_q::~string_q() { }
    string_q string_q::substr(size_t first) const { return substr(first, length()-first); }
    string_q string_q::substr(size_t first, size_t len) const { return string::substr(first, len).c_str(); }
    const string_q& string_q::operator=(const string_q& str) {
        this->string::operator=(str);
        return *this;
    }
    const string_q& string_q::operator=(char ch) {
        string str(1,ch);
        this->string::operator=(str);
        return *this;
    }
    string_q operator+(const string_q& str1, const string_q& str2) {
        string s1(str1.c_str());
        string s2(str2.c_str());
        return (s1 + s2).c_str();
    }
    string_q operator+(const string_q& str1, const char *str2) {
        return operator+(str1, string_q(str2));
    }
    string_q operator+(const char *str1, const string_q& str2) {
        return operator+(string_q(str1), str2);
    }
    string_q operator+(const string_q& str,  char ch) {
        return operator+(str, string_q(1, ch));
    }

#else
    //---------------------------------------------------------------------------------------
    string_q::string_q() {
        initialize();
        reserve(0);
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(const string_q& str) {
        initialize();
        *this = str.m_Values;
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(const char *str, size_t start, size_t len) {
        initialize();

        size_t strLen = (str ? strlen(str) : 0);
        len = (len == NOPOS ? strLen : min(len,strLen));
        if (str && (strLen > start)) {
            reserve(len);
            memcpy(m_Values, &str[start], len);
        }

        m_nValues     = len;
        m_Values[len] = '\0';
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(size_t len, char ch) {
        initialize();

        if ((long)len > 0) {
            reserve(len);
            memset(m_Values, ch, len);
            m_nValues     = len;
            m_Values[len] = '\0';
        }
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(char ch) : string_q(1, ch) {}

    //---------------------------------------------------------------------------------------
    string_q::~string_q() {
        if (m_Values)
            if (m_Values != nullStr)
                delete [] m_Values;
        m_Values  = NULL;
        m_nValues = 0;
    }

    //---------------------------------------------------------------------------------------
    string_q string_q::substr(size_t first) const {
        return substr(first, length()-first);
    }

    //---------------------------------------------------------------------------------------
    string_q string_q::substr(size_t first, size_t len) const {
        if (first+len > length())
            len = length() - first; // not past end
        if (first > length())
            len = 0;  // not longer than string
        return extract(first, len);
    }

    //---------------------------------------------------------------------------------------
    string_q string_q::extract(size_t start, size_t len) const {
        ASSERT((char*)(start+start+len) <= (char*)((int64_t)m_Values+strlen(m_Values)));

        const char *strStart = m_Values;
        strStart += start;
        string_q ret;
        ret.reserve(len);
        memcpy(ret.m_Values, strStart, len);
        ret.m_nValues     = len;
        ret.m_Values[len] = '\0';
        return ret;
    }

    //---------------------------------------------------------------------------------------
    string_q operator+(const string_q& str1, const string_q& str2) {
        size_t newLen = str1.length() + str2.length();
        string_q ret;
        ret.reserve(newLen);
        memcpy(ret.m_Values, str1.m_Values, str1.length());
        memcpy(ret.m_Values+str1.length(), str2.m_Values, str2.length());
        ret.m_nValues = newLen;
        ret.m_Values[newLen] = '\0';
        return ret;
    }

    //---------------------------------------------------------------------------------------
    const string_q& string_q::operator=(const string_q& str) {
        if (m_Values == str.m_Values) // do not change this line
            return *this;
        size_t len = str.length();
        reserve(len);
        memcpy(m_Values, str.m_Values, len);
        m_nValues     = len;
        m_Values[len] = '\0';
        return *this;
    }

    //---------------------------------------------------------------------------------------
    const string_q& string_q::operator=(char ch) {
        return operator=(string_q(ch));
    }

    //---------------------------------------------------------------------------------------
    const char *string_q::c_str(void) const {
        ASSERT(m_Values);
        return m_Values;
    }

    //--------------------------------------------------------------------
    size_t string_q::length() const {
        return m_nValues;
    }

    //--------------------------------------------------------------------
    bool string_q::empty() const {
        return !length();
    }

    //---------------------------------------------------------------------------------------
    void string_q::reserve(size_t newSize) {
        if (newSize <= m_buffSize && m_buffSize) {
            // do not release the memory, just empty the existing string
            m_nValues   = 0;
            memset(m_Values, '\0', m_buffSize);
            return;
        }

        // clear current buffer and deallocate
        clear();
        if (newSize == NOPOS || newSize == 0)
            return;

        // make sure we are not dropping memory
        ASSERT(m_Values == nullStr);

#define GROW_SIZE 16
        m_Values      = new char[newSize+GROW_SIZE+1];
        m_buffSize    = newSize + GROW_SIZE;
        m_nValues     = 0;
        memset(m_Values, '\0', m_buffSize+1);
        return;
    }

    //---------------------------------------------------------------------------------------
    void string_q::resize(size_t newSize, char c) {
        if (newSize <= m_buffSize) {
            // Return is string is shrinking. Not exactly c++ sematics, but it's
            // okay since we're replacing this class
            return;
        }

        // Note: normally we wouldn't be able to use realloc with a 'new' allocated memory,
        // but because these are just characters, it's okay
        m_Values = (char*)realloc(m_Values, newSize);
        if (m_Values) {
            m_buffSize = newSize;
        } else {
            m_buffSize = 0;
        }
        return;
    }

    //---------------------------------------------------------------------------------------
    void string_q::clear(void) {
        if (m_Values)
            if (m_Values != nullStr)
                delete [] m_Values;
        m_nValues     = 0;
        m_buffSize    = 0;
        m_Values      = nullStr;
    }

    //---------------------------------------------------------------------------------------
    void string_q::initialize() {
        m_nValues     = 0;
        m_buffSize    = 0;
        m_Values      = nullStr;
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(const char* str) const {
        return strcmp(m_Values, str);
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const char* str) const {
        string_q compared = m_Values;
        string_q comparing = str;
        return compared.substr(pos,len).compare(comparing);
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const char* str, size_t n) const {
        string_q compared = m_Values;
        string_q comparing = str;
        return compared.substr(pos,len).compare(comparing.substr(n));
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(const string_q& str) const {
        return strcmp(c_str(), str.c_str());
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const string_q& str) const {
        string_q compared = m_Values;
        string_q comparing = str.c_str();
        return compared.substr(pos,len).compare(comparing);
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const string_q& str, size_t subpos, size_t sublen) const {
        string_q compared = c_str();
        string_q comparing = str.c_str();
        return compared.substr(pos,len).compare(comparing.substr(subpos,sublen));
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(const char *str, size_t pos) const {
        const char *f = strstr(m_Values, str);
        if (f)
            return size_t(f-m_Values);
        return NOPOS;
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(char ch, size_t pos) const {
        char *f = strchr(m_Values, ch);
        if (f)
            return size_t(f-m_Values);
        return NOPOS;
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(const string_q& str, size_t pos) const {
        char *f = strstr(m_Values, str.c_str());
        if (f)
            return size_t(f-m_Values);
        return NOPOS;
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(const char* s, size_t pos, size_t n) const {
        return find(s,pos);
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::rfind(char ch) const {
        char *f = strrchr(m_Values, ch);
        return (f ? size_t(f-m_Values) : NOPOS);
    }

    //--------------------------------------------------------------------
    char string_q::operator[](size_t index) const {
        return at(index);
    }

    //---------------------------------------------------------------------------------------
    const char& string_q::at(size_t index) const {
        ASSERT(index >= 0);
        ASSERT(index < length());
        return m_Values[index];
    }

#endif

    //---------------------------------------------------------------------------------------
    string_q nextTokenClear(string_q& line, char delim, bool doClear) {
        string_q ret;
        size_t find = line.find(delim);
        if (find!=NOPOS) {
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
        return (haystack.find(needle) != NOPOS);
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
        if (f != NOPOS)
            target = (target.substr(0, f) + string_q(with.c_str()) + target.substr(f + what.length()));
    }

    //---------------------------------------------------------------------------------------
    void replaceAll(string_q& target, const string_q& what, const string_q& with) {
        if (what.empty())
            return;

        if (with.find(what) != NOPOS) {
            // may cause endless recursions so do it in two steps instead
            replaceAll(target, what, "~*~``");
            replaceAll(target, "~*~``", with);
            return;
        }

        size_t f = target.find(what);
        while (f != NOPOS) {
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

#ifdef NATIVE
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
#else
    //---------------------------------------------------------------------------------------
    void reverse(string_q& target) {
        size_t i,j;
        size_t n = target.length();
        char *t = (char *)target.c_str();
        for ( i = 0, j = n-1 ; i < n/2; i++, j-- ) {
            char tmp = t[i];
            t[i] = t[j];
            t[j] = tmp;
        }
    }
#endif

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
#ifdef _DEBUG
        bool start = contains(in, "<"+field+">");
        bool stop  = contains(in, "</"+field+">");
        if (start != stop)
        {
            string_q out = in;
            replaceAll(out, "<", "&lt;");
            replaceAll(out, ">", "&gt;");
            printf("%s", (string_q("<h3>'") + field + "' not found in '" + out + "'<h3><br>").c_str());
        }
#endif
        string_q f1 = "</" + field + ">";
        string_q ret = in.substr(0,in.find(f1));

        string_q f2 = "<" + field + ">";
        ret = ret.substr(ret.find(f2)+f2.length());

        replace(in, f2 + ret + f1, "");

        if (ret.empty())
            ret = defVal;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    string_q snagField(const string_q& in, const string_q& field, const string_q& defVal) {
#ifdef _DEBUG
        bool start = contains(in, "<"+field+">");
        bool stop  = contains(in, "</"+field+">");
        if (start != stop)
        {
            string_q out = in;
            replaceAll(out, "<", "&lt;");
            replaceAll(out, ">", "&gt;");
            printf("%s", (string_q("<h3>'") + field + "' not found in '" + out + "'<h3><br>").c_str());
        }
#endif
        string_q f = "</" + field + ">";
        string_q ret = in.substr(0,in.find(f));

        replace(f, "</", "<");
        ret = ret.substr(ret.find(f)+f.length());

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
