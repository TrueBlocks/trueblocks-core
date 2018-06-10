#pragma once
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
#include "sfos.h"
#include "biglib.h"
#include "conversions_base.h"

namespace qblocks {

    //----------------------------------------------------------------------------
    inline uint64_t hex2Long(const SFString& inHex) {
        SFString hex = toLower(startsWith(inHex, "0x") ? inHex.substr(2) : inHex);
        reverse(hex);
        char *s = (char *)hex.c_str();

        uint64_t ret = 0, mult=1;
        while (*s) {
            int val = *s - '0';
            if (*s >= 'a' && *s <= 'f')
            val = *s - 'a' + 10;
            ret += (mult * (uint64_t)val);
            s++;mult*=16;
        }

        return ret;
    }

    //--------------------------------------------------------------------
    inline SFString asHex(char val) {
        char tmp[20];
        sprintf(tmp, "%02x", (unsigned int)(char)val);
        SFString ret = tmp;
        return ret.substr(ret.length()-2,2);
    }

    //----------------------------------------------------------------------------
    inline SFString string2Hex(const SFString& inAscii) {
        SFString ret;
        for (size_t i = 0 ; i < inAscii.length() ; i++)
            ret += asHex(inAscii[i]);
        return ret;
    }

    //--------------------------------------------------------------------
    inline SFString asString(int64_t i) {
        ostringstream os;
        os << i;
        return os.str().c_str();
    }

    //--------------------------------------------------------------------
    inline SFString asStringU(uint64_t i) {
        ostringstream os;
        os << i;
        return os.str().c_str();
    }

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
    inline SFString padNum4T(uint64_t n) { return padLeft(asStringU((n)), 4); }
    inline SFString padNum5T(uint64_t n) { return padLeft(asStringU((n)), 5); }
    inline SFString padNum6T(uint64_t n) { return padLeft(asStringU((n)), 6); }
    inline SFString padNum7T(uint64_t n) { return padLeft(asStringU((n)), 7); }
    inline SFString padNum8T(uint64_t n) { return padLeft(asStringU((n)), 8); }
    inline SFString padNum9T(uint64_t n) { return padLeft(asStringU((n)), 9); }

