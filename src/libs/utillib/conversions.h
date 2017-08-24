#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "sftime.h"
#include "biglib.h"

namespace qblocks {

    //--------------------------------------------------------------------------------------------------------------
    extern SFTime dateFromTimeStamp(timestamp_t tsIn);
    extern timestamp_t toTimeStamp(const SFTime& timeIn);
    extern SFTime snagDate(const SFString& str, int dir = 0);  // -1 BOD, 0 MIDDAY, 1 EOD

    //--------------------------------------------------------------------
    inline SFString formatFloat(double f, uint32_t nDecimals=10) {
        char s[100], r[100];
        memset(s,'\0',100);
        memset(r,'\0',100);
        sprintf(s, "%.*g", (int)nDecimals, ((int64_t)(  pow(10, nDecimals) * (  fabs(f) - labs( (int64_t) f )  )  + 0.5)) / pow(10,nDecimals));
        if (strchr(s, 'e'))
            s[1] = '\0';
        sprintf(r, "%d%s", (int)f, s+1);
        return SFString(r);
    }
#define fmtFloat(f) (const char*)formatFloat(f)
#define fmtFloatp(f,p) (const char*)formatFloat(f, p)

    //--------------------------------------------------------------------
    extern uint64_t hex2Long(const SFString& inHex);
    extern SFString hex2String(const SFString& inHex);
    extern SFString string2Hex(const SFString& inAscii);
    extern unsigned char hex2Ascii(char *str);

    //--------------------------------------------------------------------
    inline SFString asString(int64_t i) {
#if 1
        ostringstream os;
        os << i;
        return os.str().c_str();
#else
        char ret[128];
        sprintf(ret, "%ld", (int64_t)i);
        return SFString(ret);
#endif
    }

    //--------------------------------------------------------------------
    inline SFString asStringU(SFUint32 i) {
#if 1
        ostringstream os;
        os << i;
        return os.str().c_str();
#else
        char ret[128];
        sprintf(ret, "%lu", (SFUint32)i);
        return SFString(ret);
#endif
    }

    //--------------------------------------------------------------------
    inline SFString asStringULL(uint64_t i) {
#if 1
        ostringstream os;
        os << i;
        return os.str().c_str();
#else
        char ret[128];
        sprintf(ret, "%lu", (uint64_t)i);
        return SFString(ret);
#endif
    }

    //--------------------------------------------------------------------
    inline SFString asStringF(float f) {
        return formatFloat(f, 10);
    }

    //--------------------------------------------------------------------
    inline SFString asStringD(double d) {
        return formatFloat(d, 10);
    }

    //--------------------------------------------------------------------
    inline SFString asBitmap(uint64_t value) {
        SFString ret;
        for (int i=31;i>-1;i--) {
            bool isOn = (value & (1<<i));
            if (isOn)
                ret += "1";
            else
                ret += "0";
        }
        return ret;
    }
    
    //-------------------------------------------------------------------------
    inline int64_t toLong(const char *str)   { return strtol(str, NULL, 10); }
    inline uint64_t toLongU(const char *str) { return strtoul(str, NULL, 10); }

    //-------------------------------------------------------------------------
    inline uint32_t toLong32u(const char *str) { return (uint32_t)strtoul((const char*)(str), NULL, 10); }
    inline uint32_t toLong32u(char *str) { return (uint32_t)strtoul((const char*)(str), NULL, 10); }

    //-------------------------------------------------------------------------
    inline float toFloat(const char *str) { return (float)strtof((const char*)(str), NULL); }
    inline float toFloat(char *str) { return (float)strtof((const char*)(str), NULL); }

    //-------------------------------------------------------------------------
    inline double toDouble(const char *str) { return (double)strtold((const char*)(str), NULL); }
    inline double toDouble(char *str) { return (double)strtold((const char*)(str), NULL); }

    //--------------------------------------------------------------------
    inline bool toBool_in(const SFString& in) { return in%"true" || toLong(in)!=0; }
#define toBool toBool_in
    
