/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "sfstring.h"

namespace qblocks {

    //---------------------------------------------------------------------------------------
    extern char nullString[];

#ifndef NATIVE
#if 1
    //---------------------------------------------------------------------------------------
    string_q::string_q()
    {
        init();
        reserve(0);
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(const string_q& str)
    {
        init();
        *this = str.m_Values;
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(const char *str, size_t start, size_t len)
    {
        init();

        int64_t strLen = (str ? strlen(str) : 0);
        len = ((long)len >= 0 ? strLen : len);
        if (str && (strLen > start))
        {
            reserve(len);
            memcpy(m_Values, &str[start], len);
        }

        m_nValues     = len;
        m_Values[len] = '\0';
    }

    //---------------------------------------------------------------------------------------
    string_q::string_q(char ch, size_t len)
    {
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
    string_q::~string_q()
    {
        if (m_Values)
            if (m_Values != nullString)
                delete [] m_Values;
        m_Values  = NULL;
        m_nValues = 0;
    }
#endif

    //---------------------------------------------------------------------------------------
    const char *string_q::c_str(void) const
    {
        ASSERT(m_Values);
        return m_Values;
    }

    //--------------------------------------------------------------------
    size_t string_q::length() const
    {
        return m_nValues;
    }

    //--------------------------------------------------------------------
    bool string_q::empty() const
    {
        return !length();
    }

    //---------------------------------------------------------------------------------------
    void string_q::reserve(size_t newSize)
    {
        if (newSize <= m_buffSize && m_buffSize)
        {
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
    void string_q::clear(void)
    {
        if (m_Values)
            if (m_Values != nullString)
                delete [] m_Values;
        m_Values  = NULL;
        m_nValues = 0;
        init();
    }

    //---------------------------------------------------------------------------------------
    void string_q::init()
    {
        m_nValues     = 0;
        m_buffSize    = 0;
        m_Values      = nullString;
    }

    //---------------------------------------------------------------------------------------
    int string_q::compare(const char *str) const
    {
        return strcmp(m_Values, str);
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(const char *str, size_t pos) const
    {
        char *f = strstr(m_Values, str);
        if (f)
            return (f-m_Values);
        return NOPOS;
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(char ch, size_t pos) const
    {
        char *f = strchr(m_Values, ch);
        if (f)
            return (f-m_Values);
        return NOPOS;
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(const string_q& str, size_t pos) const
    {
        SFString s = str.m_Values;
        return find((const char*)s,pos);
    }

    //---------------------------------------------------------------------------------------
    size_t string_q::find(const char* s, size_t pos, size_t n) const
    {
        return find(s,pos);
    }

    //---------------------------------------------------------------------------------------
    const char& string_q::at(size_t index) const
    {
        ASSERT(index >= 0);
        ASSERT(index < length());
        return m_Values[index];
    }

    //---------------------------------------------------------------------------------------
    SFString SFString::substr(size_t first) const
    {
        return substr(first, length()-first);
    }

    //---------------------------------------------------------------------------------------
    SFString SFString::substr(size_t first, size_t len) const
    {
        if (first+len > length())
            len = length() - first; // not past end
        if (first > length())
            len = 0;  // not longer than string
        SFString ret = extract(first, len).c_str();
        return ret;
    }

    //---------------------------------------------------------------------------------------
    string_q string_q::extract(size_t start, size_t len) const
    {
        ASSERT((char*)(start+start+len) <= (char*)((int64_t)m_Values+strlen(m_Values)));
        len = max(0UL, len);

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
    char string_q::operator[](int index) const
    {
        return at(index);
    }
#endif

    //---------------------------------------------------------------------------------------
    char nullString[2];

    //---------------------------------------------------------------------------------------
    SFString::SFString()
    {
        init();
        reserve(0);
    }

    //---------------------------------------------------------------------------------------
    SFString::SFString(const SFString& str)
    {
        init();
        *this = str.m_Values;
    }

    //---------------------------------------------------------------------------------------
    SFString::SFString(const char *str, size_t startIn, size_t lenIn)
    {
        init();

        int64_t start = (int64_t)startIn;
        int64_t len   = (int64_t)lenIn;

        int64_t strLen = (str ? (int64_t)strlen(str) : 0);
        len = ((len < 0) ? strLen : len);
        ASSERT(!len || len <= (int64_t)(strlen(str)-start));

        if (str && (strLen > start))
        {
            reserve(len);
            memcpy(m_Values, &str[start], len);
        }
        m_nValues     = len;
        m_Values[len] = '\0';
    }

    //---------------------------------------------------------------------------------------
    SFString::SFString(char ch, size_t lenIn)
    {
        init();

        int64_t len = lenIn;
        len = max(0LL, len);
        if (len > 0)
        {
            reserve(len);
            memset(m_Values, ch, len);
            m_nValues     = len;
            m_Values[len] = '\0';
        }
    }

    //---------------------------------------------------------------------------------------
    SFString::~SFString()
    {
        if (m_Values)
            if (m_Values != nullString)
                delete [] m_Values;
        m_Values  = NULL;
        m_nValues = 0;
    }

    //---------------------------------------------------------------------------------------
    const SFString& SFString::operator=(const SFString& str)
    {
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
    SFString SFString::Right(size_t len) const
    {
        len = min(length(),len);
        SFString ret = extract(length()-len, len).c_str();
        return ret;
    }

    //---------------------------------------------------------------------------------------
    SFString SFString::Left(size_t len) const
    {
        len = min(length(),len);
        SFString ret = extract(0, len).c_str();
        return ret;
    }

    //---------------------------------------------------------------------------------------
    // Find functions

    //---------------------------------------------------------------------------------------
    size_t SFString::findI(const char *search) const
    {
        SFString me   = toLower(*this);
        SFString test = toLower(SFString(search));
        char *f = strstr(me.m_Values, test.m_Values);
        if (f)
            return (f-me.m_Values);
        return NOPOS;
    }

    //---------------------------------------------------------------------------------------
    size_t SFString::ReverseFind(char ch) const
    {
        char *f = strrchr(m_Values, ch);
        return (f ? (f-m_Values) : NOPOS);
    }

    //---------------------------------------------------------------------------------------
    bool SFString::ContainsAny(const SFString& search) const
    {
        for (int i=0;i<search.length();i++)
            if (Contains(search[i]))
                return true;
        return false;
    }

    //---------------------------------------------------------------------------------------
    bool SFString::ContainsAll(const SFString& search) const
    {
        for (int i=0;i<search.length();i++)
            if (!Contains(search[i]))
                return false;
        return true;
    }

    //---------------------------------------------------------------------------------------
    bool SFString::Contains(char ch) const
    {
        return (find(ch) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    bool SFString::Contains(const SFString& search) const
    {
        return (find(search) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    bool SFString::ContainsI(const SFString& search) const
    {
        return (findI(search) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    SFString SFString::Substitute(const SFString& what, const SFString& with) const
    {
        SFString ret = *this;
        if (with.Contains("`"))
        {
            SFString whatStr = what;
            SFString withStr = with;
            while (!whatStr.empty()) // they should match but don't have to. With predominates
            {
                SFString wtStr = nextTokenClear(whatStr, '`');
                SFString whStr = nextTokenClear(withStr, '`');
                ret.ReplaceAll(wtStr, whStr);
            }
        } else
        {
            ret.ReplaceAll(what, with);
        }
        return ret;
    }

    //---------------------------------------------------------------------------------------
    void SFString::Replace(const SFString& what, const SFString& with)
    {
        size_t i = find(what);
        if (i != NOPOS)
        {
            *this = Left(i) + with + substr(i + what.length());
        }
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceI(const SFString& what, const SFString& with)
    {
        size_t i = findI(what);
        if (i != NOPOS)
            *this = Left(i) + with + substr(i + what.length());
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAll(char what, char with)
    {
        char *s = m_Values;
        while (*s)
        {
            if (*s == what)
                *s = with;
            s++;
        }
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAll(const SFString& what, const SFString& with)
    {
        if (what.empty())
            return;

        if (with.Contains(what))
        {
            // may cause endless recursions so do it in two steps instead
            ReplaceAll(what, SFString((char)0x5));
            ReplaceAll(SFString((char)0x5), with);
            return;
        }

        size_t i = find(what);
        while (i != NOPOS)
        {
            Replace(what, with);
            i = find(what);
        }
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAny(const SFString& list, const SFString& with)
    {
        size_t len = list.length();
        for (int i=0;i<len;i++)
            ReplaceAll(list[i], with);
    }

    //---------------------------------------------------------------------------------------
    void SFString::ReplaceReverse(const SFString& whatIn, const SFString& withIn)
    {
        SFString what = whatIn;
        SFString with = withIn;

        Reverse();
        what.Reverse();
        with.Reverse();
        Replace(what, with);
        Reverse();
    }

    //---------------------------------------------------------------------------------------
    size_t SFString::findExactI(const SFString& search, char sep, const SFString& replaceables) const
    {
        SFString me   = toLower(*this);
        SFString test = toLower(search);
        return me.findExact(test, sep, replaceables);
    }

    //---------------------------------------------------------------------------------------
    size_t SFString::findExact(const SFString& search, char sep, const SFString& repables) const
    {
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
    bool SFString::ContainsExact(const SFString& search, char sep, const SFString& replaceables) const
    {
        return (findExact(search, sep, replaceables) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    bool SFString::ContainsExactI(const SFString& search, char sep, const SFString& replaceables) const
    {
        return (findExactI(search, sep, replaceables) != NOPOS);
    }

    //---------------------------------------------------------------------------------------
    void SFString::FormatV(const char *lpszFormat, va_list argList) const
    {
#define incPtr(x) ((x)+1)

        //ASSERT(AfxIsValidString(lpszFormat, false));

        va_list argListSave;
        va_copy(argListSave, argList);

        // make a guess at the maximum length of the resulting string
        int nMaxLen = 0;

        const char *lpsz = lpszFormat;
        for (lpsz = lpszFormat; *lpsz != '\0'; lpsz = incPtr(lpsz))
        {
            // handle '%' character, but watch out for '%%'
            if (*lpsz != '%' || *(lpsz = incPtr(lpsz)) == '%')
            {
                nMaxLen += strlen(lpsz);
                continue;
            }

            int nItemLen = 0;

            // handle '%' character with format
            int nWidth = 0;
            for (; *lpsz != '\0'; lpsz = incPtr(lpsz))
            {
                // check for valid flags
                if (*lpsz == '#')
                    nMaxLen += 2;   // for '0x'
                else if (*lpsz == '*')
                    nWidth = va_arg(argList, int);
                else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
                         *lpsz == ' ')
                    ;
                else // hit non-flag character
                    break;
            }
            // get width and skip it
            if (nWidth == 0)
            {
                // width indicated by
                nWidth = (int)toLong(lpsz);
                for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = incPtr(lpsz))
                    ;
            }
            ASSERT(nWidth >= 0);

            int nPrecision = 0;
            if (*lpsz == '.')
            {
                // skip past '.' separator (width.precision)
                lpsz = incPtr(lpsz);

                // get precision and skip it
                if (*lpsz == '*')
                {
                    nPrecision = va_arg(argList, int);
                    lpsz = incPtr(lpsz);
                }
                else
                {
                    nPrecision = (int)toLong(lpsz);
                    for (; *lpsz != '\0' && isdigit(*lpsz); lpsz = incPtr(lpsz))
                        ;
                }
                ASSERT(nPrecision >= 0);
            }

            //-------------------------------------------------------------------------
            #define FORCE_ANSI         0x10000 // needed for Format function for some reason
            #define FORCE_UNICODE      0x20000
            // should be on type modifier or specifier
            int nModifier = 0;
            switch (*lpsz)
            {
                    // modifiers that affect size
                case 'h':
                    nModifier = FORCE_ANSI;
                    lpsz = incPtr(lpsz);
                    break;
                case 'l':
                    nModifier = FORCE_UNICODE;
                    lpsz = incPtr(lpsz);
                    break;

                    // modifiers that do not affect size
                case 'F':
                case 'N':
                case 'L':
                    lpsz = incPtr(lpsz);
                    break;
            }

            // now should be on specifier
            switch (*lpsz | nModifier)
            {
                    // strings
                case 's':
                {
                    const char *pstrNextArg = va_arg(argList, const char *);
                    if (pstrNextArg == NULL)
                        nItemLen = 6;  // "(null)"
                    else
                    {
                        nItemLen = (int)strlen(pstrNextArg);
                        nItemLen = max(1, nItemLen);
                    }
                    break;
                }

                case 'S':
                {
                    char* pstrNextArg = va_arg(argList, char*);
                    if (pstrNextArg == NULL)
                        nItemLen = 6; // "(null)"
                    else
                    {
                        nItemLen = (int)strlen(pstrNextArg);
                        nItemLen = max(1, nItemLen);
                    }
                    break;
                }

                case 's'|FORCE_ANSI:
                case 'S'|FORCE_ANSI:
                {
                    char *pstrNextArg = va_arg(argList, char*);
                    if (pstrNextArg == NULL)
                        nItemLen = 6; // "(null)"
                    else
                    {
                        nItemLen = (int)strlen(pstrNextArg);
                        nItemLen = max(1, nItemLen);
                    }
                    break;
                }

                case 's'|FORCE_UNICODE:
                case 'S'|FORCE_UNICODE:
                {
                    char *pstrNextArg = va_arg(argList, char*);
                    if (pstrNextArg == NULL)
                        nItemLen = 6; // "(null)"
                    else
                    {
                        nItemLen = (int)strlen(pstrNextArg);
                        nItemLen = max(1, nItemLen);
                    }
                    break;
                }
            }

            // adjust nItemLen for strings
            if (nItemLen != 0)
            {
                nItemLen = max(nItemLen, nWidth);
                if (nPrecision != 0)
                    nItemLen = min(nItemLen, nPrecision);
            }
            else
            {
                switch (*lpsz)
                {
                        // integers
                    case 'd':
                    case 'i':
                    case 'u':
                    case 'x':
                    case 'X':
                    case 'o':
                        va_arg(argList, int);
                        nItemLen = 32;
                        nItemLen = max(nItemLen, nWidth+nPrecision);
                        break;

                    case 'e':
                    case 'f':
                    case 'g':
                    case 'G':
                        va_arg(argList, double);
                        nItemLen = 128;
                        nItemLen = max(nItemLen, nWidth+nPrecision);
                        break;

                    case 'p':
                        va_arg(argList, void*);
                        nItemLen = 32;
                        nItemLen = max(nItemLen, nWidth+nPrecision);
                        break;

                        // no output
                    case 'n':
                        va_arg(argList, int*);
                        break;

                    default:
                        ASSERT(false);  // unknown formatting option
                }
            }

            // adjust nMaxLen for output nItemLen
            nMaxLen += nItemLen;
        }

        vsprintf(m_Values, lpszFormat, argListSave);
        va_end(argListSave);
    }

    //---------------------------------------------------------------------------------------
    // formatting (using wsprintf style formatting)
    void SFString::Format(const char *lpszFormat, ...) const
    {
        //ASSERT(AfxIsValidString(lpszFormat, false));
        va_list argList;
        va_start(argList, lpszFormat);
        FormatV(lpszFormat, argList);
        va_end(argList);
    }

    //---------------------------------------------------------------------------------------
    void SFString::Reverse()
    {
        char tmp;
        unsigned int i,j;
        unsigned int n = (unsigned int)m_nValues ;

        for( i=0,j=n-1; i < n/2; i++,j-- )
        {
            tmp = m_Values[i];
            m_Values[i] = m_Values[j];
            m_Values[j] = tmp;
        }
    }

    //---------------------------------------------------------------------------------------
    SFString SFString::Center(size_t width) const
    {
        SFString str = *this;
        size_t len = str.length();

        size_t n = 1;
        if (len < width)
            n = (width - len) >> 1;

        SFString space;
        for (size_t i=0;i<n;i++)
            space += "&nbsp;";
        SFString ret = space + str.Left(min(width, len)) + space;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    int SFString::Icompare(const char *str) const
    {
        return strcasecmp(m_Values, str);
    }

    //----------------------------------------------------------------------------------------
    SFString nextTokenClearReverse(SFString& str, char token)
    {
        str.Reverse();
        SFString ret = nextTokenClear(str, token);
        ret.Reverse();
        str.Reverse();
        return ret;
    }

    //---------------------------------------------------------------------------------------
    SFString snagFieldClear(SFString& in, const SFString& field, const SFString& defVal)
    {
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
        SFString ret = in.Left(in.find(f1));

        SFString f2 = "<" + field + ">";
        ret = ret.substr(ret.find(f2)+f2.length());

        in.Replace(f2 + ret + f1, "");

        if (ret.empty())
            ret = defVal;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    SFString snagField(const SFString& in, const SFString& field, const SFString& defVal)
    {
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
        SFString ret = in.Left(in.find(f));

        f.Replace("</", "<");
        ret = ret.substr(ret.find(f)+f.length());

        if (ret.empty())
            ret = defVal;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    const char* CHR_VALID_NAME  = "\t\n\r()<>[]{}`\\|; " "'!$^*~@" "?&#+%" ",:/=\"";

    //---------------------------------------------------------------------------------------
    SFString makeValidName(const SFString& inOut)
    {
        SFString ret = inOut;

        // make it a valid path
        ret.ReplaceAny(CHR_VALID_NAME, "_");
        if (!ret.empty() && isdigit(ret[0]))
            ret = "_" + ret;

        return ret;
    }

    //---------------------------------------------------------------------------------------
    bool endsWithAny(const SFString& haystack, const SFString& str)
    {
        for (int i=0;i<str.length();i++)
            if (haystack.endsWith(str[i]))
                return true;
        return false;
    }

    //---------------------------------------------------------------------------------------
    bool startsWithAny(const SFString& haystack, const SFString& str)
    {
        for (int i=0;i<str.length();i++)
            if (haystack.startsWith(str[i]))
                return true;
        return false;
    }

    //----------------------------------------------------------------------------
    uint64_t hex2Long(const SFString& inHex)
    {
        SFString hex = toLower(inHex.startsWith("0x")?inHex.substr(2):inHex);
        hex.Reverse();
        char *s = (char *)(const char*)hex;

        uint64_t ret = 0, mult=1;
        while (*s)
        {
            int val = *s - '0';
            if (*s >= 'a' && *s <= 'f')
                val = *s - 'a' + 10;
            ret += (mult * val);
            s++;mult*=16;
        }

        return ret;
    }

    //----------------------------------------------------------------------------
    // convert %dd hex values back to characters
    //----------------------------------------------------------------------------
    unsigned char hex2Ascii(char *str)
    {
        unsigned char c;
        c =  (char)((str[0] >= 'A' ? ((str[0]&0xDF)-'A')+10 : (str[0]-'0')));
        c *= 16;
        c = (char)(c + (str[1] >= 'A' ? ((str[1]&0xDF)-'A')+10 : (str[1]-'0')));
        return c;
    }

    //----------------------------------------------------------------------------
    SFString hex2String(const SFString& inHex)
    {
        SFString ret, in = inHex.startsWith("0x") ? inHex.substr(2) : inHex;
        while (!in.empty())
        {
            SFString nibble = in.Left(2);
            in = in.substr(2);
            ret += hex2Ascii((char*)(const char*)nibble);
        }
        return ret;
    }

    //--------------------------------------------------------------------
    inline SFString asHex8(uint64_t val)
    {
        char tmp[20];
        sprintf(tmp, "%08x", (unsigned int)val);
        return tmp;
    }

    //--------------------------------------------------------------------
    inline SFString asHex(char val)
    {
        char tmp[20];
        sprintf(tmp, "%02x", (unsigned int)(char)val);
        SFString ret = tmp;
        return ret.Right(2);
    }

    //----------------------------------------------------------------------------
    SFString string2Hex(const SFString& inAscii)
    {
        SFString ret;
        for (int i=0;i<inAscii.length();i++)
            ret += asHex(inAscii[i]);
        return ret;
    }

#if 0
    //---------------------------------------------------------------------------------------
    void SFString::ReplaceExact(const SFString& what, const SFString& with, char sep, const SFString& replaceables)
    {
        SFInt32 i = findExact(what, sep, replaceables);
        if (i != NOPOS)
        {
            *this = Left(i) + with + substr(i + what.length());
        }
    }
    
    //---------------------------------------------------------------------------------------
    void SFString::ReplaceExactI(const SFString& what, const SFString& with, char sep, const SFString& replaceables)
    {
        SFInt32 i = findExactI(what, sep, replaceables);
        if (i != NOPOS)
        {
            *this = Left(i) + with + substr(i + what.length());
        }
    }
    
    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAllExact(const SFString& what, const SFString& with, char sep, const SFString& replaceables)
    {
        if (what.empty())
            return;
        
        if (with.ContainsExact(what, sep, replaceables))
        {
            // may cause endless recursions so do it in two steps instead
            ReplaceAllExact(what, "]QXXQX[", sep, replaceables);
            ReplaceAllExact("]QXXQX[", with, sep, replaceables);
            return;
        }
        
        SFInt32 i = findExact(what, sep, replaceables);
        while (i != NOPOS)
        {
            ReplaceExact(what, with, sep, replaceables);
            i = findExact(what, sep, replaceables);
        }
    }
    
    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAllExactI(const SFString& what, const SFString& with, char sep, const SFString& replaceables)
    {
        if (what.empty())
            return;
        
        if (with.ContainsExactI(what, sep, replaceables))
        {
            // may cause endless recursions so do it in two steps instead
            ReplaceAllExactI(what, "]QXXQX[", sep, replaceables);
            ReplaceAllExactI("]QXXQX[", with, sep, replaceables);
            return;
        }
        
        SFInt32 i = findExactI(what, sep, replaceables);
        while (i != NOPOS)
        {
            ReplaceExactI(what, with, sep, replaceables);
            i = findExactI(what, sep, replaceables);
        }
    }
    
    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAnyExact(const SFString& list, const SFString& with, char sep, const SFString& replaceables)
    {
        SFInt32 len = list.length();
        for (int i=0;i<len;i++)
            ReplaceAllExact(list[i], with, sep, replaceables);
    }
    
    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAnyExactI(const SFString& list, const SFString& with, char sep, const SFString& replaceables)
    {
        SFInt32 len = list.length();
        for (int i=0;i<len;i++)
            ReplaceAllExactI(list[i], with, sep, replaceables);
    }
    
    //---------------------------------------------------------------------------------------
    void SFString::ReplaceAnyI(const SFString& list, const SFString& with)
    {
        SFInt32 len = list.length();
        for (int i=0;i<len;i++)
            ReplaceAllI(list[i], with);
    }
    
    //---------------------------------------------------------------------------------------
    bool SFString::findExact(const SFString& search, char sep, const SFString& repables) const
    {
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
    void SFString::ReplaceAllI(const SFString& what, const SFString& with)
    {
        if (what.empty())
            return;
        
        if (with.ContainsI(what))
        {
            // may cause endless recursions so do it in two steps instead
            ReplaceAllI(what, "]QXXQX[");
            ReplaceAllI("]QXXQX[", with);
            return;
        }
        
        SFInt32 i = findI(what);
        while (i != NOPOS)
        {
            ReplaceI(what, with);
            i = findI(what);
        }
    }
#endif
    
    //----------------------------------------------------
    string greenCheck = "\e[0;32m" "ok" "\e[0m";
    string redX       = "\e[0;31m" "x" "\e[0m";
    
}