    //--------------------------------------------------------------------
    inline SFString padNum3T(uint32_t n) { return padLeft(asStringU((n)), 3); }
    inline SFString padNum4T(uint32_t n) { return padLeft(asStringU((n)), 4); }

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
        // trim leading '0's except the tens digit.
        SFString ret = _value;
        if (ret.length() < 18)
            ret = padLeft(_value, 18).Substitute(" ", "0");
        reverse(ret);
        ret = ret.substr(0,18) + "." + ret.substr(18);
        reverse(ret);
        ret = trimLeading(ret, '0');
        if (startsWith(ret, '.'))
            ret = "0" + ret;
        if (contains(ret, "0-")) {
            ret = "-" + ret.Substitute("0-","0");
        }
        ret = ret.Substitute("-.","-0.");
        return ret;
    }

    //-------------------------------------------------------------------------
    inline int64_t  toLong   (const string_q& str) { return (int64_t) strtol (str.c_str(), NULL, 10); }
    inline uint32_t toLong32u(const string_q& str) { return (uint32_t)strtoul(str.c_str(), NULL, 10); }

    extern string to_string(const SFUintBN& bu);
    extern string to_hex(const SFUintBN& bu);
    extern string to_string(const SFIntBN&  bi);

    extern SFUintBN hex2BigUint(const string& s);
    extern SFIntBN hex2BigInt(const string& s);
    extern SFUintBN str2BigUint(const string &s);
    #define toBigUint str2BigUint

    //--------------------------------------------------------------------------------
    inline SFUintBN str2BigUint(const SFString& s) {
        string ss = s.c_str(); return str2BigUint(ss);
    }

    extern SFIntBN exp2BigInt(const string& s);
    //--------------------------------------------------------------------------------
    inline SFUintBN exp2BigUint(const string &s) {
        SFString exponent = s.c_str();
        SFString decimals = nextTokenClear(exponent,'e');
        SFString num = nextTokenClear(decimals,'.');
        long nD = (long)decimals.length();
        uint64_t e = toLongU(exponent);
        SFUintBN ee = 1;
        uint64_t power = e - (uint64_t)nD;
        for (uint64_t i=0;i<power;i++)
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
    inline SFUintBN canonicalWei(uint64_t _value) {
        return SFUintBN(_value);
    }

    //--------------------------------------------------------------------------------
    inline SFUintBN canonicalWei(const SFString& _value) {
        if (contains(_value, "0x"))
            return hex2BigUint(_value.substr(2).c_str());
        if (contains(_value, "e"))
            return exp2BigUint(_value.c_str());
        return str2BigUint(_value);
    }

    //--------------------------------------------------------------------------------
    inline SFString asStringBN(const SFUintBN& bu) {
        return to_string(bu).c_str();
    }

    //--------------------------------------------------------------------------------
    inline SFString asStringBN(const SFIntBN& bn) {
        return to_string(bn).c_str();
    }

#define SFAddress      SFString
#define SFHash         SFString
#define SFBloom        SFUintBN
#define SFWei          SFUintBN
#define SFGas          uint64_t
#define blknum_t       uint64_t
#define txnum_t        uint64_t
    typedef SFArrayBase<SFAddress> SFAddressArray;
    typedef SFArrayBase<SFBloom> SFBloomArray;

#define toSigned(a)    (int64_t)(startsWith((a), "0x")?hex2Long((a)):toLong((a)))
#define toHash(a)      toLower(a)
#define toTopic(a)     canonicalWei(a)
#define toBloom(a)     canonicalWei(a)
#define toWei(a)       canonicalWei(a)
#define addr2BN        toWei
#define hex2BN         toWei

#define fromAddress(a)  ((a).empty() ? "0x0" : (a))
#define fromHash(a)     ((a).empty() ? "0x0" : (a))
#define fromWei(a)      to_string((a)).c_str()
#define fromTopic(a)    ("0x"+padLeft(toLower(SFString(to_hex((a)).c_str())),64,'0'))
#define fromGas(a)      asStringU(a)

    //-------------------------------------------------------------------------
    inline SFString toHex(const SFString& str) {
        if (str == "null")
            return str;
        SFUintBN bn = canonicalWei(str);
        return toLower("0x" + SFString(to_hex(bn).c_str()));
    }

    //-------------------------------------------------------------------------
    inline SFString toHex(uint64_t num) {
        SFUintBN bn = num;
        return toLower("0x" + SFString(to_hex(bn).c_str()));
    }

    extern SFString bloom2Bytes(const SFBloom& bl);
    extern SFString bloom2Bits(const SFBloom& b);

    //----------------------------------------------------------------------------------
    inline SFString fixHash(const SFString& hashIn) {
        SFString ret = startsWith(hashIn, "0x") ? hashIn.substr(2) : hashIn;
        return "0x" + padLeft(ret, 32 * 2, '0');
    }

    //----------------------------------------------------------------------------------
    inline bool zeroAddr(const SFAddress& addr) {
        return (addr2BN(addr) == 0);
    }

    //----------------------------------------------------------------------------------
    inline SFString fixAddress(const SFAddress& addrIn) {
        SFString ret = startsWith(addrIn, "0x") ? addrIn.substr(2) : addrIn;
        return "0x" + ret;
    }

    //----------------------------------------------------------------------------------
    inline bool isHexStr(const SFString& str) {
        if (!startsWith(str, "0x"))
            return false;
        for (size_t i = 2 ; i < str.length() ; i++)
            if (!isxdigit(str[i]))
                return false;
        return true;
    }

    //----------------------------------------------------------------------------
    inline bool isNumeral(const SFString& test) {
        for (size_t i = 0 ; i < test.length() ; i++)
            if (!isdigit(test[i]))
                return false;
        return true;
    }

    //----------------------------------------------------------------------------------
    inline bool isAddress(const SFAddress& addrIn) {
        return (addrIn.length() == 42 && isHexStr(addrIn));
    }

    //----------------------------------------------------------------------------------
    inline bool isHash(const SFHash& hashIn) {
        return (hashIn.length() == 66 && isHexStr(hashIn));
    }

    //-------------------------------------------------------------------------
    inline bool isUnsigned(const SFString& in) {
        // Empty string is not valid...
        if (in.empty())
            return false;
        // ...first character must be 0-9 (no negatives)...
        if (!isdigit(in[0]))
            return false;
        // ...or first two must be '0x' and the third must be non negative hex digit
        if (startsWith(in,"0x") && in.length() > 2)
            return isxdigit(in.at(2));
        return true;
    }

    //------------------------------------------------------
    inline SFAddress toAddress(const SFAddress& strIn) {
        // trim it if it's there. We will put it back
        SFString ret = strIn.Substitute("0x","");

        // Shorten, but only if all leading zeros
        SFString leading('0', 64-40);
        if (ret.length()==64 && startsWith(ret, leading))
            ret.Replace(leading,"");

        // Special case
        if (ret.empty())
            ret = "0";

        return "0x" + toLower(ret);
    }

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
