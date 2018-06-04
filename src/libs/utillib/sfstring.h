#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "basetypes.h"

namespace qblocks {

    //--------------------------------------------------------------------
#define NOPOS ((size_t)-1)
    extern const char* CHR_VALID_NAME;

    //--------------------------------------------------------------------
    //#define NATIVE
#ifndef NATIVE
    class string_q {
    private:
        string_q  (long) {};
        string_q  (unsigned long) {};
        const string_q& operator=(long);

    protected:
        char  *m_Values;
        size_t m_nValues;
        size_t m_buffSize;

    public:
        string_q(void);
        string_q(const string_q& str);
        string_q(char ch, size_t reps=1);
        string_q(const char *str, size_t start=0, size_t len=NOPOS);
        ~string_q(void);

        void     clear    (void);
        size_t   length   (void) const;
        int      compare  (const char* s) const;
        int      compare  (size_t pos, size_t len, const char* s) const;
        int      compare  (size_t pos, size_t len, const char* s, size_t n) const;
        int      compare  (const string_q& str) const;
        int      compare  (size_t pos, size_t len, const string_q& str) const;
        int      compare  (size_t pos, size_t len, const string_q& str, size_t subpos, size_t sublen) const;
        void     reserve  (size_t n=0);
#ifdef NEW_CODE
        void     resize   (size_t n) { resize(n, '\0'); }
        void     resize   (size_t n, char c);
#endif
        bool     empty    (void) const;
        size_t   find     (const string_q& str, size_t pos=0) const;
        size_t   find     (const char* s, size_t pos=0) const;
        size_t   find     (const char* s, size_t pos, size_t n) const;
        size_t   find     (char c, size_t pos=0) const;
        string_q extract  (size_t first, size_t len) const;

        const char&    at       (size_t index) const;
        char     operator[](int index) const;

        const char    *c_str     (void) const;

        friend bool   operator==(const string_q& str1, const string_q& str2);
        friend bool   operator==(const string_q& str1, const char *str2);
        friend bool   operator==(const char *str1, const string_q& str2);

        friend bool   operator!=(const string_q& str1, const string_q& str2);
        friend bool   operator!=(const string_q& str1, const char *str2);
        friend bool   operator!=(const char *str1, const string_q& str2);

        friend bool   operator< (const string_q& str1, const string_q& str2);
        friend bool   operator< (const string_q& str1, const char *str2);
        friend bool   operator< (const char *str1, const string_q& str2);

        friend bool   operator<=(const string_q& str1, const string_q& str2);
        friend bool   operator<=(const string_q& str1, const char *str2);
        friend bool   operator<=(const char *str1, const string_q& str2);

        friend bool   operator> (const string_q& str1, const string_q& str2);
        friend bool   operator> (const string_q& str1, const char *str2);
        friend bool   operator> (const char *str1, const string_q& str2);

        friend bool   operator>=(const string_q& str1, const string_q& str2);
        friend bool   operator>=(const string_q& str1, const char *str2);
        friend bool   operator>=(const char *str1, const string_q& str2);

        //----------------------------------------------------------------------------------------------------
        friend ostream& operator<<(ostream &os, const string_q& x) {
            os << x.m_Values;
            return os;
        }

    protected:
        void    init      (void);
    };

    //--------------------------------------------------------------------
    inline bool operator==(const string_q& str1, const string_q& str2) {
        return str1.compare(str2.m_Values) == 0;
    }

    //--------------------------------------------------------------------
    inline bool operator==(const string_q& str1, const char *str2) {
        return str1.compare(str2) == 0;
    }

    //--------------------------------------------------------------------
    inline bool operator==(const char *str1, const string_q& str2) {
        return str2.compare(str1) == 0;
    }

    //--------------------------------------------------------------------
    inline bool operator!=(const string_q& str1, const string_q& str2) {
        return str1.compare(str2.m_Values) != 0;
    }

    //--------------------------------------------------------------------
    inline bool operator!=(const string_q& str1, const char *str2) {
        return str1.compare(str2) != 0;
    }

    //--------------------------------------------------------------------
    inline bool operator!=(const char *str1, const string_q& str2) {
        return str2.compare(str1) != 0;
    }

    //--------------------------------------------------------------------
    inline bool operator<(const string_q& str1, const string_q& str2) {
        return str1.compare(str2.m_Values) < 0;
    }

    //--------------------------------------------------------------------
    inline bool operator<(const string_q& str1, const char *str2) {
        return str1.compare(str2) < 0;
    }

    //--------------------------------------------------------------------
    inline bool operator<(const char *str1, const string_q& str2) {
        return str2.compare(str1) > 0;
    }

    //--------------------------------------------------------------------
    inline bool operator>(const string_q& str1, const string_q& str2) {
        return str1.compare(str2.m_Values) > 0;
    }

    //--------------------------------------------------------------------
    inline bool operator>(const string_q& str1, const char *str2) {
        return str1.compare(str2) > 0;
    }