    //--------------------------------------------------------------------
    inline SFString padNum2(uint64_t n) { return padLeft(asStringU((n)), 2, '0'); }
    inline SFString padNum3(uint64_t n) { return padLeft(asStringU((n)), 3, '0'); }
    inline SFString padNum4(uint64_t n) { return padLeft(asStringU((n)), 4, '0'); }
    inline SFString padNum5(uint64_t n) { return padLeft(asStringU((n)), 5, '0'); }
    inline SFString padNum6(uint64_t n) { return padLeft(asStringU((n)), 6, '0'); }
    inline SFString padNum7(uint64_t n) { return padLeft(asStringU((n)), 7, '0'); }
    inline SFString padNum8(uint64_t n) { return padLeft(asStringU((n)), 8, '0'); }
    inline SFString padNum9(uint64_t n) { return padLeft(asStringU((n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline SFString padNum2T(uint64_t n) { return padLeft(asStringU((n)), 2); }
    inline SFString padNum3T(uint64_t n) { return padLeft(asStringU((n)), 3); }
    inline SFString padNum4T(uint64_t n) { return padLeft(asStringU((n)), 4); }
    inline SFString padNum5T(uint64_t n) { return padLeft(asStringU((n)), 5); }
    inline SFString padNum6T(uint64_t n) { return padLeft(asStringU((n)), 6); }
    inline SFString padNum7T(uint64_t n) { return padLeft(asStringU((n)), 7); }
    inline SFString padNum8T(uint64_t n) { return padLeft(asStringU((n)), 8); }
    inline SFString padNum9T(uint64_t n) { return padLeft(asStringU((n)), 9); }
    inline SFString padNum18T(uint64_t n) { return padLeft(asStringU((n)), 18); }

    //--------------------------------------------------------------------
    inline SFString padNum2(int64_t n) { return padLeft(asString((n)), 2, '0'); }
    inline SFString padNum3i(int64_t n) { return padLeft(asString((n)), 3, '0'); }
    inline SFString padNum4(int64_t n) { return padLeft(asString((n)), 4, '0'); }
    inline SFString padNum5(int64_t n) { return padLeft(asString((n)), 5, '0'); }
    inline SFString padNum6(int64_t n) { return padLeft(asString((n)), 6, '0'); }
    inline SFString padNum7(int64_t n) { return padLeft(asString((n)), 7, '0'); }
    inline SFString padNum8(int64_t n) { return padLeft(asString((n)), 8, '0'); }
    inline SFString padNum9(int64_t n) { return padLeft(asString((n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline SFString padNum2T(int64_t n) { return padLeft(asString((n)), 2); }
    inline SFString padNum3T(int64_t n) { return padLeft(asString((n)), 3); }
    inline SFString padNum4T(int64_t n) { return padLeft(asString((n)), 4); }
    inline SFString padNum5T(int64_t n) { return padLeft(asString((n)), 5); }
    inline SFString padNum6T(int64_t n) { return padLeft(asString((n)), 6); }
    inline SFString padNum7T(int64_t n) { return padLeft(asString((n)), 7); }
    inline SFString padNum8T(int64_t n) { return padLeft(asString((n)), 8); }
    inline SFString padNum9T(int64_t n) { return padLeft(asString((n)), 9); }
    inline SFString padNum18T(int64_t n) { return padLeft(asString((n)), 18); }
    
#define toEther wei2Ether
    inline SFString wei2Ether(const SFString& _value) {
        // Make sure the wei number is at least 18 characters long. Once it is,
        // reverse it, put a decimal at the 18th position, reverse it back,
        // strip leading '0's except the tens digit.
        SFString ret = _value;
        if (ret.length() < 18)
            ret = padLeft(_value, 18).Substitute(" ", "0");
        ret.Reverse();
        ret = ret.Left(18) + "." + ret.substr(18);
        ret.Reverse();
        ret = StripLeading(ret, '0');
        if (ret.startsWith('.'))
            ret = "0" + ret;
        if (ret.Contains("0-")) {
            ret = "-" + ret.Substitute("0-","0");
        }
        ret = ret.Substitute("-.","-0.");
        return ret;
    }
    extern string to_string(const SFUintBN& bu);
    extern string to_hex(const SFUintBN& bu);
    extern string to_string(const SFIntBN&  bi);

    extern SFUintBN str2BigUint(const string& s);
    inline SFUintBN str2BigUint(const SFString& s) { string ss = (const char*)s; return str2BigUint(ss); }
    extern SFIntBN str2BigInt(const string& s);

    extern SFUintBN hex2BigUint(const string& s);
    extern SFIntBN hex2BigInt(const string& s);

    extern SFUintBN exp2BigUint(const string& s);
    extern SFIntBN exp2BigInt(const string& s);
    //-------------------------------------------------------------------------
    inline SFUintBN canonicalWei(SFUint32 _value)
    {
        return SFUintBN(_value);
    }

    inline SFUintBN canonicalWei(const SFString& _value)
    {
        if (_value.Contains( "0x" ))
            return hex2BigUint((const char*) _value.substr(2));
        if (_value.Contains( "e"  ))
            return exp2BigUint((const char*) _value);
        return str2BigUint(_value);
    }

    inline SFUint32 canonicalWeiL(SFUint32 _value)
    {
        return _value;
    }

    inline SFUint32 canonicalWeiL(const SFString& _value)
    {
        extern SFUint32 toLong(const SFString& in);
        return toLong(_value);
    }

    inline SFString asStringBN(const SFUintBN& bu)
    {
        return to_string(bu).c_str();
    }

    inline SFString asStringBN(const SFIntBN& bn)
    {
        return to_string(bn).c_str();
    }
#define SFAddress      SFString
    typedef SFArrayBase<SFAddress> SFAddressArray;
#define SFHash         SFString
#define SFBloom        SFUintBN
#define SFWei          SFUintBN
#define blknum_t       uint64_t
#define txnum_t        uint32_t
#define lognum_t       uint32_t

#define toHash(a)      (a)
#define toTopic(a)     canonicalWei(a)
#define toBloom(a)     canonicalWei(a)
#define toWei(a)       canonicalWei(a)
#define toUnsigned(a)  (SFUint32)((a).startsWith("0x")?hex2Long((a)):toLongU((a)))

#define fromAddress(a)  ((a).empty() ? "0x0" : (a))
#define fromHash(a)     ((a).empty() ? "0x0" : (a))
#define fromWei(a)      to_string((a)).c_str()
#define fromTopic(a)    ("0x"+padLeft(toLower(SFString(to_hex((a)).c_str())),64,'0'))
#define fromBloom(a)    ((a)==0?"0x0":"0x"+padLeft(toLower(SFString(to_hex((a)).c_str())),512,'0'))
#define fromUnsigned(a) asStringU((a))

#define addr2BN toWei
    //----------------------------------------------------------------------------------
    inline bool zeroAddr(const SFAddress& addr) {
        return (addr2BN(addr) == 0);
    }

    //------------------------------------------------------
    inline SFAddress toAddress(const SFString& strIn)
    {
        // Strip it if it's there. We will put it back
        SFString ret = strIn.Substitute("0x","");

        // Shorten, but only if all leading zeros
        if (ret.length()==64 && ret.startsWith("000000000000000000000000"))
            ret.Replace("000000000000000000000000","");

        // Special case
        if (ret.empty())
            ret = "0";

        return "0x" + ret;
    }

    //-------------------------------------------------------------------------
    inline uint32_t bitsTwiddled(SFBloom n) {
        uint32_t count = 0;
        while (n != 0) {
            SFUintBN x = n - 1;
            SFUintBN y = n & x;
            n = y;
            count++;
        }
        return count;
    }

    // NEW_CODE
    inline bool isUnsigned(const SFString& in) { return in.length() && isdigit(in[0]); } // negative '-' will fail

#define newTimestamp(a)   ((a).startsWith("0x") ? (timestamp_t)hex2Long((a)) : (timestamp_t)toLong ((a)))

#define newUnsigned64(a)  ((a).startsWith("0x") ?    (uint64_t)hex2Long((a)) :    (uint64_t)toLongU((a)))
#define newSigned64(a)    ((a).startsWith("0x") ?    ( int64_t)hex2Long((a)) :    ( int64_t)toLong ((a)))

#define newUnsigned32(a)  ((a).startsWith("0x") ?    (uint32_t)hex2Long((a)) :    (uint32_t)toLongU((a)))
#define newSigned32(a)    ((a).startsWith("0x") ?    ( int32_t)hex2Long((a)) :    ( int32_t)toLong ((a)))
    
}  // namespace qblocks
