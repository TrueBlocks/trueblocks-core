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
#include "sfstring.h"
#include "conversions.h"

namespace qblocks {

    //---------------------------------------------------------------------------------------
    extern char nullString[];

#ifndef NATIVE
    //---------------------------------------------------------------------------------------
    string_q::string_q() {
        init();
        reserve(0);
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(const string_q& str) {
        init();
        *this = str.m_Values;
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(const char *str, size_t start, size_t len) {
        init();

        size_t strLen = (str ? strlen(str) : 0);
        len = (len == NOPOS ? strLen : len);
        if (str && (strLen > start))
        {
            reserve(len);
            memcpy(m_Values, &str[start], len);
        }

        m_nValues     = len;
        m_Values[len] = '\0';
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(char ch, size_t len) {
        init();

        if (((long)len) > 0)
        {
            reserve(len);
            memset(m_Values, ch, len);
            m_nValues     = len;
            m_Values[len] = '\0';
        }
    }

    //---------------------------------------------------------------------------------------
    string_q::~string_q() {
        if (m_Values)
            if (m_Values != nullString)
                delete [] m_Values;
        m_Values  = NULL;
        m_nValues = 0;
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
            m_Values[0] = '\0';
            return;
        }

        // clear current buffer and deallocate
        clear();
        if (newSize == NOPOS || newSize == 0)
            return;

        // make sure we are not dropping memory
        ASSERT(m_Values == nullString);

#define GROW_SIZE 16
        m_Values      = new char[newSize+GROW_SIZE+1];
        m_Values[0]   = '\0';
        m_nValues     = 0;
        m_buffSize    = newSize + GROW_SIZE;
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
            if (m_Values != nullString)
                delete [] m_Values;
        m_nValues     = 0;
        m_buffSize    = 0;
        m_Values      = nullString;
    }

    //---------------------------------------------------------------------------------------
    void string_q::init() {
        m_nValues     = 0;
        m_buffSize    = 0;
        m_Values      = nullString;
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(const char* str) const {
        return strcmp(m_Values, str);
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const char* str) const {
        SFString compared = m_Values;
        SFString comparing = str;
        return compared.substr(pos,len).compare(comparing);
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const char* str, size_t n) const {
        SFString compared = m_Values;
        SFString comparing = str;
        return compared.substr(pos,len).compare(comparing.substr(n));
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(const string_q& str) const {
        return strcmp(m_Values, str.c_str());
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const string_q& str) const {
        SFString compared = m_Values;
        SFString comparing = str.c_str();
        return compared.substr(pos,len).compare(comparing);
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(size_t pos, size_t len, const string_q& str, size_t subpos, size_t sublen) const {
        SFString compared = m_Values;
        SFString comparing = str.c_str();
        return compared.substr(pos,len).compare(comparing.substr(subpos,sublen));
    }
    //---------------------------------------------------------------------------------------
    size_t string_q::find(const char *str, size_t pos) const {
        char *f = strstr(m_Values, str);
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
        SFString s = str.m_Values;
        return find((const char*)s,pos);
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(const char* s, size_t pos, size_t n) const {
        return find(s,pos);
    }

    //---------------------------------------------------------------------------------------
    const char& string_q::at(size_t index) const {
        ASSERT(index >= 0);
        ASSERT(index < length());
        return m_Values[index];
    }

#ifdef THE_SWITCH
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
        string_q ret = extract(first, len).c_str();
        return ret;
    }
#else
    //---------------------------------------------------------------------------------------
    SFString SFString::substr(size_t first) const {
        return substr(first, length()-first);
    }

    //---------------------------------------------------------------------------------------
    SFString SFString::substr(size_t first, size_t len) const {
        if (first+len > length())
            len = length() - first; // not past end
        if (first > length())
            len = 0;  // not longer than string
        SFString ret = extract(first, len).c_str();
        return ret;
    }
#endif

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

    //--------------------------------------------------------------------
    char string_q::operator[](size_t index) const {
        return at(index);
    }

#ifdef THE_SWITCH
    //---------------------------------------------------------------------------------------
    const string_q& string_q::operator=(const string_q& str) noexcept {
        if (m_Values == str.m_Values) // do not change this line
            return *this;
        size_t len = str.length();
        reserve(len);
        memcpy(m_Values, str.m_Values, len);
        m_nValues     = len;
        m_Values[len] = '\0';
        return *this;
    }
#else
    //---------------------------------------------------------------------------------------
    const SFString& SFString::operator=(const SFString& str) {
        if (m_Values == str.m_Values) // do not change this line
            return *this;
        size_t len = str.length();
        reserve(len);
        memcpy(m_Values, str.m_Values, len);
        m_nValues     = len;
        m_Values[len] = '\0';
        return *this;
    }
#endif
#endif

    //---------------------------------------------------------------------------------------
    char nullString[2];

    //---------------------------------------------------------------------------------------
    SFString::SFString() {
        init();
        reserve(0);
    }

    //---------------------------------------------------------------------------------------
    SFString::SFString(const SFString& str) {
        init();
        *this = str.m_Values;
    }

    //---------------------------------------------------------------------------------------
    SFString::SFString(const char *str, size_t startIn, size_t lenIn) {
        init();

        int64_t start = (int64_t)startIn;
        int64_t len   = (int64_t)lenIn;

        int64_t strLen = (str ? (int64_t)strlen(str) : 0);
        len = ((len < 0) ? strLen : len);
        ASSERT(!len || len <= (int64_t)(strlen(str)-start));

        if (str && (strLen > start))
        {
            reserve((size_t)len);
            memcpy(m_Values, &str[start], (size_t)len);
        }
        m_nValues     = (size_t)len;
        m_Values[len] = '\0';
    }

    //---------------------------------------------------------------------------------------
    SFString::SFString(char ch, size_t lenIn) {
        init();

        int64_t len = (int64_t)lenIn;
        len = max((int64_t)0, len);
        if (len > 0)
        {
            reserve((size_t)len);
            memset(m_Values, ch, (size_t)len);
            m_nValues     = (size_t)len;
            m_Values[len] = '\0';
        }
    }

    //---------------------------------------------------------------------------------------
    SFString::~SFString() {
        if (m_Values)
            if (m_Values != nullString)
                delete [] m_Values;
        m_Values  = NULL;
        m_nValues = 0;
    }

    //---------------------------------------------------------------------------------------
    // Find functions

    //---------------------------------------------------------------------------------------
    size_t SFString::findI(const char *search) const {
        SFString me   = toLower(*this);
        SFString test = toLower(SFString(search));
        char *f = strstr(me.m_Values, test.m_Values);
        if (f)
            return size_t(f-me.m_Values);
        return NOPOS;
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::rfind(char ch) const {
        char *f = strrchr(m_Values, ch);
        return (f ? size_t(f-m_Values) : NOPOS);
    }

    //---------------------------------------------------------------------------------------
    bool SFString::ContainsAny(const SFString& search) const {
        for (size_t i=0;i<search.length();i++)
            if (Contains(search[i]))
                return true;
        return false;
    }

    //---------------------------------------------------------------------------------------
    bool SFString::ContainsAll(const SFString& search) const {
        for (size_t i=0;i<search.length();i++)
            if (!Contains(search[i]))
                return false;
        return true;
    }

    //---------------------------------------------------------------------------------------
    bool SFString::Contains(char ch) const {
        return (find(ch) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    bool SFString::Contains(const SFString& search) const {
        return (find(search) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    bool SFString::ContainsI(const SFString& search) const {
        return (findI(search) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    SFString SFString::Substitute(const SFString& what, const SFString& with) const {
        SFString ret = *this;
        if (with.Contains("`")) {
            SFString whatStr = what;
            SFString withStr = with;
            while (!whatStr.empty()) { // they should match but don't have to. With predominates
                SFString wtStr = nextTokenClear(whatStr, '`');
                SFString whStr = nextTokenClear(withStr, '`');
                ret.ReplaceAll(wtStr, whStr);
            }
        } else {
            ret.ReplaceAll(what, with);
        }
        return ret;
    }

    //---------------------------------------------------------------------------------------
    void SFString::Replace(const SFString& what, const SFString& with) {
        size_t i = find(what);
        if (i != NOPOS)
            *this = substr(0,i) + with + substr(i + what.length());
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceI(const SFString& what, const SFString& with) {
        size_t i = findI(what);
        if (i != NOPOS)
            *this = substr(0,i) + with + substr(i + what.length());
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAll(char what, char with) {
        char *s = m_Values;
        while (*s) {
            if (*s == what)
                *s = with;
            s++;
        }
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAll(const SFString& what, const SFString& with) {
        if (what.empty())
            return;

        if (with.Contains(what)) {
            // may cause endless recursions so do it in two steps instead
            ReplaceAll(what, SFString((char)0x5));
            ReplaceAll(SFString((char)0x5), with);
            return;
        }

        size_t i = find(what);
        while (i != NOPOS) {
            Replace(what, with);
            i = find(what);
        }
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAny(const SFString& list, const SFString& with) {
        size_t len = list.length();
        for (size_t i = 0 ; i < len ; i++)
            ReplaceAll(list[i], with);
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceReverse(const SFString& whatIn, const SFString& withIn) {
        SFString what = whatIn;
        SFString with = withIn;

        Reverse();
        what.Reverse();
        with.Reverse();
        Replace(what, with);
        Reverse();
    }

    //---------------------------------------------------------------------------------------
    size_t SFString::findExactI(const SFString& search, char sep, const SFString& replaceables) const {
        SFString me   = toLower(*this);
        SFString test = toLower(search);
        return me.findExact(test, sep, replaceables);
    }

    //---------------------------------------------------------------------------------------
    size_t SFString::findExact(const SFString& search, char sep, const SFString& repables) const {
        ASSERT(sep == '|' || sep == ';' || sep == ',' || sep == ' ');

        SFString sepStr(sep);

        // Surround the stringa with sep to handle boundary cases
        SFString tS = sepStr + *this  + sepStr;
        SFString qS = sepStr + search + sepStr;

        // we will replace everything but the separator
        SFString replaceables = repables;
        replaceables.Replace(sepStr, EMPTY);

        tS.ReplaceAny(replaceables, sepStr);

        return tS.find(qS);
    }

    //---------------------------------------------------------------------------------------
    void SFString::Reverse() {
        char tmp;
        unsigned int i,j;
        unsigned int n = (unsigned int)m_nValues ;

        for( i=0,j=n-1; i < n/2; i++,j-- ) {
            tmp = m_Values[i];
            m_Values[i] = m_Values[j];
            m_Values[j] = tmp;
        }
    }

    //---------------------------------------------------------------------------------------
    int SFString::Icompare(const char *str) const {
        return strcasecmp(m_Values, str);
    }

    //----------------------------------------------------------------------------------------
    SFString nextTokenClearReverse(SFString& str, char token) {
        str.Reverse();
        SFString ret = nextTokenClear(str, token);
        ret.Reverse();
        str.Reverse();
        return ret;
    }

    //---------------------------------------------------------------------------------------
    SFString snagFieldClear(SFString& in, const SFString& field, const SFString& defVal) {
#ifdef _DEBUG
        bool start = in.Contains("<"+field+">");
        bool stop  = in.Contains("</"+field+">");
        if (start != stop)
        {
            SFString out = in;
            out.ReplaceAll("<", "&lt;");
            out.ReplaceAll(">", "&gt;");
            printf("%s", (const char*)(SFString("<h3>'") + field + "' not found in '" + out + "'<h3><br>"));
        }
#endif
        SFString f1 = "</" + field + ">";
        SFString ret = in.substr(0,in.find(f1));

        SFString f2 = "<" + field + ">";
        ret = ret.substr(ret.find(f2)+f2.length());

        in.Replace(f2 + ret + f1, "");

        if (ret.empty())
            ret = defVal;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    SFString snagField(const SFString& in, const SFString& field, const SFString& defVal) {
#ifdef _DEBUG
        bool start = in.Contains("<"+field+">");
        bool stop  = in.Contains("</"+field+">");
        if (start != stop)
        {
            SFString out = in;
            out.ReplaceAll("<", "&lt;");
            out.ReplaceAll(">", "&gt;");
            printf("%s", (const char*)(SFString("<h3>'") + field + "' not found in '" + out + "'<h3><br>"));
        }
#endif
        SFString f = "</" + field + ">";
        SFString ret = in.substr(0,in.find(f));

        f.Replace("</", "<");
        ret = ret.substr(ret.find(f)+f.length());

        if (ret.empty())
            ret = defVal;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    const char* CHR_VALID_NAME  = "\t\n\r()<>[]{}`\\|; " "'!$^*~@" "?&#+%" ",:/=\"";

    //---------------------------------------------------------------------------------------
    SFString makeValidName(const SFString& inOut) {
        SFString ret = inOut;

        // make it a valid path
        ret.ReplaceAny(CHR_VALID_NAME, "_");
        if (!ret.empty() && isdigit(ret[0]))
            ret = "_" + ret;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    bool endsWithAny(const SFString& haystack, const SFString& str) {
        for (size_t i = 0 ; i < str.length() ; i++)
            if (haystack.endsWith(str[i]))
                return true;
        return false;
    }

    //---------------------------------------------------------------------------------------
    bool startsWithAny(const SFString& haystack, const SFString& str) {
        for (size_t i = 0 ; i < str.length() ; i++)
            if (haystack.startsWith(str[i]))
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
        char prev = 'x'; // not a space
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
    SFString toLower(const SFString& in) {
        SFString ret = in;
        if (ret.length()) {
            char *s = (char*)ret.c_str();
            while (*s) {
                *s = (char)tolower(*s);
                s++;
            }
        }
        return ret;
    }

    //--------------------------------------------------------------------
    SFString toUpper(const SFString& in) {
        SFString ret = in;
        if (ret.length()) {
            char *s = (char*)ret.c_str();
            while (*s) {
                *s = (char)toupper(*s);
                s++;
            }
        }
        return ret;
    }

    //--------------------------------------------------------------------
    SFString toProper(const SFString& in) {
        SFString ret = in;
        if (ret.length()) {
            char last='\0';
            char *s = (char*)ret.c_str();
            while (*s) {
                if (last == '_' || isWhiteSpace(last))
                    *s = (char)toupper(*s);

                else
                    *s = (char)tolower(*s);
                last = *s;
                s++;
            }
        }
//        ret.ReplaceAll("_", " ");
        return ret;
    }
}  // namespace qblocks
