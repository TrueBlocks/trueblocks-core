#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "conversions_base.h"
#include "sftime.h"
#include "biglib.h"

namespace qblocks {

    //----------------------------------------------------------------------------
    inline bool isNumeral(const SFString& test) {
        for (int32_t i = 0 ; i < (int32_t)test.length() ; i++)
            if (!isdigit(test[i]))
                return false;
        return true;
    }

    //----------------------------------------------------------------------------
    inline uint64_t hex2Long(const SFString& inHex)
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
            ret += (mult * (uint64_t)val);
            s++;mult*=16;
        }

        return ret;
    }

    //----------------------------------------------------------------------------
    inline unsigned char hex2Ascii(char *str)
    {
        unsigned char c;
        c =  (unsigned char)((str[0] >= 'A' ? ((str[0]&0xDF)-'A')+10 : (str[0]-'0')));
        c *= 16;
        c = (unsigned char)(c + (str[1] >= 'A' ? ((str[1]&0xDF)-'A')+10 : (str[1]-'0')));
        return c;
    }

    //----------------------------------------------------------------------------
    inline SFString hex2String(const SFString& inHex)
    {
        SFString ret, in = inHex.startsWith("0x") ? inHex.substr(2) : inHex;
        while (!in.empty())
        {
            SFString nibble = in.Left(2);
            in = in.substr(2);
            ret += (char)hex2Ascii((char*)(const char*)nibble);
        }
        return ret;
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
    inline SFString string2Hex(const SFString& inAscii)
    {
        SFString ret;
        for (size_t i = 0 ; i < inAscii.length() ; i++)
            ret += asHex(inAscii[(int)i]);
        return ret;
    }

    //--------------------------------------------------------------------
    inline SFString asString(int64_t i) {
        ostringstream os;
        os << i;
        return os.str().c_str();
    }

    //--------------------------------------------------------------------
    inline SFString asStringU(SFUint32 i) {
        ostringstream os;
        os << i;
        return os.str().c_str();
    }

    //-------------------------------------------------------------------------
    inline int64_t toLong(const char *str)   { return strtol(str, NULL, 10); }
    inline uint64_t toLongU(const char *str) { return strtoul(str, NULL, 10); }

    //-------------------------------------------------------------------------
    inline uint32_t toLong32u(const char *str) { return (uint32_t)strtoul((const char*)(str), NULL, 10); }
    inline uint32_t toLong32u(char *str) { return (uint32_t)strtoul((const char*)(str), NULL, 10); }

    //-------------------------------------------------------------------------
    inline int32_t toLong32(const char *str) { return (int32_t)strtol((const char*)(str), NULL, 10); }
    inline int32_t toLong32(char *str) { return (int32_t)strtol((const char*)(str), NULL, 10); }

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
    inline SFString padNum3T(uint64_t n) { return padLeft(asStringU((n)), 3); }
    inline SFString padNum5T(uint64_t n) { return padLeft(asStringU((n)), 5); }
    inline SFString padNum6T(uint64_t n) { return padLeft(asStringU((n)), 6); }
    inline SFString padNum7T(uint64_t n) { return padLeft(asStringU((n)), 7); }
    inline SFString padNum8T(uint64_t n) { return padLeft(asStringU((n)), 8); }

    //--------------------------------------------------------------------
    inline SFString padNum2 (int64_t n) { return padLeft(asString((n)), 2, '0'); }
    inline SFString padNum3i(int64_t n) { return padLeft(asString((n)), 3, '0'); }
    inline SFString padNum4 (int64_t n) { return padLeft(asString((n)), 4, '0'); }
    inline SFString padNum5 (int64_t n) { return padLeft(asString((n)), 5, '0'); }
    inline SFString padNum6 (int64_t n) { return padLeft(asString((n)), 6, '0'); }
    inline SFString padNum7 (int64_t n) { return padLeft(asString((n)), 7, '0'); }
    inline SFString padNum8 (int64_t n) { return padLeft(asString((n)), 8, '0'); }
    inline SFString padNum9 (int64_t n) { return padLeft(asString((n)), 9, '0'); }

    //--------------------------------------------------------------------
    inline SFString padNum3T(int64_t n) { return padLeft(asString((n)), 3); }
    inline SFString padNum5T(int64_t n) { return padLeft(asString((n)), 5); }
    inline SFString padNum7T(int64_t n) { return padLeft(asString((n)), 7); }
    inline SFString padNum8T(int64_t n) { return padLeft(asString((n)), 8); }

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

    extern SFUintBN hex2BigUint(const string& s);
    extern SFIntBN hex2BigInt(const string& s);
    extern SFUintBN str2BigUint(const string &s);

    //--------------------------------------------------------------------------------
    inline SFUintBN str2BigUint(const SFString& s) {
        string ss = (const char*)s; return str2BigUint(ss);
    }

    extern SFIntBN exp2BigInt(const string& s);
    //--------------------------------------------------------------------------------
    inline SFUintBN exp2BigUint(const string &s)
    {
        SFString exponent = s.c_str();
        SFString decimals = nextTokenClear(exponent,'e');
        SFString num = nextTokenClear(decimals,'.');
        long nD = (long)decimals.length();
        SFUint32 e = toLongU(exponent);
        SFUintBN ee = 1;
        SFUint32 power = e - (SFUint32)nD;
        for (SFUint32 i=0;i<power;i++)
            ee *= 10;
        num += decimals;
        return str2BigUint(num) * ee;
    }

    //--------------------------------------------------------------------------------
    inline SFIntBN str2BigInt(const string &s) {
        return (s[0] == '-') ? SFIntBN(str2BigUint(s.substr(1, s.length() - 1)), -1)
        : (s[0] == '+') ? SFIntBN(str2BigUint(s.substr(1, s.length() - 1)))
        : SFIntBN(str2BigUint(s));
    }

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
#define SFGas          uint64_t
#define blknum_t       uint64_t
#define txnum_t        uint64_t

#define toUnsigned(a)  (uint64_t)((a).startsWith("0x")?hex2Long((a)):toLongU((a)))
#define toSigned(a)    (int64_t)((a).startsWith("0x")?hex2Long((a)):toLong((a)))
#define toHash(a)      (a)
#define toTopic(a)     canonicalWei(a)
#define toBloom(a)     canonicalWei(a)
#define toWei(a)       canonicalWei(a)
#define toGas(a)       toUnsigned(a)
#define addr2BN        toWei

#define fromAddress(a)  ((a).empty() ? "0x0" : (a))
#define fromHash(a)     ((a).empty() ? "0x0" : (a))
#define fromWei(a)      to_string((a)).c_str()
#define fromTopic(a)    ("0x"+padLeft(toLower(SFString(to_hex((a)).c_str())),64,'0'))
#define fromGas(a)      asStringU(a)
#define toHex2(a)       (a == "null" ? "null" : ("0x"+toLower(SFString(to_hex(str2BigUint(a)).c_str()))))

	extern  SFString   fromBloom(const SFBloom& bl);
#define fromUnsigned(a) asStringU((a))

    //--------------------------------------------------------------------------------------------------------------
    extern SFTime      dateFromTimeStamp(timestamp_t tsIn);
    extern SFTime      snagDate(const SFString& str, int dir = 0);  // -1 BOD, 0 MIDDAY, 1 EOD

    extern timestamp_t toTimestamp(const SFTime&   timeIn);
    extern timestamp_t toTimestamp(const SFString& timeIn);
    inline SFString    fromTimestamp(timestamp_t ts) { return asString(ts); }

    //----------------------------------------------------------------------------------
    inline SFString fixHash(const SFString& hashIn) {
        SFString ret = hashIn.startsWith("0x") ? hashIn.substr(2) : hashIn;
        return "0x" + padLeft(ret, 32 * 2, '0');
    }

    //----------------------------------------------------------------------------------
    inline bool zeroAddr(const SFAddress& addr) {
        return (addr2BN(addr) == 0);
    }

    //----------------------------------------------------------------------------------
    inline SFString fixAddress(const SFString& addrIn) {
        SFString ret = addrIn.startsWith("0x") ? addrIn.substr(2) : addrIn;
        return "0x" + ret;
    }

    //----------------------------------------------------------------------------------
    inline bool isAddress(const SFAddress& addrIn) {
        return (addrIn.length() == 42 && isHexStr(addrIn));
    }

    //----------------------------------------------------------------------------------
    inline bool isHash(const SFAddress& hashIn) {
        return (hashIn.length() == 66 && isHexStr(hashIn));
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
    //-------------------------------------------------------------------------
    inline bool isUnsigned(const SFString& in) {
        // empty string is not valid
        if (in.empty())
            return false;

        // first character must be 0-9 (no negatives)
        if (!isdigit(in[0]))
            return false;

        return (!in.startsWith("0x") || isHex(in.at(2)));
    }

#define newUnsigned64(a)  ((a).startsWith("0x") ?    (uint64_t)hex2Long((a)) :    (uint64_t)toLongU((a)))
#define newSigned64(a)    ((a).startsWith("0x") ?    ( int64_t)hex2Long((a)) :    ( int64_t)toLong ((a)))

#define newUnsigned32(a)  ((a).startsWith("0x") ?    (uint32_t)hex2Long((a)) :    (uint32_t)toLongU((a)))
#define newSigned32(a)    ((a).startsWith("0x") ?    ( int32_t)hex2Long((a)) :    ( int32_t)toLong ((a)))

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

}  // namespace qblocks