    //--------------------------------------------------------------------
    inline bool operator>(const char *str1, const string_q& str2) {
        return str2.compare(str1) < 0;
    }

    //--------------------------------------------------------------------
    inline bool operator<=(const string_q& str1, const string_q& str2) {
        return str1.compare(str2.m_Values) <= 0;
    }

    //--------------------------------------------------------------------
    inline bool operator<=(const string_q& str1, const char *str2) {
        return str1.compare(str2) <= 0;
    }

    //--------------------------------------------------------------------
    inline bool operator<=(const char *str1, const string_q& str2) {
        return str2.compare(str1) >= 0;
    }

    //--------------------------------------------------------------------
    inline bool operator>=(const string_q& str1, const string_q& str2) {
        return str1.compare(str2.m_Values) >= 0;
    }

    //--------------------------------------------------------------------
    inline bool operator>=(const string_q& str1, const char *str2) {
        return str1.compare(str2) >= 0;
    }

    //--------------------------------------------------------------------
    inline bool operator>=(const char *str1, const string_q& str2) {
        return str2.compare(str1) <= 0;
    }

    //--------------------------------------------------------------------
    inline string_q toLower(const string_q& in) {
        string_q ret = in;
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
    inline string_q toUpper(const string_q& in) {
        string_q ret = in;
        if (ret.length()) {
            char *s = (char*)ret.c_str();
            while (*s) {
                *s = (char)toupper(*s);
                s++;
            }
        }
        return ret;
    }

#else
#define string_q std::string
#endif

    //--------------------------------------------------------------------
    class SFString : public string_q {
        SFString  (long) {};
        SFString  (unsigned long) {};
        const SFString& operator=(long);
    public:
        SFString  (void);
        SFString  (const SFString& str);
        SFString  (char ch, size_t reps=1);
        SFString  (const char *str, size_t start=0, size_t len=NOPOS);

        ~SFString (void);

        const SFString& operator=     (const SFString& str);

        const SFString& operator+=    (const SFString& str);
        const SFString& operator+=    (char ch);
        const SFString& operator+=    (const char *str);

        operator  const char *        (void) const;

        SFString        substr        (size_t first, size_t len) const;
        SFString        substr        (size_t first) const;

        void            Reverse       (void);
        int             Icompare      (const char *str) const;

        size_t   findI           (const char *search) const;
        size_t   ReverseFind     (char ch) const;
        size_t   findExact       (const SFString& search, char sep, const SFString& replaceables=CHR_VALID_NAME) const;
        size_t   findExactI      (const SFString& search, char sep, const SFString& replaceables=CHR_VALID_NAME) const;

        bool     Contains        (char search) const;
        bool     Contains        (const SFString& search) const;
        bool     ContainsI       (const SFString& search) const;
        bool     ContainsAll     (const SFString& search) const;
        bool     ContainsAny     (const SFString& search) const;
        bool     ContainsExact   (const SFString& search, char sep, const SFString& replaceables=CHR_VALID_NAME) const;
        bool     ContainsExactI  (const SFString& search, char sep, const SFString& replaceables=CHR_VALID_NAME) const;

        SFString Substitute      (const SFString& what, const SFString& with) const;

        void     Replace         (const SFString& what, const SFString& with);
        void     ReplaceI        (const SFString& what, const SFString& with);

        void     ReplaceAll      (      char      what,       char      with);
        void     ReplaceAll      (const SFString& what, const SFString& with);

        void     ReplaceAny      (const SFString& list, const SFString& with);
        void     ReplaceReverse  (const SFString& what, const SFString& with);

        bool     startsWith      (const SFString& str) const;
        bool     endsWith        (const SFString& str) const;

        friend SFString operator+(const SFString& str1, const SFString& str2);
        friend SFString operator+(const SFString& str1, const char *str2);
    };

    //--------------------------------------------------------------------
    extern bool     endsWithAny   (const SFString& haystack, const SFString& str);
    extern bool     startsWithAny (const SFString& haystack, const SFString& str);
    extern SFString makeValidName (const SFString& input);

    //--------------------------------------------------------------------
    inline bool isWhiteSpace(char c) {
        return (c == '\0' || c == ' ' || c == '\n' || c == '\r' || c == '\t');
    }

    //--------------------------------------------------------------------
    inline SFString::operator const char *() const {
        return m_Values;
    }

    //---------------------------------------------------------------------------------------
    inline SFString operator+(const SFString& str1, const SFString& str2) {
        size_t newLen = str1.length() + str2.length();
        SFString ret;
        ret.reserve(newLen);
        memcpy(ret.m_Values, str1.m_Values, str1.length());
        memcpy(ret.m_Values+str1.length(), str2.m_Values, str2.length());
        ret.m_nValues = newLen;
        ret.m_Values[newLen] = '\0';
        return ret;
    }

    //---------------------------------------------------------------------------------------
    inline const SFString& SFString::operator+=(const SFString& add) {
        if (add.length())
            *this = (*this + add);
        return *this;
    }

    //--------------------------------------------------------------------
    inline const SFString& SFString::operator+=(const char *str) {
        return operator+=(SFString(str));
    }

    //--------------------------------------------------------------------
    inline const SFString& SFString::operator+=(char ch) {
        return operator+=(SFString(ch));
    }

    //--------------------------------------------------------------------
    inline SFString operator+(const SFString& str1, const char *str2) {
        return operator+(str1, SFString(str2));
    }

    //--------------------------------------------------------------------
    inline SFString operator+(const char *str1, const SFString& str2) {
        return operator+(SFString(str1), str2);
    }

    //--------------------------------------------------------------------
    inline SFString operator+(const SFString& str,  char ch) {
        return operator+(str, SFString(ch));
    }

    //--------------------------------------------------------------------
    inline bool SFString::endsWith(const SFString& str) const {
        if (empty())
            return false;
        return (substr(length()-str.length(),str.length()) == str);
    }

    //--------------------------------------------------------------------
    inline bool SFString::startsWith(const SFString& str) const {
        if (empty())
            return false;
        return (substr(0,str.length()) == str);
    }

    //--------------------------------------------------------------------
    inline bool operator%(const SFString& str1, const SFString& str2) {
        return str1.Icompare(str2) == 0;
    }

    //--------------------------------------------------------------------
    inline bool operator%(const SFString& str1, const char *str2) {
        return str1.Icompare(str2) == 0;
    }

    //--------------------------------------------------------------------
    inline bool operator%(const char *str1, const SFString& str2) {
        return str2.Icompare(str1) == 0;
    }

    //--------------------------------------------------------------------
    inline bool operator%(const SFString& str1, char ch) {
        return str1.Icompare(SFString(ch)) == 0;
    }

    //--------------------------------------------------------------------
    inline bool operator%(char ch, const SFString& str2) {
        return str2.Icompare(SFString(ch)) == 0;
    }

    //--------------------------------------------------------------------
    inline size_t countOf(char c, const SFString& str) {
        size_t len = str.length();
        size_t i=0, cnt=0;
        while (i<len) {
            if (str.at(i) == c)
                cnt++;
            i++;
        }
        return cnt;
    }

    //--------------------------------------------------------------------
    extern SFString nextTokenClearReverse(SFString& str, char token);
    inline SFString nextTokenClear(SFString& line, char delim, bool doClear=true) {
        SFString ret;

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

    //--------------------------------------------------------------------
#define EMPTY SFString("")

    //--------------------------------------------------------------------
    inline SFString padRight(const SFString& str, uint32_t len, char p=' ') {
        if (len > str.length())
            return str + SFString(p, len-str.length());
        return str; //.substr(0,len);
    }

    //--------------------------------------------------------------------
    inline SFString padLeft(const SFString& str, uint32_t len, char p=' ') {
        if (len > str.length())
            return SFString(p, len-str.length()) + str;
        return str; //.substr(0,len);
    }

    //--------------------------------------------------------------------
    inline SFString padCenter(const SFString& str, size_t len, char p=' ') {
        if (len > str.length()) {
            size_t padding = (len-str.length()) / 2;
            return SFString(p, padding) + str + SFString(p, padding);
        }
        return str; //.substr(0,len);
    }

    //--------------------------------------------------------------------
    extern SFString snagFieldClear     (      SFString& in, const SFString& tagName, const SFString& defVal="");
    extern SFString snagField          (const SFString& in, const SFString& tagName, const SFString& defVal="");

    //--------------------------------------------------------------------
    inline SFString toLower(const SFString& in) {
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
    inline SFString toUpper(const SFString& in) {
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
    inline SFString toProper(const SFString& in) {
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
        ret.ReplaceAll("_", " ");
        return ret;
    }

    //--------------------------------------------------------------------
    extern void writeTheCode(const SFString& fileName, const SFString& code, const SFString& ns = "", bool spaces = true);

    //----------------------------------------------------------------------------
    inline SFString shorten(const SFString& in, size_t x) {
        return padRight(in.length()>x-3 ? in.substr(0,x-3) + "..." : in, (uint32_t)x);
    }

    //--------------------------------------------------------------------
    inline SFString StripTrailing(const SFString& str, char c) {
        SFString ret = str;
        while (ret.endsWith(c))
            ret = ret.substr(0,ret.length()-1);

        return ret;
    }

    //--------------------------------------------------------------------
    inline SFString StripLeading(const SFString& str, char c) {
        SFString ret = str;
        while (ret.startsWith(c))
            ret = ret.substr(1);

        return ret;
    }

    //--------------------------------------------------------------------
    inline SFString Strip(const SFString& str, char c) {
        return StripTrailing(StripLeading(str, c), c);
    }

    //--------------------------------------------------------------------
    inline SFString StripAny(const SFString& str, const SFString& any) {
        SFString ret = str;
        while (endsWithAny(ret, any) || startsWithAny(ret, any)) {
            for (size_t i = 0 ; i < any.length() ; i++)
                ret = Strip(ret, any[(int)i]);
        }
        return ret;
    }
}  // namespace qblocks
